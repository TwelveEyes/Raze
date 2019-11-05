// On-screen Display (ie. console)
// for the Build Engine
// by Jonathon Fowler (jf@jonof.id.au)

#include "build.h"
#include "cache1d.h"
#include "compat.h"
#include "baselayer.h"
#include "osd.h"
#include "scancodes.h"
#include "common.h"
#include "c_cvars.h"
#include "inputstate.h"
#include "keyboard.h"
#include "control.h"
#include "gamecontrol.h"
#include "m_crc32.h"
#include "c_commandline.h"
#include "c_bind.h"
#include "c_dispatch.h"

#include "vfs.h"

static int32_t whiteColorIdx=-1;            // colour of white (used by default display routines)
static void _internal_drawosdchar(int32_t, int32_t, char, int32_t, int32_t);
static void _internal_drawosdstr(int32_t, int32_t, const char *, int32_t, int32_t, int32_t);
static void _internal_drawosdcursor(int32_t,int32_t,int32_t,int32_t);
static int32_t _internal_getcolumnwidth(int32_t);
static int32_t _internal_getrowheight(int32_t);
static void _internal_clearbackground(int32_t,int32_t);
static int32_t _internal_gettime(void);
static void _internal_onshowosd(int32_t);

osdmain_t *osd;

static int osdrowscur = -1;
static int osdmaxrows = 20;

buildvfs_FILE osdlog;

const char *osdlogfn;

static uint32_t osdkeytime = 0;
static uint32_t osdscrtime = 0;

#define OSD_EDIT_LINE_WIDTH (osd->draw.cols - 1 - 3)

static hashtable_t h_osd = { OSDMAXSYMBOLS >> 1, NULL };

// Application callbacks: these are the currently effective ones.
static void (*drawosdchar)(int32_t, int32_t, char, int32_t, int32_t) = _internal_drawosdchar;
static void (*drawosdstr)(int32_t, int32_t, const char *, int32_t, int32_t, int32_t) = _internal_drawosdstr;
static void (*drawosdcursor)(int32_t, int32_t, int32_t, int32_t) = _internal_drawosdcursor;
static int32_t (*getcolumnwidth)(int32_t) = _internal_getcolumnwidth;
static int32_t (*getrowheight)(int32_t) = _internal_getrowheight;

static void (*clearbackground)(int32_t,int32_t) = _internal_clearbackground;
static int32_t (*gettime)(void) = _internal_gettime;
static void (*onshowosd)(int32_t) = _internal_onshowosd;

// Application callbacks: these are the backed-up ones.
static void (*_drawosdchar)(int32_t, int32_t, char, int32_t, int32_t) = _internal_drawosdchar;
static void (*_drawosdstr)(int32_t, int32_t, const char *, int32_t, int32_t, int32_t) = _internal_drawosdstr;
static void (*_drawosdcursor)(int32_t, int32_t, int32_t, int32_t) = _internal_drawosdcursor;
static int32_t (*_getcolumnwidth)(int32_t) = _internal_getcolumnwidth;
static int32_t (*_getrowheight)(int32_t) = _internal_getrowheight;

static hashtable_t h_cvars      = { OSDMAXSYMBOLS >> 1, NULL };
bool m32_osd_tryscript = false;  // whether to try executing m32script on unkown command in the osd


int OSD_ParsingScript(void) { return osd->execdepth; }
int OSD_OSDKey(void)        { return osd->keycode; }
int OSD_GetCols(void)       { return osd->draw.cols; }
int OSD_IsMoving(void)      { return (osdrowscur != -1 && osdrowscur != osd->draw.rows); }
int OSD_GetRowsCur(void)    { return osdrowscur; }
int OSD_GetTextMode(void)   { return osd->draw.mode; }

void OSD_GetShadePal(const char *ch, int *shd, int *pal)
{
    auto &t = osd->text;

    if (ch < t.buf || ch >= t.buf + OSDBUFFERSIZE)
        return;

    *shd = (t.fmt[ch-t.buf] & ~0x1F)>>4;
    *pal = (t.fmt[ch-t.buf] & ~0xE0);
}

