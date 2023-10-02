#pragma once

#include "build.h"

#include "gamecvars.h"
#include "razemenu.h"
#include "gamecontrol.h"
#include "gamevar.h"
#include "global.h"
#include "funct.h"
#include "names.h"
#include "quotemgr.h"
#include "rts.h"
#include "sounds.h"
#include "soundefs.h"
#include "gamestruct.h"
#include "v_draw.h"
#include "gamefuncs.h"

BEGIN_DUKE_NS

struct GameInterface : public ::GameInterface
{
	const char* Name() override { return "Duke"; }
	void app_init() override;
	void FinalizeSetup() override;
	void loadPalette() override;
	void SetupSpecialTextures(TilesetBuildInfo& info) override;
	bool GenerateSavePic() override;
	void PlayHudSound() override;
	void MenuOpened() override;
	void MenuSound(EMenuSounds snd) override;
	bool CanSave() override;
	bool StartGame(FNewGameStartup& gs) override;
	FSavegameInfo GetSaveSig() override;
	double SmallFontScale() override { return isRR() ? 0.5 : 1.; }
	void SerializeGameState(FSerializer& arc) override;
	void ExitFromMenu() override;
	void DrawPlayerSprite(const DVector2& origin, bool onteam) override;
	void doPlayerMovement(const float scaleAdjust) override;
	unsigned getCrouchState() override;
	void UpdateSounds() override;
	void Startup() override;
	void DrawBackground() override;
	void Render() override;
	void Ticker() override;
	const char* GenericCheat(int player, int cheat) override;
	const char* CheckCheatMode() override;
	void NextLevel(MapRecord* map, int skill) override;
	void NewGame(MapRecord* map, int skill, bool) override;
	void LevelCompleted(MapRecord* map, int skill) override;
	bool DrawAutomapPlayer(const DVector2& mxy, const DVector2& cpos, const DAngle cang, const DVector2& xydim, const double czoom, double const interpfrac) override;
	void ToggleThirdPerson() override;
	void SwitchCoopView() override;
	void ToggleShowWeapon() override;
	void processSprites(tspriteArray& tsprites, const DVector3& view, DAngle viewang, double interpfrac) override;
	void UpdateCameras(double smoothratio) override;
	void EnterPortal(DCoreActor* viewer, int type) override;
	void LeavePortal(DCoreActor* viewer, int type) override;
	bool GetGeoEffect(GeoEffect* eff, sectortype* viewsector) override;
	void AddExcludedEpisode(const FString& episode) override;
	int GetCurrentSkill() override;
	bool WantEscape() override;
	void StartSoundEngine() override;

};

struct Dispatcher
{
	// sectors_?.cpp
	void (*think)();
	void (*movetransports)();
	bool (*checkaccessswitch)(int snum, int switchpal, DDukeActor* act, walltype* w);
	void (*activatebysector)(sectortype* sect, DDukeActor* j);
	void (*checksectors)(int low);

	void (*addweapon)(DukePlayer *p, int weapon, bool wswitch);
	int  (*ifhitbyweapon)(DDukeActor* sectnum);

	// player
	void (*incur_damage)(DukePlayer* p);
	void (*selectweapon)(int snum, int j);
	int (*doincrements)(DukePlayer* p);
	void (*checkweapons)(DukePlayer* p);
	void (*processinput)(int snum);
	void (*displayweapon)(int snum, double interpfrac);
	void (*displaymasks)(int snum, int p, double interpfrac);

	void (*animatesprites)(tspriteArray& tsprites, const DVector2& viewVec, DAngle viewang, double interpfrac);


};

extern Dispatcher fi;

void CallInitialize(DDukeActor* actor, DDukeActor* spawner);
void CallTick(DDukeActor* actor);
bool CallOperate(DDukeActor* actor, int plnum);
void CallAction(DDukeActor* actor);
void checkhitsprite(DDukeActor* actor, DDukeActor* hitter);
void CallOnHurt(DDukeActor* actor, DukePlayer* hitter);
void CallOnTouch(DDukeActor* actor, DukePlayer* hitter);
bool CallOnUse(DDukeActor* actor, DukePlayer* user);
void CallOnMotoSmash(DDukeActor* actor, DukePlayer* hitter);
void CallOnRespawn(DDukeActor* actor, int low);
bool CallAnimate(DDukeActor* actor, tspritetype* hitter);
bool CallShootThis(DDukeActor* clsdef, DDukeActor* actor, int pn, const DVector3& spos, DAngle sang);
void CallStaticSetup(DDukeActor* actor);
void CallPlayFTASound(DDukeActor* actor, int mode = 0);
void CallStandingOn(DDukeActor* actor, DukePlayer* p);
void CallRunState(DDukeActor* actor);
int CallTriggerSwitch(DDukeActor* actor, DukePlayer* p);
PClassActor* CallGetRadiusDamageType(DDukeActor* actor, int targhealth);



extern FTextureID mirrortex, foftex;

END_DUKE_NS

