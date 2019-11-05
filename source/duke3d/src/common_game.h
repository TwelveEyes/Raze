//
// Definitions of common game-only data structures/functions
// (and declarations of data appearing in both)
// for EDuke32 and Mapster32
//

#ifndef EDUKE32_COMMON_GAME_H_
#define EDUKE32_COMMON_GAME_H_

#include "collections.h"
#include "gamecontrol.h"

#include "vfs.h"

BEGIN_DUKE_NS


#define DUKE                (g_gameType & GAMEFLAG_DUKE)
#define NAM                 (g_gameType & GAMEFLAG_NAM)
#define NAPALM              (g_gameType & GAMEFLAG_NAPALM)
#define WW2GI               (g_gameType & GAMEFLAG_WW2GI)
#define NAM_WW2GI           (g_gameType & (GAMEFLAG_NAM|GAMEFLAG_WW2GI))
#define SHAREWARE           (g_gameType & GAMEFLAG_SHAREWARE)
#define DUKEBETA            ((g_gameType & GAMEFLAG_DUKEBETA) == GAMEFLAG_DUKEBETA)
#define FURY                (g_gameType & GAMEFLAG_FURY)

enum Games_t {
    GAME_DUKE = 0,
    GAME_NAM,
    GAME_NAPALM,
    GAME_WW2GI,
    GAMECOUNT
};

typedef enum basepal_ {
    BASEPAL = 0,
    WATERPAL,
    SLIMEPAL,
    DREALMSPAL,
    TITLEPAL,
    ENDINGPAL,  // 5
    ANIMPAL,
    BASEPALCOUNT
} basepal_t;

#include "v_text.h"

extern int loaddefinitions_game(const char *fn, int32_t preload);

//////////

extern void G_InitMultiPsky(int CLOUDYOCEAN__DYN, int MOONSKY1__DYN, int BIGORBIT1__DYN, int LA__DYN);
extern void G_SetupGlobalPsky(void);

//////////

extern void G_LoadLookups(void);

//////////

END_DUKE_NS
#endif