// XXX: well, converting function pointers to "data pointers" (void *) is
// undefined behavior. See
//  http://blog.frama-c.com/index.php?post/2013/08/24/Function-pointers-in-C
// Then again, my GCC just crashed (any kept on crashing until after a reboot!)
// when I tried to rewrite this into something different.

static inline void swaposdptrs(void)
{
    swapptr(&_drawosdchar,    &drawosdchar);
    swapptr(&_drawosdstr,     &drawosdstr);
    swapptr(&_drawosdcursor,  &drawosdcursor);
    swapptr(&_getcolumnwidth, &getcolumnwidth);
    swapptr(&_getrowheight,   &getrowheight);
}

void OSD_SetTextMode(int mode)
{
	osd->draw.mode = 1;// (mode != 0);

    if ((osd->draw.mode && drawosdchar != _internal_drawosdchar) ||
        (!osd->draw.mode && drawosdchar == _internal_drawosdchar))
        swaposdptrs();

    if (in3dmode())
        OSD_ResizeDisplay(xdim, ydim);
}

static void _internal_drawosdchar(int x, int y, char ch, int shade, int pal)
{
    UNREFERENCED_PARAMETER(shade);
    UNREFERENCED_PARAMETER(pal);

    char st[2] = { ch, 0 };

    printext256(4+(x<<3),4+(y<<3), whiteColorIdx, -1, st, 0);
}

static void _internal_drawosdstr(int x, int y, const char *ch, int len, int shade, int pal)
{
    char st[1024];

    UNREFERENCED_PARAMETER(shade);

    if (len>1023) len=1023;
    Bmemcpy(st,ch,len);
    st[len]=0;

    OSD_GetShadePal(ch, &shade, &pal);

    {
        int32_t colidx = whiteColorIdx >= 0 ? palookup[(uint8_t)pal][whiteColorIdx] : whiteColorIdx;
        printext256(4+(x<<3),4+(y<<3), colidx, -1, st, 0);
    }
}

static void _internal_drawosdcursor(int x, int y, int flags, int lastkeypress)
{
    char st[2] = { '_',0 };

    UNREFERENCED_PARAMETER(lastkeypress);

    if (flags) st[0] = '#';

    if (whiteColorIdx > -1)
    {
        printext256(4+(x<<3),4+(y<<3)+2, whiteColorIdx, -1, st, 0);
        return;
    }

    // Find the palette index closest to Duke3D's brightest blue
    // "foreground" color.  (Index 79, or the last column of the 5th row,
    // if the palette is laid out in a 16x16 pattern.)
    for (int i=0, k=UINT8_MAX+1; i<256; i++)
    {
        int const j =
            klabs(curpalette[i].r - 4*47) +
            klabs(curpalette[i].g - 4*55) +
            klabs(curpalette[i].b - 4*63);
        if (j < k) { k = j; whiteColorIdx = i; }
    }
}

static int _internal_getcolumnwidth(int w)
{
    return w/8 - 1;
}

static int _internal_getrowheight(int w)
{
    return w/8;
}

void COMMON_doclearbackground(int numcols, int height);

static void _internal_clearbackground(int cols, int rows)
{
	COMMON_doclearbackground(cols, rows);
}

static int32_t _internal_gettime(void)
{
    return BGetTime();
}

static void _internal_onshowosd(int a)
{
    UNREFERENCED_PARAMETER(a);
}


//
// OSD_Cleanup() -- Cleans up the on-screen display
//
void OSD_Cleanup(void)
{
    hash_free(&h_osd);
    hash_free(&h_cvars);

    for (auto &symb : osd->symbptrs)
        DO_FREE_AND_NULL(symb);

    osd->symbols = NULL;

    for (auto & i : osd->history.buf)
        DO_FREE_AND_NULL(i);

    DO_FREE_AND_NULL(osd->cvars);

    DO_FREE_AND_NULL(osd->editor.buf);
    DO_FREE_AND_NULL(osd->editor.tmp);

    DO_FREE_AND_NULL(osd->text.buf);
    DO_FREE_AND_NULL(osd->text.fmt);

    DO_FREE_AND_NULL(osd->version.buf);

    MAYBE_FCLOSE_AND_NULL(osdlog);
    DO_FREE_AND_NULL(osd);
}


