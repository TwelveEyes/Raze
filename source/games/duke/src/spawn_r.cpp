//-------------------------------------------------------------------------
/*
Copyright (C) 1996, 2003 - 3D Realms Entertainment
Copyright (C) 2017-2019 Nuke.YKT
Copyright (C) 2020 - Christoph Oelckers

This file is part of Duke Nukem 3D version 1.5 - Atomic Edition

Duke Nukem 3D is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Original Source: 1996 - Todd Replogle
Prepared for public release: 03/21/2003 - Charlie Wiederhold, 3D Realms
*/
//-------------------------------------------------------------------------

#include <utility>
#include "ns.h"
#include "global.h"
#include "sounds.h"
#include "names_r.h"
#include "dukeactor.h"

BEGIN_DUKE_NS

DDukeActor* spawninit_r(DDukeActor* actj, DDukeActor* act, TArray<DDukeActor*>* actors)
{
	auto sectp = act->sector();

	switch (act->spr.picnum)
	{
	default:
	default_case:
		spawninitdefault(actj, act);
		break;
	case BOWLINGPINSPOT:
	case RRTILE281:
	case BOWLINGBALLSPOT:
	case RRTILE283:
	case RRTILE2025:
	case RRTILE2026:
	case RRTILE2027:
	case RRTILE2028:
		act->spr.cstat = 0;
		act->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
		act->spr.scale = DVector2(0, 0);
		act->clipdist = 0;
		act->spr.extra = 0;
		ChangeActorStat(act, STAT_BOWLING);
		break;
	case RRTILE3410:
		act->spr.extra = 0;
		ChangeActorStat(act, 107);
		break;
	case RRTILE8450:
		if (!isRRRA()) goto default_case;
		act->spr.scale = DVector2(1, 1);
		act->spr.extra = act->spr.lotag;
		act->spr.cstat |= CSTAT_SPRITE_BLOCK_ALL;
		ChangeActorStat(act, 116);
		break;
	case PIG + 11:
		if (!isRRRA()) goto default_case;
		act->spr.scale = DVector2(0.25, 0.25);
		act->clipdist = 0;
		act->spr.extra = 0;
		act->spr.cstat = 0;
		ChangeActorStat(act, 121);
		break;
	case RRTILE8487:
	case RRTILE8489:
		if (!isRRRA()) goto default_case;
		act->spr.scale = DVector2(0.5, 0.5);
		act->spr.extra = 0;
		act->spr.cstat |= CSTAT_SPRITE_BLOCK_ALL;
		act->spr.hitag = 0;
		ChangeActorStat(act, 117);
		break;
	case RRTILE7424:
		if (!isRRRA()) goto default_case;
		act->spr.extra = 0;
		act->spr.scale = DVector2(0, 0);
		ChangeActorStat(act, STAT_FX);
		break;
	case RRTILE7936:
		if (!isRRRA()) goto default_case;
		act->spr.scale = DVector2(0, 0);
		fogactive = 1;
		break;
	case RRTILE6144:
		if (!isRRRA()) goto default_case;
		act->spr.scale = DVector2(0, 0);
		ps[screenpeek].sea_sick_stat = 1;
		break;
	case RRTILE8448:
		if (!isRRRA()) goto default_case;
		act->spr.lotag = 1;
		act->clipdist = 0;
		break;
	case RRTILE8099:
		if (!isRRRA()) goto default_case;
		act->spr.lotag = 5;
		act->clipdist = 0;
		ChangeActorStat(act, 123);
		break;
	case RRTILE8704:
		if (!isRRRA()) goto default_case;
		act->spr.lotag = 1;
		act->clipdist = 0;
		break;
	case RRTILE8192:
		if (!isRRRA()) goto default_case;
		act->spr.scale = DVector2(0, 0);
		ufospawnsminion = 1;
		break;
	case RRTILE8193:
		if (!isRRRA()) goto default_case;
		act->spr.scale = DVector2(0, 0);
		pistonsound = 1;
		break;
	case RRTILE8165:
		if (!isRRRA()) goto default_case;
		act->spr.lotag = 1;
		act->clipdist = 0;
		act->SetOwner(act);
		act->spr.extra = 0;
		ChangeActorStat(act, 115);
		break;
	case RRTILE8593:
		if (!isRRRA()) goto default_case;
		act->spr.lotag = 1;
		act->clipdist = 0;
		act->SetOwner(act);
		act->spr.extra = 0;
		ChangeActorStat(act, 122);
		break;
	case RRTILE285:
	case RRTILE286:
	case RRTILE287:
	case RRTILE288:
	case RRTILE289:
	case RRTILE290:
	case RRTILE291:
	case RRTILE292:
	case RRTILE293:
		act->spr.cstat = 0;
		act->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
		act->spr.scale = DVector2(0, 0);
		act->clipdist = 0;
		act->spr.lotag = 0;
		ChangeActorStat(act, 106);
		break;

	case RRTILE2460:
	case RRTILE2465:
	case BIKEJIBA:
	case BIKEJIBB:
	case BIKEJIBC:
	case BIKERJIBA:
	case BIKERJIBB:
	case BIKERJIBC:
	case BIKERJIBD:
	case CHEERJIBA:
	case CHEERJIBB:
	case CHEERJIBC:
	case CHEERJIBD:
	case FBOATJIBA:
	case FBOATJIBB:
	case RABBITJIBA:
	case RABBITJIBB:
	case RABBITJIBC:
	case MAMAJIBA:
	case MAMAJIBB:
		// impossible to do with better methods outside of redoing the entire switch/case block
		if (isRRRA()) goto rrra_badguy;
		else goto default_case;

	case WATERSPLASH2:
	case MUD:
		if (actj)
		{
			SetActor(act, actj->spr.pos);
			double s = 0.125 + (krand() & 7) * REPEAT_SCALE;
			act->spr.scale = DVector2(s, s);
		}
		else
		{
			double s = 0.25 + (krand() & 15) * REPEAT_SCALE;
			act->spr.scale = DVector2(s, s);
		}

		act->spr.shade = -16;
		act->spr.cstat |= CSTAT_SPRITE_YCENTER;
		if (actj)
		{
			if (actj->sector()->lotag == 2)
			{
				act->spr.pos.Z = getceilzofslopeptr(act->sector(), act->spr.pos) + 16;
				act->spr.cstat |= CSTAT_SPRITE_YFLIP;
			}
			else if (actj->sector()->lotag == 1)
				act->spr.pos.Z = getceilzofslopeptr(act->sector(), act->spr.pos);
		}

		if (sectp->floorpicnum == FLOORSLIME ||
			sectp->ceilingpicnum == FLOORSLIME)
			act->spr.pal = 7;
		[[fallthrough]];
	case NEON1:
	case NEON2:
	case NEON3:
	case NEON4:
	case NEON5:
	case NEON6:
	case DOMELITE:
		if (act->spr.picnum != WATERSPLASH2)
			act->spr.cstat |= CSTAT_SPRITE_BLOCK_ALL;
		if (act->spr.picnum == DOMELITE)
			act->spr.cstat |= CSTAT_SPRITE_BLOCK_ALL;
		[[fallthrough]];
	case JIBS1:
	case JIBS2:
	case JIBS3:
	case JIBS4:
	case JIBS5:
	case JIBS6:
	case DUKETORSO:
	case DUKEGUN:
	case DUKELEG:
	case BILLYJIBA:
	case BILLYJIBB:
	case HULKJIBA:
	case HULKJIBB:
	case HULKJIBC:
	case MINJIBA:
	case MINJIBB:
	case MINJIBC:
	case COOTJIBA:
	case COOTJIBB:
	case COOTJIBC:
	rrra_badguy:
		if (act->spr.picnum == JIBS6)
		{
			act->spr.scale *= 0.5;
		}
		else if (isRRRA())
		{
			if (act->spr.picnum == RABBITJIBA)
			{
				act->spr.scale = DVector2(0.28125, 0.28125);
			}
			else if (act->spr.picnum == RABBITJIBB)
			{
				act->spr.scale = DVector2(0.5625, 0.5625);
			}
			else if (act->spr.picnum == RABBITJIBC)
			{
				act->spr.scale = DVector2(0.84375, 0.84375);
			}
		}
		ChangeActorStat(act, STAT_MISC);
		break;
	case TONGUE:
		if (actj)
			act->spr.angle = actj->spr.angle;
		act->spr.pos.Z -= gs.playerheight;
		act->vel.Z = 1 - krandf(2);
		act->vel.X = 4 - krandf(8);
		ChangeActorStat(act, STAT_PROJECTILE);
		break;
	case TRANSPORTERSTAR:
	case TRANSPORTERBEAM:
		spawntransporter(actj, act, act->spr.picnum == TRANSPORTERBEAM);
		break;

	case FRAMEEFFECT1:
		if (actj)
		{
			act->spr.scale = actj->spr.scale;
			if (actj->spr.picnum == APLAYER)
				act->temp_data[1] = SMALLSMOKE;
			else
				act->temp_data[1] = actj->spr.picnum;
		}
		else act->spr.scale = DVector2(0, 0);

		ChangeActorStat(act, STAT_MISC);
		break;

	case FORCESPHERE:
		if (!actj)
		{
			act->spr.cstat = CSTAT_SPRITE_INVISIBLE;
			ChangeActorStat(act, STAT_ZOMBIEACTOR);
		}
		else
		{
			act->spr.scale = DVector2(REPEAT_SCALE, REPEAT_SCALE);
			ChangeActorStat(act, STAT_MISC);
		}
		break;

	case BLOOD:
		act->spr.scale = DVector2(0.0625, 0.0625);
		act->spr.pos.Z -= 26;
		ChangeActorStat(act, STAT_MISC);
		break;
	case BLOODPOOL:
		if (spawnbloodpoolpart1(act)) break;

		if (actj)
		{
			if (actj->spr.pal == 1)
				act->spr.pal = 1;
			else if (actj->spr.pal != 6 && actj->spr.picnum != NUKEBARREL && actj->spr.picnum != TIRE)
			{
				act->spr.pal = 2; // Red
			}
			else act->spr.pal = 0;  // green

			if (actj->spr.picnum == TIRE)
				act->spr.shade = 127;
		}
		act->spr.cstat |= CSTAT_SPRITE_ALIGNMENT_FLOOR;
		[[fallthrough]];

	case BLOODSPLAT1:
	case BLOODSPLAT2:
	case BLOODSPLAT3:
	case BLOODSPLAT4:
		act->spr.cstat |= CSTAT_SPRITE_ALIGNMENT_WALL;
		act->spr.scale.X = (0.109375 + (krand() & 7) * REPEAT_SCALE);
		act->spr.scale.Y = (0.109375 + (krand() & 7) * REPEAT_SCALE);
		act->spr.pos.Z -= 16;
		if (actj && actj->spr.pal == 6)
			act->spr.pal = 6;
		insertspriteq(act);
		ChangeActorStat(act, STAT_MISC);
		break;

	case HYDRENT:
	case SATELITE:
	case FUELPOD:
	case SOLARPANNEL:
	case ANTENNA:
	case GRATE1:
	case CHAIR1:
	case CHAIR2:
	case CHAIR3:
	case BOTTLE1:
	case BOTTLE2:
	case BOTTLE3:
	case BOTTLE4:
	case BOTTLE5:
	case BOTTLE6:
	case BOTTLE7:
	case BOTTLE8:
	case BOTTLE10:
	case BOTTLE11:
	case BOTTLE12:
	case BOTTLE13:
	case BOTTLE14:
	case BOTTLE15:
	case BOTTLE16:
	case BOTTLE17:
	case BOTTLE18:
	case BOTTLE19:
	case SCALE:
	case VACUUM:
	case FANSPRITE:
	case CACTUS:
	case CACTUSBROKE:
	case CAMERALIGHT:
	case MOVIECAMERA:
	case IVUNIT:
	case POT1:
	case POT2:
	case POT3:
	case SUSHIPLATE1:
	case SUSHIPLATE2:
	case SUSHIPLATE3:
	case SUSHIPLATE4:
	case SUSHIPLATE5:
	case WAITTOBESEATED:
	case VASE:
	case PIPE1:
	case PIPE2:
	case PIPE3:
	case PIPE4:
	case PIPE5:
	case PIPE6:
		act->clipdist = 8;
		act->spr.cstat |= CSTAT_SPRITE_BLOCK_ALL;
		ChangeActorStat(act, 0);
		break;
	case FEMMAG1:
	case FEMMAG2:
		act->spr.cstat &= ~CSTAT_SPRITE_BLOCK_ALL;
		ChangeActorStat(act, 0);
		break;

	case MASKWALL7:
	{
		auto j = act->spr.cstat & (CSTAT_SPRITE_ALIGNMENT_MASK | CSTAT_SPRITE_XFLIP | CSTAT_SPRITE_YFLIP);
		act->spr.cstat = j | CSTAT_SPRITE_BLOCK;
		ChangeActorStat(act, 0);
		break;
	}
	case FOOTPRINTS:
	case FOOTPRINTS2:
	case FOOTPRINTS3:
	case FOOTPRINTS4:
		initfootprint(actj, act);
		break;
	case FEM10:
	case NAKED1:
	case STATUE:
	case TOUGHGAL:
		act->spr.yint = act->spr.hitag;
		act->spr.hitag = -1;
		[[fallthrough]];
	case QUEBALL:
	case STRIPEBALL:
		if (act->spr.picnum == QUEBALL || act->spr.picnum == STRIPEBALL)
		{
			act->spr.cstat = CSTAT_SPRITE_BLOCK_HITSCAN;
			act->clipdist = 2;
		}
		else
		{
			act->spr.cstat |= CSTAT_SPRITE_BLOCK_ALL;
			act->clipdist = 8;
		}
		ChangeActorStat(act, STAT_ZOMBIEACTOR);
		break;
	case BOWLINGBALL:
		act->spr.cstat = CSTAT_SPRITE_BLOCK_HITSCAN;
		act->clipdist = 16;
		act->spr.scale = DVector2(0.171875, 0.140625);
		ChangeActorStat(act, STAT_ZOMBIEACTOR);
		break;
	case HENSTAND:
		act->spr.cstat = CSTAT_SPRITE_BLOCK_ALL;
		act->clipdist = 12;
		act->spr.scale = DVector2(0.328125, 0.234375);
		ChangeActorStat(act, STAT_ZOMBIEACTOR);
		break;
	case RRTILE295:
		act->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
		ChangeActorStat(act, 107);
		break;
	case RRTILE296:
	case RRTILE297:
		act->spr.scale = DVector2(1, 1);
		act->clipdist = 16;
		ChangeActorStat(act, 108);
		break;
	case RRTILE3190:
	case RRTILE3191:
	case RRTILE3192:
		act->spr.cstat = CSTAT_SPRITE_BLOCK_ALL;
		act->clipdist = 2;
		act->spr.scale = DVector2(0.5, 0.40625);
		act->vel.X = 2;
		ChangeActorStat(act, STAT_ACTOR);
		break;
	case RRTILE3120:
		act->spr.cstat = CSTAT_SPRITE_BLOCK_ALL;
		act->clipdist = 2;
		act->spr.scale = DVector2(0.1875, 0.15625);
		act->vel.X = 2;
		ChangeActorStat(act, STAT_ACTOR);
		break;
	case RRTILE3122:
		act->spr.cstat = CSTAT_SPRITE_BLOCK_ALL;
		act->clipdist = 0.5;
		act->spr.scale = DVector2(0.125, 0.09375);
		act->vel.X = 1;
		ChangeActorStat(act, STAT_ACTOR);
		break;
	case RRTILE3123:
		act->spr.cstat = CSTAT_SPRITE_BLOCK_ALL;
		act->clipdist = 2;
		act->spr.scale = DVector2(0.203125, 0.203125);
		act->vel.X = 1;
		ChangeActorStat(act, STAT_ACTOR);
		break;
	case RRTILE3124:
		act->spr.cstat = CSTAT_SPRITE_BLOCK_ALL;
		act->clipdist = 2;
		act->spr.scale = DVector2(0.265625, 0.1875);
		act->vel.X = 2;
		ChangeActorStat(act, STAT_ACTOR);
		break;
	case RRTILE3132:
		act->spr.cstat = CSTAT_SPRITE_BLOCK_ALL;
		act->clipdist = 2;
		act->spr.scale = DVector2(0.203125, 0.15625);
		act->vel.X = 0;
		ChangeActorStat(act, STAT_ACTOR);
		break;
	case BOWLINGPIN:
		act->spr.cstat = CSTAT_SPRITE_BLOCK_ALL;
		act->clipdist = 12;
		act->spr.scale = DVector2(0.359375, 0.359375);
		ChangeActorStat(act, STAT_ZOMBIEACTOR);
		break;
	case DUKELYINGDEAD:
		if (actj && actj->spr.picnum == APLAYER)
		{
			act->spr.scale = actj->spr.scale;
			act->spr.shade = actj->spr.shade;
			act->spr.pal = ps[actj->PlayerIndex()].palookup;
		}
		act->spr.cstat = 0;
		act->spr.extra = 1;
		act->vel.X = 292 / 16.;
		act->vel.Z = 360 / 256.;
		[[fallthrough]];
	case RESPAWNMARKERRED:
		if (act->spr.picnum == RESPAWNMARKERRED)
		{
			act->spr.scale = DVector2(0.125, 0.125);
			if (actj) act->spr.pos.Z = actj->floorz;
		}
		else
		{
			act->spr.cstat |= CSTAT_SPRITE_BLOCK_ALL;
			act->clipdist = 32;
		}
		[[fallthrough]];
	case MIKE:
		if (act->spr.picnum == MIKE)
			act->spr.yint = act->spr.hitag;
		ChangeActorStat(act, STAT_ACTOR);
		break;

	case SPOTLITE:
		break;
	case BULLETHOLE:
		act->spr.scale = DVector2(0.046875, 0.046875);
		act->spr.cstat = CSTAT_SPRITE_ALIGNMENT_WALL | randomFlip();
		insertspriteq(act);
		[[fallthrough]];
	case MONEY:
		if (act->spr.picnum == MONEY)
		{
			act->temp_data[0] = krand() & 2047;
			act->spr.cstat = randomFlip();
			act->spr.scale = DVector2(0.125, 0.125);
			act->spr.angle = randomAngle();
		}
		ChangeActorStat(act, STAT_MISC);
		break;

	case SHELL: //From the player
	case SHOTGUNSHELL:
		initshell(actj, act, act->spr.picnum == SHELL);
		break;
	case RESPAWN:
		act->spr.extra = 66 - 13;
		[[fallthrough]];
	case MUSICANDSFX:
		if (ud.multimode < 2 && act->spr.pal == 1)
		{
			act->spr.scale = DVector2(0, 0);
			ChangeActorStat(act, STAT_MISC);
			break;
		}
		act->spr.cstat = CSTAT_SPRITE_INVISIBLE;
		ChangeActorStat(act, STAT_FX);
		break;
	case SOUNDFX:
	{
		act->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
		ChangeActorStat(act, STAT_ZOMBIEACTOR);
	}
	break;
	case EXPLOSION2:
	case EXPLOSION3:
	case BURNING:
	case SMALLSMOKE:
		if (actj)
		{
			act->spr.angle = actj->spr.angle;
			act->spr.shade = -64;
			act->spr.cstat = CSTAT_SPRITE_YCENTER | randomXFlip();
		}

		if (act->spr.picnum == EXPLOSION2)
		{
			act->spr.scale = DVector2(0.75, 0.75);
			act->spr.shade = -127;
			act->spr.cstat |= CSTAT_SPRITE_YCENTER;
		}
		else if (act->spr.picnum == EXPLOSION3)
		{
			act->spr.scale = DVector2(2, 2);
			act->spr.shade = -127;
			act->spr.cstat |= CSTAT_SPRITE_YCENTER;
		}
		else if (act->spr.picnum == SMALLSMOKE)
		{
			act->spr.scale = DVector2(0.1875, 0.1875);
		}
		else if (act->spr.picnum == BURNING)
		{
			act->spr.scale = DVector2(0.0625, 0.0625);
		}

		if (actj)
		{
			double x = getflorzofslopeptr(act->sector(), act->spr.pos);
			if (act->spr.pos.Z > x - 12)
				act->spr.pos.Z = x - 12;
		}

		ChangeActorStat(act, STAT_MISC);

		break;

	case PLAYERONWATER:
		if (actj)
		{
			act->spr.scale = actj->spr.scale;
			act->vel.Z = 0.5;
			if (act->sector()->lotag != 2)
				act->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
		}
		ChangeActorStat(act, STAT_DUMMYPLAYER);
		break;

	case APLAYER:
	{
		act->spr.scale = DVector2(0, 0);
		int j = ud.coop;
		if (j == 2) j = 0;

		if (ud.multimode < 2 || (ud.multimode > 1 && j != act->spr.lotag))
			ChangeActorStat(act, STAT_MISC);
		else
			ChangeActorStat(act, STAT_PLAYER);
		break;
	}
	case WATERBUBBLE:
		if (actj && actj->spr.picnum == APLAYER)
			act->spr.pos.Z -= 16;
		if (act->spr.picnum == WATERBUBBLE)
		{
			if (actj)
				act->spr.angle = actj->spr.angle;
			double s = REPEAT_SCALE + (krand() & 7) * REPEAT_SCALE;
			act->spr.scale = DVector2(s, s);
		}
		else
			act->spr.scale = DVector2(0.5, 0.5);
		ChangeActorStat(act, STAT_MISC);
		break;
	case CRANE:
		initcrane(actj, act, CRANEPOLE);
		break;
	case WATERDRIP:
		initwaterdrip(actj, act);
		break;
	case TRASH:

		if (act->spr.picnum != WATERDRIP) act->spr.angle = randomAngle();

		act->spr.scale = DVector2(0.375, 0.375);
		ChangeActorStat(act, STAT_STANDABLE);
		break;

	case PLUG:
		act->spr.lotag = 9999;
		ChangeActorStat(act, STAT_STANDABLE);
		break;
	case TOUCHPLATE:
		act->temp_pos.Z = sectp->floorz;
		if (sectp->lotag != 1 && sectp->lotag != 2)
			sectp->setfloorz(act->spr.pos.Z);
		if (act->spr.pal && ud.multimode > 1)
		{
			act->spr.scale = DVector2(0, 0);
			ChangeActorStat(act, STAT_MISC);
			break;
		}
		[[fallthrough]];
	case WATERBUBBLEMAKER:
		act->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
		ChangeActorStat(act, STAT_STANDABLE);
		break;
	case BOLT1:
	case BOLT1 + 1:
	case BOLT1 + 2:
	case BOLT1 + 3:
		act->temp_pos.X = act->spr.scale.X;
		act->temp_pos.Y = act->spr.scale.Y;
		[[fallthrough]];
	case MASTERSWITCH:
		if (act->spr.picnum == MASTERSWITCH)
			act->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
		act->spr.yint = 0;
		ChangeActorStat(act, STAT_STANDABLE);
		break;

		// this is not really nice...
	case BIKERB:
	case BIKERBV2:
	case BIKER:
	case MAKEOUT:
	case CHEERB:
	case CHEER:
	case COOTPLAY:
	case BILLYPLAY:
	case MINIONBOAT:
	case HULKBOAT:
	case CHEERBOAT:
	case RABBIT:
	case ROCK:
	case ROCK2:
	case MAMACLOUD:
	case MAMA:
	case UFO1_RRRA:
		if (isRRRA()) goto rrra_badguy2;
		else goto default_case;

	case UFO1_RR:
		if (!isRRRA()) goto rrra_badguy2;
		else goto default_case;

	case SBSWIPE:
	case CHEERSTAYPUT:
		if (isRRRA()) goto rrra_stayput;
		else goto default_case;
	case SBMOVE:
		if (isRRRA()) goto default_case;
		[[fallthrough]];

	case BILLYRAYSTAYPUT:
	case BRAYSNIPER:
	case BUBBASTAND:
	case HULKSTAYPUT:
	case HENSTAYPUT:
	case PIGSTAYPUT:
	case MINIONSTAYPUT:
	case COOTSTAYPUT:
	rrra_stayput:
		act->actorstayput = act->sector();
		[[fallthrough]];
	case BOULDER:
	case BOULDER1:
	case RAT:
	case TORNADO:
	case BILLYCOCK:
	case BILLYRAY:
	case DOGRUN:
	case LTH:
	case HULK:
	case HEN:
	case DRONE:
	case PIG:
	case MINION:
	case UFO2:
	case UFO3:
	case UFO4:
	case UFO5:
	case COW:
	case COOT:
	case SHARK:
	case VIXEN:
	rrra_badguy2:
		act->spr.scale = DVector2(0.625, 0.625);
		// Note: All inappropriate tiles have already been weeded out by the outer switch block so this does not need game type checks anymore.
		switch (act->spr.picnum)
		{
		case VIXEN:
			if (act->spr.pal == 34)
			{
				act->spr.scale = DVector2(0.34375, 0.328125);
			}
			else
			{
				act->spr.scale = DVector2(0.34375, 0.3125);
			}
			act->setClipDistFromTile();
			break;
		case HULKHANG:
		case HULKHANGDEAD:
		case HULKJUMP:
		case HULK:
		case HULKSTAYPUT:
			act->spr.scale = DVector2(0.5, 0.5);
			act->setClipDistFromTile();
			break;
		case COOTPLAY:
		case COOT:
		case COOTSTAYPUT:
			act->spr.scale = DVector2(0.375, 0.28125);
			act->setClipDistFromTile();
			act->clipdist *= 4;
			break;
		case DRONE:
			act->spr.scale = DVector2(0.21875, 0.109375);
			act->clipdist = 32;
			break;
		case SBSWIPE:
		case BILLYPLAY:
		case BILLYCOCK:
		case BILLYRAY:
		case BILLYRAYSTAYPUT:
		case BRAYSNIPER:
		case BUBBASTAND:
			act->spr.scale = DVector2(0.390625, 0.328125);
			act->setClipDistFromTile();
			break;
		case COW:
			act->spr.scale = DVector2(0.5, 0.5);
			act->setClipDistFromTile();
			break;
		case HEN:
		case HENSTAYPUT:
		case HENSTAND:
			if (act->spr.pal == 35)
			{
				act->spr.scale = DVector2(0.65625, 0.46875);
				act->setClipDistFromTile();
			}
			else
			{
				act->spr.scale = DVector2(0.328125, 0.234375);
				act->clipdist = 16;
			}
			break;
		case MINION:
		case MINIONSTAYPUT:
			act->spr.scale = DVector2(0.25, 0.25);
			act->setClipDistFromTile();
			if (isRRRA() && ufospawnsminion)
				act->spr.pal = 8;
			break;
		case DOGRUN:
		case PIG:
			act->spr.scale = DVector2(0.25, 0.25);
			act->setClipDistFromTile();
			break;
		case RABBIT:
			act->spr.scale = DVector2(0.28125, 0.28125);
			act->setClipDistFromTile();
			break;
		case MAMACLOUD:
			act->spr.scale = DVector2(1, 1);
			act->spr.cstat = CSTAT_SPRITE_TRANSLUCENT;
			act->spr.cstat |= CSTAT_SPRITE_TRANS_FLIP;
			act->spr.pos.X += krandf(128) - 64;
			act->spr.pos.Y += krandf(128) - 64;
			act->spr.pos.Z += krandf(8) - 4;
			break;
		case MAMA:
			if (act->spr.pal == 30)
			{
				act->spr.scale = DVector2(0.40625, 0.40625);
				act->clipdist = 18.75;
			}
			else if (act->spr.pal == 31)
			{
				act->spr.scale = DVector2(0.5625, 0.5625);
				act->clipdist = 25;
			}
			else if (act->spr.pal == 32)
			{
				act->spr.scale = DVector2(0.78125, 0.78125);
				act->clipdist = 25;
			}
			else
			{
				act->spr.scale = DVector2(0.78125, 0.78125);
				act->clipdist = 25;
			}
			break;
		case BIKERB:
			act->spr.scale = DVector2(0.4375, 0.34375);
			act->clipdist = 18;
			break;
		case BIKERBV2:
			act->spr.scale = DVector2(0.4375, 0.34375);
			act->clipdist = 18;
			break;
		case BIKER:
			act->spr.scale = DVector2(0.4375, 0.34375);
			act->setClipDistFromTile();
			break;
		case CHEERB:
			act->spr.scale = DVector2(0.4375, 0.34375);
			act->clipdist = 18;
			break;
		case CHEER:
		case CHEERSTAYPUT:
			act->spr.scale = DVector2(0.34375, 0.3125);
			act->setClipDistFromTile();
			break;
		case MAKEOUT:
			act->spr.scale = DVector2(0.40625, 0.40625);
			act->setClipDistFromTile();
			break;
		case MINIONBOAT:
			act->spr.scale = DVector2(0.25, 0.25);
			act->setClipDistFromTile();
			break;
		case HULKBOAT:
			act->spr.scale = DVector2(0.75, 0.75);
			act->setClipDistFromTile();
			break;
		case CHEERBOAT:
			act->spr.scale = DVector2(0.5, 0.5);
			act->setClipDistFromTile();
			break;

		case TORNADO:
			act->spr.scale = DVector2(1, 2);
			act->setClipDistFromTile();
			act->clipdist *= 0.25;
			act->spr.cstat = CSTAT_SPRITE_TRANSLUCENT;
			break;
		case LTH:
			act->spr.scale = DVector2(0.375, 0.34375);
			act->setClipDistFromTile();
			break;
		case ROCK:
		case ROCK2:
			act->spr.scale = DVector2(1, 1);
			act->setClipDistFromTile();
			break;

		case UFO1_RRRA:
		case UFO1_RR:
		case UFO2:
		case UFO3:
		case UFO4:
		case UFO5:
			act->spr.scale = DVector2(0.5, 0.5);
			act->setClipDistFromTile();
			act->spr.extra = 50;
			break;
		case SBMOVE:
			act->spr.scale = DVector2(0.75, 0.75);
			act->setClipDistFromTile();
			break;

		default:
			break;
		}

		if (actj) act->spr.lotag = 0;

		if ((act->spr.lotag > ud.player_skill) || ud.monsters_off == 1)
		{
			act->spr.scale = DVector2(0, 0);
			ChangeActorStat(act, STAT_MISC);
			break;
		}
		else
		{
			makeitfall(act);

			if (act->spr.picnum == RAT)
			{
				act->spr.angle = randomAngle();
				act->spr.scale = DVector2(0.75, 0.75);
				act->spr.cstat = 0;
			}
			else
			{
				act->spr.cstat |= CSTAT_SPRITE_BLOCK_ALL;

				if (act->spr.picnum != 5501)
					if (actorfella(act))
						ps[myconnectindex].max_actors_killed++;
			}

			if (actj)
			{
				act->timetosleep = 0;
				check_fta_sounds_r(act);
				ChangeActorStat(act, STAT_ACTOR);
				act->spr.shade = actj->spr.shade;
			}
			else ChangeActorStat(act, STAT_ZOMBIEACTOR);

		}

		break;
	case LOCATORS:
		act->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
		ChangeActorStat(act, STAT_LOCATOR);
		break;

	case ACTIVATORLOCKED:
	case ACTIVATOR:
		act->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
		if (act->spr.picnum == ACTIVATORLOCKED)
			sectp->lotag ^= 16384;
		ChangeActorStat(act, STAT_ACTIVATOR);
		break;
	case DOORSHOCK:
		act->spr.cstat |= CSTAT_SPRITE_BLOCK_ALL;
		act->spr.shade = -12;

		ChangeActorStat(act, STAT_STANDABLE);
		break;

	case OOZ:
	{
		act->spr.shade = -12;

		if (actj)
			if (actj->spr.picnum == NUKEBARREL)
				act->spr.pal = 8;

		ChangeActorStat(act, STAT_STANDABLE);

		getglobalz(act);

		double j = ((act->floorz - act->ceilingz) / 128.);

		act->spr.scale = DVector2(max(0., 0.390625 - j * 0.5), j);
		if(krand() & 4) act->spr.cstat |= CSTAT_SPRITE_XFLIP;
		break;
	}
	case HEAVYHBOMB:
		act->SetOwner(act);
		act->spr.scale = DVector2(0.140625, 0.140625);
		act->spr.yint = 4;
		[[fallthrough]];
	case REACTOR2:
	case REACTOR:
	case RECON:
		if (initreactor(actj, act, act->spr.picnum == RECON)) return act;
		break;

	case RPG2SPRITE:
	case MOTOAMMO:
	case BOATAMMO:
		if (!isRRRA()) goto default_case;
		[[fallthrough]];

	case ATOMICHEALTH:
	case STEROIDS:
	case HEATSENSOR:
	case SHIELD:
	case AIRTANK:
	case POWDERKEG:
	case COWPIE:
	case HOLODUKE:

	case FIRSTGUNSPRITE:
	case CHAINGUNSPRITE:
	case SHOTGUNSPRITE:
	case RPGSPRITE:
	case SHRINKERSPRITE:
	case FREEZESPRITE:
	case DEVISTATORSPRITE:

	case SHOTGUNAMMO:
	case FREEZEAMMO:
	case HBOMBAMMO:
	case CRYSTALAMMO:
	case GROWAMMO:
	case BATTERYAMMO:
	case DEVISTATORAMMO:
	case RPGAMMO:
	case BOOTS:
	case AMMO:
	case AMMOLOTS:
	case BEER:
	case FIRSTAID:
	case SIXPAK:

	case SAWAMMO:
	case BOWLINGBALLSPRITE:
		if (actj)
		{
			act->spr.lotag = 0;
			if (act->spr.picnum != BOWLINGBALLSPRITE)
			{
				act->spr.pos.Z -= 32;
				act->vel.Z = -4;
			}
			else
			{
				act->vel.Z = 0;
			}
			ssp(act, CLIPMASK0);
			act->spr.cstat = randomXFlip();
		}
		else
		{
			act->SetOwner(act);
			act->spr.cstat = 0;
		}

		if ((ud.multimode < 2 && act->spr.pal != 0) || (act->spr.lotag > ud.player_skill))
		{
			act->spr.scale = DVector2(0, 0);
			ChangeActorStat(act, STAT_MISC);
			break;
		}

		act->spr.pal = 0;
		[[fallthrough]];

	case ACCESSCARD:

		if (act->spr.picnum == ATOMICHEALTH)
			act->spr.cstat |= CSTAT_SPRITE_YCENTER;

		if (ud.multimode > 1 && ud.coop != 1 && act->spr.picnum == ACCESSCARD)
		{
			act->spr.scale = DVector2(0, 0);
			ChangeActorStat(act, STAT_MISC);
			break;
		}
		else
		{
			if (act->spr.picnum == AMMO)
				act->spr.scale = DVector2(0.25, 0.25);
			else act->spr.scale = DVector2(0.5, 0.5);
		}

		act->spr.shade = -17;

		if (actj) ChangeActorStat(act, STAT_ACTOR);
		else
		{
			ChangeActorStat(act, STAT_ZOMBIEACTOR);
			makeitfall(act);
		}
		switch (act->spr.picnum)
		{
		case FIRSTGUNSPRITE:
			act->spr.scale = DVector2(0.25, 0.25);
			break;
		case SHOTGUNAMMO:
			act->spr.scale = DVector2(0.28125, 0.265625);
			if (isRRRA()) act->spr.cstat = CSTAT_SPRITE_BLOCK_HITSCAN;
			break;
		case SIXPAK:
			act->spr.scale = DVector2(0.203125, 0.140625);
			if (isRRRA()) act->spr.cstat = CSTAT_SPRITE_BLOCK_HITSCAN;
			break;
		case FIRSTAID:
			act->spr.scale = DVector2(0.125, 0.125);
			break;
		case BEER:
			act->spr.scale = DVector2(0.078125, 0.0625);
			break;
		case AMMO:
			act->spr.scale = DVector2(0.140625, 0.140625);
			break;
		case MOTOAMMO:
			if (!isRRRA()) goto default_case;
			act->spr.scale = DVector2(0.359375, 0.359375);
			break;
		case BOATAMMO:
			if (!isRRRA()) goto default_case;
			act->spr.scale = DVector2(0.25, 0.25);
			break;
		case COWPIE:
			act->spr.scale = DVector2(0.125, 0.09375);
			break;
		case STEROIDS:
			act->spr.scale = DVector2(0.203125, 0.140625);
			break;
		case ACCESSCARD:
			act->spr.scale = DVector2(0.171875, 0.1875);
			break;
		case HEATSENSOR:
			act->spr.scale = DVector2(0.09375, 0.0625);
			break;
		case AIRTANK:
			act->spr.scale = DVector2(0.296875, 0.25);
			break;
		case BATTERYAMMO:
			act->spr.scale = DVector2(0.234375, 0.234375);
			break;
		case BOWLINGBALLSPRITE:
			act->spr.scale = DVector2(0.171875, 0.171875);
			break;
		case POWDERKEG:
			act->spr.scale = DVector2(0.171875, 0.171875);
			act->spr.yint = 4;
			act->vel.X = 2;
			break;
		case RPGSPRITE:
			act->spr.scale = DVector2(0.25, 0.21875);
			break;
		case RPG2SPRITE:
			if (!isRRRA()) goto default_case;
			act->spr.scale = DVector2(0.34375, 0.3125);
			break;
		case SHRINKERSPRITE:
			act->spr.scale = DVector2(0.34375, 0.203125);
			break;
		case DEVISTATORSPRITE:
			act->spr.scale = DVector2(0.28125, 0.265625);
			break;
		case SAWAMMO:
			act->spr.scale = DVector2(0.1875, 0.109375);
			break;
		case GROWSPRITEICON:
			act->spr.scale = DVector2(0.15625, 0.140625);
			break;
		case DEVISTATORAMMO:
			act->spr.scale = DVector2(0.15625, 0.140625);
			break;
		case ATOMICHEALTH:
			act->spr.scale = DVector2(0.125, 0.125);
			break;
		case FREEZESPRITE:
			act->spr.scale = DVector2(0.265625, 0.25);
			break;
		}
		act->spr.shade = act->sector()->floorshade;
		break;
	case WATERFOUNTAIN:
		act->spr.lotag = 1;
		[[fallthrough]];
	case TREE1:
	case TREE2:
	case TIRE:
		act->spr.cstat = CSTAT_SPRITE_BLOCK_ALL; // Make it hitable
		act->spr.extra = 1;
		ChangeActorStat(act, STAT_STANDABLE);
		break;

	case CAMERA1:
	case CAMERA1 + 1:
	case CAMERA1 + 2:
	case CAMERA1 + 3:
	case CAMERA1 + 4:
	case CAMERAPOLE:
		act->spr.extra = 1;

		if (gs.camerashitable) act->spr.cstat = CSTAT_SPRITE_BLOCK_ALL;
		else act->spr.cstat = 0;

		if (ud.multimode < 2 && act->spr.pal != 0)
		{
			act->spr.scale = DVector2(0, 0);
			ChangeActorStat(act, STAT_MISC);
			break;
		}
		else act->spr.pal = 0;
		if (act->spr.picnum == CAMERAPOLE) break;
		act->spr.picnum = CAMERA1;
		ChangeActorStat(act, STAT_ACTOR);
		break;
	case STEAM:
		if (actj)
		{
			act->spr.angle = actj->spr.angle;
			act->spr.cstat = CSTAT_SPRITE_ALIGNMENT_WALL | CSTAT_SPRITE_YCENTER | CSTAT_SPRITE_TRANSLUCENT;
			act->spr.scale = DVector2(REPEAT_SCALE, REPEAT_SCALE);
			act->vel.X = -0.5;
			ssp(act, CLIPMASK0);
		}
		[[fallthrough]];
	case CEILINGSTEAM:
		ChangeActorStat(act, STAT_STANDABLE);
		break;
	case SECTOREFFECTOR:
		spawneffector(act, actors);
		break;

	case SEENINE:
	case OOZFILTER:

		act->spr.shade = -16;
		if (act->spr.scale.X <= 0.125)
		{
			act->spr.cstat = CSTAT_SPRITE_INVISIBLE;
			act->spr.scale = DVector2(0, 0);
		}
		else act->spr.cstat = CSTAT_SPRITE_BLOCK_ALL;
		act->spr.extra = gs.impact_damage << 2;
		act->SetOwner(act);
		ChangeActorStat(act, STAT_STANDABLE);
		break;

	case CRACK1:
	case CRACK2:
	case CRACK3:
	case CRACK4:
		act->spr.cstat |= CSTAT_SPRITE_BLOCK | CSTAT_SPRITE_ALIGNMENT_WALL;
		act->spr.extra = 1;
		if (ud.multimode < 2 && act->spr.pal != 0)
		{
			act->spr.scale = DVector2(0, 0);
			ChangeActorStat(act, STAT_MISC);
			break;
		}

		act->spr.pal = 0;
		act->SetOwner(act);
		ChangeActorStat(act, STAT_STANDABLE);
		act->vel.X = 0.5;
		ssp(act, CLIPMASK0);
		break;

	case EMPTYBIKE:
		if (!isRRRA()) goto default_case;
		if (ud.multimode < 2 && act->spr.pal == 1)
		{
			act->spr.scale = DVector2(0, 0);
			break;
		}
		act->spr.pal = 0;
		act->spr.scale = DVector2(0.28125, 0.28125);
		act->setClipDistFromTile();
		act->saved_ammo = 100;
		act->spr.cstat = CSTAT_SPRITE_BLOCK_ALL;
		act->spr.lotag = 1;
		ChangeActorStat(act, STAT_ACTOR);
		break;
	case EMPTYBOAT:
		if (!isRRRA()) goto default_case;
		if (ud.multimode < 2 && act->spr.pal == 1)
		{
			act->spr.scale = DVector2(0, 0);
			break;
		}
		act->spr.pal = 0;
		act->spr.scale = DVector2(0.5, 0.5);
		act->setClipDistFromTile();
		act->saved_ammo = 20;
		act->spr.cstat = CSTAT_SPRITE_BLOCK_ALL;
		act->spr.lotag = 1;
		ChangeActorStat(act, STAT_ACTOR);
		break;

	case TOILET:
	case STALL:
	case RRTILE2121:
	case RRTILE2122:
		act->spr.lotag = 1;
		act->spr.cstat |= CSTAT_SPRITE_BLOCK_ALL;
		act->clipdist = 2;
		act->SetOwner(act);
		break;
	case CANWITHSOMETHING:
	case RUBBERCAN:
		act->spr.extra = 0;
		[[fallthrough]];
	case EXPLODINGBARREL:
	case HORSEONSIDE:
	case FIREBARREL:
	case NUKEBARREL:
	case FIREVASE:
	case NUKEBARRELDENTED:
	case NUKEBARRELLEAKED:
	case WOODENHORSE:

		if (actj)
			act->spr.scale = DVector2(0.5, 0.5);
		act->clipdist = 18;
		makeitfall(act);
		if (actj) act->SetOwner(actj);
		else act->SetOwner(act);
		[[fallthrough]];

	case EGG:
		if (ud.monsters_off == 1 && act->spr.picnum == EGG)
		{
			act->spr.scale = DVector2(0, 0);
			ChangeActorStat(act, STAT_MISC);
		}
		else
		{
			if (act->spr.picnum == EGG)
				act->clipdist = 6;
			act->spr.cstat = CSTAT_SPRITE_BLOCK_ALL | randomXFlip();
			ChangeActorStat(act, STAT_ZOMBIEACTOR);
		}
		break;
	case TOILETWATER:
		act->spr.shade = -16;
		ChangeActorStat(act, STAT_STANDABLE);
		break;
	case RRTILE63:
		act->spr.cstat |= CSTAT_SPRITE_INVISIBLE;
		act->spr.scale = DVector2(REPEAT_SCALE, REPEAT_SCALE);
		act->clipdist = 0.25;
		ChangeActorStat(act, 100);
		break;
	}
	return act;
}

END_DUKE_NS