//
// OSD_Init() -- Initializes the on-screen display
//
void OSD_Init(void)
{
    osd = (osdmain_t *)Xcalloc(1, sizeof(osdmain_t));

    mutex_init(&osd->mutex);

    if (!osd->keycode)
        osd->keycode = sc_Tilde;

    osd->text.buf   = (char *)Xmalloc(OSDBUFFERSIZE);
    osd->text.fmt   = (char *)Xmalloc(OSDBUFFERSIZE);
    osd->editor.buf = (char *)Xmalloc(OSDEDITLENGTH);
    osd->editor.tmp = (char *)Xmalloc(OSDEDITLENGTH);

    Bmemset(osd->text.buf, asc_Space, OSDBUFFERSIZE);
    Bmemset(osd->text.fmt, osd->draw.textpal + (osd->draw.textshade<<5), OSDBUFFERSIZE);
    Bmemset(osd->symbptrs, 0, sizeof(osd->symbptrs));

    osd->numsymbols    = 0;
    osd->numcvars      = 0;
    osd->text.lines    = 1;
    osd->text.maxlines = OSDDEFAULTMAXLINES;  // overwritten later
    osd->draw.rows     = OSDDEFAULTROWS;
    osd->draw.cols     = OSDDEFAULTCOLS;
    osd->log.cutoff    = OSDLOGCUTOFF;
	OSD_SetTextMode(1);
    osd->history.maxlines = OSDMINHISTORYDEPTH;

    hash_init(&h_osd);
    hash_init(&h_cvars);
}


//
// OSD_SetLogFile() -- Sets the text file where printed text should be echoed
//
void OSD_SetLogFile(const char *fn)
{
    MAYBE_FCLOSE_AND_NULL(osdlog);
    osdlogfn = NULL;

    if (!fn)
        return;

    osdlog = buildvfs_fopen_write_text(fn);

    if (osdlog)
    {
        const int bufmode = _IOLBF;
        setvbuf(osdlog, (char *)NULL, bufmode, BUFSIZ);
        osdlogfn = fn;
    }
}

//
// OSD_SetParameters() -- Sets the parameters for presenting the text
//
void OSD_SetParameters(int promptShade, int promptPal, int editShade, int editPal, int textShade, int textPal,
                       char const *errorStr, char const *highlight, uint32_t flags)
{
    osddraw_t &draw = osd->draw;

    draw.promptshade = promptShade;
    draw.promptpal   = promptPal;
    draw.editshade   = editShade;
    draw.editpal     = editPal;
    draw.textshade   = textShade;
    draw.textpal     = textPal;
    draw.errorfmt    = errorStr;
    draw.highlight   = highlight;

    osd->flags |= flags;
}


//
// OSD_CaptureKey() -- Sets the scancode for the key which activates the onscreen display
//
void OSD_CaptureKey(uint8_t scanCode)
{
    osd->keycode = scanCode;
}

static void OSD_AdjustEditorPosition(osdedit_t &e)
{
    e.pos = 0;
    while (e.buf[e.pos])
        e.pos++;
    e.len = e.pos;

    if (e.pos < e.start)
    {
        e.end   = e.pos;
        e.start = e.end - OSD_EDIT_LINE_WIDTH;

        if (e.start < 0)
        {
            e.end -= e.start;
            e.start = 0;
        }
    }
    else if (e.pos >= e.end)
    {
        e.start += (e.pos - e.end);
        e.end += (e.pos - e.end);
    }
}

static void OSD_HistoryPrev(void)
{
    osdhist_t &h = osd->history;

    if (h.pos >= h.lines - 1)
        return;

    osdedit_t &e = osd->editor;

    Bmemcpy(e.buf, h.buf[++h.pos], OSDEDITLENGTH);

    OSD_AdjustEditorPosition(e);
}

static void OSD_HistoryNext(void)
{
    osdhist_t &h = osd->history;

    if (h.pos < 0)
        return;

    osdedit_t &e = osd->editor;

    if (h.pos == 0)
    {
        e.len   = 0;
        e.pos   = 0;
        e.start = 0;
        e.end   = OSD_EDIT_LINE_WIDTH;
        h.pos  = -1;

        return;
    }

    Bmemcpy(e.buf, h.buf[--h.pos], OSDEDITLENGTH);

    OSD_AdjustEditorPosition(e);
}

//
// OSD_HandleKey() -- Handles keyboard input when capturing input.
//  Returns 0 if the key was handled internally, or the scancode if it should
//  be passed on to the game.
//

int OSD_HandleChar(char ch)
{
    if (!osd || (osd->flags & OSD_CAPTURE) != OSD_CAPTURE)
        return ch;

    osdhist_t &h  = osd->history;
    osdedit_t &ed = osd->editor;

    osdsymbol_t *       tabc      = NULL;
    static osdsymbol_t *lastmatch = NULL;

    if (ch != asc_Tab)
        lastmatch = NULL;

    switch (ch)
    {
        case asc_Ctrl_A:  // jump to beginning of line
            ed.pos   = 0;
            ed.start = 0;
            ed.end   = OSD_EDIT_LINE_WIDTH;
            return 0;

        case asc_Ctrl_B:  // move one character left
            if (ed.pos > 0)
                --ed.pos;
            return 0;

        case asc_Ctrl_C:  // discard line
            ed.buf[ed.len] = 0;
            OSD_Printf("%s\n", ed.buf);

            ed.len    = 0;
            ed.pos    = 0;
            ed.start  = 0;
            ed.end    = OSD_EDIT_LINE_WIDTH;
            ed.buf[0] = 0;
            return 0;

        case asc_Ctrl_E:  // jump to end of line
            ed.pos   = ed.len;
            ed.end   = ed.pos;
            ed.start = ed.end - OSD_EDIT_LINE_WIDTH;

            if (ed.start < 0)
            {
                ed.start = 0;
                ed.end   = OSD_EDIT_LINE_WIDTH;
            }
            return 0;

        case asc_Ctrl_F:  // move one character right
            if (ed.pos < ed.len)
                ed.pos++;
            return 0;

        case asc_BackSpace:
#ifdef __APPLE__
        case 127:  // control h, backspace
#endif
            if (!ed.pos || !ed.len)
                return 0;

            if ((osd->flags & OSD_OVERTYPE) == 0)
            {
                if (ed.pos < ed.len)
                    Bmemmove(ed.buf + ed.pos - 1, ed.buf + ed.pos, ed.len - ed.pos);
                ed.len--;
            }

            if (--ed.pos < ed.start)
                ed.start--, ed.end--;
#ifndef __APPLE__
        fallthrough__;
        case 127:  // handled in OSD_HandleScanCode (delete)
#endif
            return 0;

        case asc_Tab:  // tab
        {
			// Todo: redirect to the new dispatcher
            return 0;
        }

        case asc_Ctrl_K:  // delete all to end of line
            Bmemset(ed.buf + ed.pos, 0, OSDEDITLENGTH - ed.pos);
            ed.len = ed.pos;
            return 0;

        case asc_Ctrl_L:  // clear screen
            Bmemset(osd->text.buf, 0, OSDBUFFERSIZE);
            Bmemset(osd->text.fmt, osd->draw.textpal + (osd->draw.textshade << 5), OSDBUFFERSIZE);
            osd->text.lines = 1;
            return 0;

        case asc_Enter:  // control m, enter
            if (ed.len > 0)
            {
                ed.buf[ed.len] = 0;
                if (!h.buf[0] || Bstrcmp(h.buf[0], ed.buf))
                {
                    DO_FREE_AND_NULL(h.buf[h.maxlines - 1]);
                    Bmemmove(&h.buf[1], &h.buf[0], sizeof(intptr_t) * h.maxlines - 1);
                    OSD_SetHistory(0, ed.buf);

                    if (h.lines < h.maxlines)
                        h.lines++;

                    h.total++;
                }

                if (h.exec++ == h.maxlines)
                    OSD_Printf("Buffer full! Consider increasing \"osdhistorydepth\" beyond %d.\n", --h.exec);

                h.pos = -1;
            }

            ed.len   = 0;
            ed.pos   = 0;
            ed.start = 0;
            ed.end   = OSD_EDIT_LINE_WIDTH;
            return 0;

        case asc_Ctrl_N:  // next (ie. down arrow)
            OSD_HistoryNext();
            return 0;

        case asc_Ctrl_P:  // previous (ie. up arrow)
            OSD_HistoryPrev();
            return 0;

        case asc_Ctrl_U:  // delete all to beginning
            if (ed.pos > 0 && ed.len)
            {
                if (ed.pos < ed.len)
                    Bmemmove(ed.buf, ed.buf + ed.pos, ed.len - ed.pos);

                ed.len -= ed.pos;
                ed.pos   = 0;
                ed.start = 0;
                ed.end   = OSD_EDIT_LINE_WIDTH;
            }
            return 0;

        case asc_Ctrl_W:  // delete one word back
            if (ed.pos > 0 && ed.len > 0)
            {
                int editPos = ed.pos;

                while (editPos > 0 && ed.buf[editPos - 1] == asc_Space) editPos--;
                while (editPos > 0 && ed.buf[editPos - 1] != asc_Space) editPos--;

                if (ed.pos < ed.len)
                    Bmemmove(ed.buf + editPos, ed.buf + ed.pos, ed.len - ed.pos);

                ed.len -= (ed.pos - editPos);
                ed.pos = editPos;

                if (ed.pos < ed.start)
                {
                    ed.start = ed.pos;
                    ed.end   = ed.start + OSD_EDIT_LINE_WIDTH;
                }
            }
            return 0;

        default:
            if (ch >= asc_Space)  // text char
            {
                if ((osd->flags & OSD_OVERTYPE) == 0)
                {
                    if (ed.len == OSDEDITLENGTH)  // buffer full, can't insert another char
                        return 0;

                    if (ed.pos < ed.len)
                        Bmemmove(ed.buf + ed.pos + 1, ed.buf + ed.pos, ed.len - ed.pos);

                    ed.len++;
                }
                else if (ed.pos == ed.len)
                    ed.len++;

                ed.buf[ed.pos++] = ch;

                if (ed.pos > ed.end)
                    ed.start++, ed.end++;
            }
            return 0;
    }
    return 0;
}

int OSD_HandleScanCode(uint8_t scanCode, int keyDown)
{
    if (!osd)
        return 1;

    osddraw_t &draw = osd->draw;

    if (scanCode == osd->keycode && (inputState.ShiftPressed() || (osd->flags & OSD_CAPTURE) || (osd->flags & OSD_PROTECTED) != OSD_PROTECTED))
    {
        if (keyDown)
        {
            draw.scrolling = (osdrowscur == -1) ? 1 : (osdrowscur == draw.rows) ? -1 : -draw.scrolling;
            osdrowscur += draw.scrolling;
            OSD_CaptureInput(draw.scrolling == 1);
            osdscrtime = timerGetTicks();
        }
        return -1;
    }
    else if ((osd->flags & OSD_CAPTURE) == 0)
        return 2;

    if (!keyDown)
    {
        if (scanCode == sc_LeftShift || scanCode == sc_RightShift)
            osd->flags &= ~OSD_SHIFT;

        if (scanCode == sc_LeftControl || scanCode == sc_RightControl)
            osd->flags &= ~OSD_CTRL;

        return 0;
    }

    osdedit_t &ed = osd->editor;
    osdkeytime    = gettime();

    switch (scanCode)
    {
    case sc_Escape:
        //        OSD_ShowDisplay(0);
        draw.scrolling = -1;
        osdrowscur--;
        OSD_CaptureInput(0);
        osdscrtime = timerGetTicks();
        break;

    case sc_PgUp:
        if (draw.head < osd->text.lines-1)
            ++draw.head;
        break;

    case sc_PgDn:
        if (draw.head > 0)
            --draw.head;
        break;

    case sc_Home:
        if (osd->flags & OSD_CTRL)
            draw.head = osd->text.lines-1;
        else
        {
            ed.pos   = 0;
            ed.start = ed.pos;
            ed.end   = ed.start + OSD_EDIT_LINE_WIDTH;
        }
        break;

    case sc_End:
        if (osd->flags & OSD_CTRL)
            draw.head = 0;
        else
        {
            ed.pos   = ed.len;
            ed.end   = ed.pos;
            ed.start = ed.end - OSD_EDIT_LINE_WIDTH;

            if (ed.start < 0)
            {
                ed.start = 0;
                ed.end   = OSD_EDIT_LINE_WIDTH;
            }
        }
        break;

    case sc_Insert:
        osd->flags = (osd->flags & ~OSD_OVERTYPE) | (-((osd->flags & OSD_OVERTYPE) == 0) & OSD_OVERTYPE);
        break;

    case sc_LeftArrow:
        if (ed.pos > 0)
        {
            if (osd->flags & OSD_CTRL)
            {
                while (ed.pos > 0)
                {
                    if (ed.buf[ed.pos-1] != asc_Space)
                        break;

                    --ed.pos;
                }

                while (ed.pos > 0)
                {
                    if (ed.buf[ed.pos-1] == asc_Space)
                        break;

                    --ed.pos;
                }
            }
            else --ed.pos;
        }

        if (ed.pos < ed.start)
        {
            ed.end -= (ed.start - ed.pos);
            ed.start -= (ed.start - ed.pos);
        }
        break;

    case sc_RightArrow:
        if (ed.pos < ed.len)
        {
            if (osd->flags & OSD_CTRL)
            {
                while (ed.pos < ed.len)
                {
                    if (ed.buf[ed.pos] == asc_Space)
                        break;

                    ed.pos++;
                }

                while (ed.pos < ed.len)
                {
                    if (ed.buf[ed.pos] != asc_Space)
                        break;

                    ed.pos++;
                }
            }
            else ed.pos++;
        }

        if (ed.pos >= ed.end)
        {
            ed.start += (ed.pos - ed.end);
            ed.end += (ed.pos - ed.end);
        }
        break;

    case sc_UpArrow:
        OSD_HistoryPrev();
        break;

    case sc_DownArrow:
        OSD_HistoryNext();
        break;

    case sc_LeftShift:
    case sc_RightShift:
        osd->flags |= OSD_SHIFT;
        break;

    case sc_LeftControl:
    case sc_RightControl:
        osd->flags |= OSD_CTRL;
        break;

    case sc_CapsLock:
        osd->flags = (osd->flags & ~OSD_CAPS) | (-((osd->flags & OSD_CAPS) == 0) & OSD_CAPS);
        break;

    case sc_Delete:
        if (ed.pos == ed.len || !ed.len)
            return 0;

        if (ed.pos <= ed.len-1)
            Bmemmove(ed.buf+ed.pos, ed.buf+ed.pos+1, ed.len-ed.pos-1);

        ed.len--;
        break;
    }

    return 0;
}


//
// OSD_ResizeDisplay() -- Handles readjustment of the display when the screen resolution
//  changes on us.
//
void OSD_ResizeDisplay(int w, int h)
{
    auto &t = osd->text;
    auto &d = osd->draw;

    int const newcols     = getcolumnwidth(w);
    int const newmaxlines = OSDBUFFERSIZE / newcols;

    auto newtext = (char *)Xmalloc(OSDBUFFERSIZE);
    auto newfmt  = (char *)Xmalloc(OSDBUFFERSIZE);

    Bmemset(newtext, asc_Space, OSDBUFFERSIZE);

    int const copylines = min(newmaxlines, t.maxlines);
    int const copycols  = min(newcols, d.cols);

    for (int i = 0; i < copylines; ++i)
    {
        Bmemcpy(newtext + newcols * i, t.buf + d.cols * i, copycols);
        Bmemcpy(newfmt  + newcols * i, t.fmt + d.cols * i, copycols);
    }

    Xfree(t.buf);
    t.buf = newtext;

    Xfree(t.fmt);
    t.fmt = newfmt;

    t.maxlines = newmaxlines;
    osdmaxrows = getrowheight(h) - 2;
    d.cols     = newcols;

    if (d.rows > osdmaxrows)
        d.rows = osdmaxrows;

    t.pos  = 0;
    d.head = 0;

    osd->editor.start = 0;
    osd->editor.end   = OSD_EDIT_LINE_WIDTH;

    whiteColorIdx = -1;
}


//
// OSD_CaptureInput()
//
void OSD_CaptureInput(int cap)
{
    osd->flags = (osd->flags & ~(OSD_CAPTURE|OSD_CTRL|OSD_SHIFT)) | (-cap & OSD_CAPTURE);

    mouseGrabInput(cap == 0 ? g_mouseLockedToWindow : 0);
    onshowosd(cap);

    inputState.keyFlushChars();
}


//
// OSD_ShowDisplay() -- Shows or hides the onscreen display
//
void OSD_ShowDisplay(int onf)
{
    osd->flags = (osd->flags & ~OSD_DRAW) | (-onf & OSD_DRAW);
    OSD_CaptureInput(onf);
}


//
// OSD_Draw() -- Draw the onscreen display
//

void OSD_Draw(void)
{
    if (!osd)
        return;

    if (osdrowscur == 0)
        OSD_ShowDisplay((osd->flags & OSD_DRAW) != OSD_DRAW);

    if (osdrowscur == osd->draw.rows)
        osd->draw.scrolling = 0;
    else
    {
        if ((osdrowscur < osd->draw.rows && osd->draw.scrolling == 1) || osdrowscur < -1)
        {
            uint32_t j = (timerGetTicks() - osdscrtime);

            while (j < UINT32_MAX)
            {
                j -= tabledivide32_noinline(200, osd->draw.rows);
                if (++osdrowscur > osd->draw.rows-1)
                    break;
            }
        }
        else if ((osdrowscur > -1 && osd->draw.scrolling == -1) || osdrowscur > osd->draw.rows)
        {
            uint32_t j = (timerGetTicks() - osdscrtime);

            while (j < UINT32_MAX)
            {
                j -= tabledivide32_noinline(200, osd->draw.rows);
                if (--osdrowscur < 1)
                    break;
            }
        }

        osdscrtime = timerGetTicks();
    }

    if ((osd->flags & OSD_DRAW) == 0 || !osdrowscur) return;

    int topoffs = osd->draw.head * osd->draw.cols;
    int row     = osdrowscur - 1;
    int lines   = min(osd->text.lines - osd->draw.head, osdrowscur);

    videoBeginDrawing();

    clearbackground(osd->draw.cols,osdrowscur+1);

    for (; lines>0; lines--, row--)
    {
        // XXX: May happen, which would ensue an oob if not checked.
        // Last char accessed is osd->text.buf[topoffs + osd->draw.cols-1].
        // Reproducible by running test.lua with -Lopts=diag
        // and scrolling to the top.
        if (topoffs + osd->draw.cols-1 >= OSDBUFFERSIZE)
            break;

        drawosdstr(0, row, osd->text.buf + topoffs, osd->draw.cols, osd->draw.textshade, osd->draw.textpal);
        topoffs += osd->draw.cols;
    }

    int       offset = ((osd->flags & (OSD_CAPS | OSD_SHIFT)) == (OSD_CAPS | OSD_SHIFT) && osd->draw.head > 0);
    int const shade  = osd->draw.promptshade ? osd->draw.promptshade : (sintable[((int32_t) totalclock<<4)&2047]>>11);

    if (osd->draw.head == osd->text.lines-1)
        drawosdchar(0, osdrowscur, '~', shade, osd->draw.promptpal);
    else if (osd->draw.head > 0)
        drawosdchar(0, osdrowscur, '^', shade, osd->draw.promptpal);

    if (osd->flags & OSD_CAPS)
        drawosdchar((osd->draw.head > 0), osdrowscur, 'C', shade, osd->draw.promptpal);

    if (osd->flags & OSD_SHIFT)
        drawosdchar(1 + (osd->flags & OSD_CAPS && osd->draw.head > 0), osdrowscur, 'H', shade, osd->draw.promptpal);

    drawosdchar(2 + offset, osdrowscur, '>', shade, osd->draw.promptpal);

    int const len = min(osd->draw.cols-1-3 - offset, osd->editor.len - osd->editor.start);

    for (int x=len-1; x>=0; x--)
        drawosdchar(3 + x + offset, osdrowscur, osd->editor.buf[osd->editor.start+x], osd->draw.editshade<<1, osd->draw.editpal);

    offset += 3 + osd->editor.pos - osd->editor.start;

    drawosdcursor(offset,osdrowscur,osd->flags & OSD_OVERTYPE,osdkeytime);

    if (osd->version.buf)
        drawosdstr(osd->draw.cols - osd->version.len, osdrowscur - (offset >= osd->draw.cols - osd->version.len),
                    osd->version.buf, osd->version.len, (sintable[((int32_t) totalclock<<4)&2047]>>11), osd->version.pal);

    videoEndDrawing();
}


//
// OSD_Printf() -- Print a formatted string to the onscreen display
//   and write it to the log file
//


//
// OSD_Puts() -- Print a string to the onscreen display
//   and write it to the log file
//

static inline void OSD_LineFeed(void)
{
    auto &t = osd->text;
    auto &d = osd->draw;

    Bmemmove(t.buf + d.cols, t.buf, OSDBUFFERSIZE - d.cols);
    Bmemset(t.buf, asc_Space, d.cols);

    Bmemmove(t.fmt + d.cols, t.fmt, OSDBUFFERSIZE - d.cols);
    Bmemset(t.fmt, d.textpal, d.cols);

    if (t.lines < t.maxlines)
        t.lines++;
}

#define MAX_ERRORS 4096

void OSD_Puts(const char *tmpstr)
{
    if (tmpstr[0] == 0 || osdlog == nullptr)
        return;

    if (!osd)
        OSD_Init();

    const char *chp;

    int textPal   = osd->draw.textpal;
    int textShade = osd->draw.textshade;

    mutex_lock(&osd->mutex);

    osdlog_t &log = osd->log;

    if (tmpstr[0]=='^' && tmpstr[1]=='1' && tmpstr[2]=='0' && ++log.errors > MAX_ERRORS)
    {
        if (log.errors == MAX_ERRORS + 1)
            tmpstr = "\nToo many errors. Logging errors stopped.\n";
        else
        {
            log.errors = MAX_ERRORS + 2;
            mutex_unlock(&osd->mutex);
            return;
        }
    }

    if (log.lines < log.cutoff)
    {
        char *chp2 = Xstrdup(tmpstr);
        buildvfs_fputs(OSD_StripColors(chp2, tmpstr), osdlog);
        Bprintf("%s", chp2);
        Xfree(chp2);
    }
    else if (log.lines == log.cutoff)
    {
        buildvfs_fputs("\nLog file full! Consider increasing \"osdlogcutoff\".\n", osdlog);
        log.lines = log.cutoff + 1;
    }

    chp = tmpstr;
    do
    {
        if (*chp == '\n')
        {
            osd->text.pos = 0;
            ++log.lines;
            OSD_LineFeed();
            continue;
        }

        if (*chp == '\r')
        {
            osd->text.pos = 0;
            continue;
        }

        if (*chp == '^')
        {
            if (isdigit(*(chp+1)))
            {
                char smallbuf[4];
                if (!isdigit(*(++chp+1)))
                {
                    smallbuf[0] = *(chp);
                    smallbuf[1] = '\0';
                    textPal = Batoi(smallbuf);
                    continue;
                }

                smallbuf[0] = *(chp++);
                smallbuf[1] = *(chp);
                smallbuf[2] = '\0';
                textPal = Batoi(smallbuf);
                continue;
            }

            if (Btoupper(*(chp+1)) == 'S')
            {
                chp++;
                if (isdigit(*(++chp)))
                    textShade = *chp;
                continue;
            }

            if (Btoupper(*(chp+1)) == 'O')
            {
                chp++;
                textPal   = osd->draw.textpal;
                textShade = osd->draw.textshade;
                continue;
            }
        }

        osd->text.buf[osd->text.pos] = *chp;
        osd->text.fmt[osd->text.pos++] = textPal+(textShade<<5);

        if (osd->text.pos == osd->draw.cols)
        {
            osd->text.pos = 0;
            OSD_LineFeed();
        }
    }
    while (*(++chp));

    mutex_unlock(&osd->mutex);
}


//
// OSD_DispatchQueued() -- Executes any commands queued in the buffer
//
void OSD_DispatchQueued(void)
{
	C_RunDelayedCommands();
}


#define MAXPARMS 256
void OSD_Dispatch(const char *cmd)
{
	AddCommandString(cmd);
}

//
// OSD_SetVersionString()
//
void OSD_SetVersion(const char *pszVersion, int osdShade, int osdPal)
{
    DO_FREE_AND_NULL(osd->version.buf);
    osd->version.buf   = Xstrdup(pszVersion);
    osd->version.len   = Bstrlen(pszVersion);
    osd->version.shade = osdShade;
    osd->version.pal   = osdPal;
}

