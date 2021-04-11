#pragma once

#include "m_fixed.h"
#include "binaryangle.h"
#include "gamecvars.h"
#include "gamestruct.h"
#include "packet.h"

int getincangle(int a, int na);
double getincanglef(double a, double na);
fixed_t getincangleq16(fixed_t a, fixed_t na);
binangle getincanglebam(binangle a, binangle na);

struct PlayerHorizon
{
	fixedhoriz horiz, ohoriz, horizoff, ohorizoff;

	void backup()
	{
		ohoriz = horiz;
		ohorizoff = horizoff;
	}

	void restore()
	{
		horiz = ohoriz;
		horizoff = ohorizoff;
	}

	void addadjustment(double value)
	{
		__addadjustment(buildfhoriz(value));
	}

	void addadjustment(fixedhoriz value)
	{
		__addadjustment(value);
	}

	void resetadjustment()
	{
		adjustment = 0;
	}

	void settarget(double value, bool backup = false)
	{
		__settarget(buildfhoriz(clamp(value, FixedToFloat(gi->playerHorizMin()), FixedToFloat(gi->playerHorizMax()))), backup);
	}

	void settarget(fixedhoriz value, bool backup = false)
	{
		__settarget(q16horiz(clamp(value.asq16(), gi->playerHorizMin(), gi->playerHorizMax())), backup);
	}

	bool targetset()
	{
		return target.asq16();
	}

	void processhelpers(double const scaleAdjust)
	{
		if (targetset())
		{
			auto delta = (target - horiz).asbuildf();

			if (abs(delta) > 1)
			{
				horiz += buildfhoriz(scaleAdjust * delta);
			}
			else
			{
				horiz = target;
				target = q16horiz(0);
			}
		}
		else if (adjustment)
		{
			horiz += buildfhoriz(scaleAdjust * adjustment);
		}
	}

	fixedhoriz osum()
	{
		return ohoriz + ohorizoff;
	}

	fixedhoriz sum()
	{
		return horiz + horizoff;
	}

	fixedhoriz interpolatedsum(double const smoothratio)
	{
		return q16horiz(interpolatedvalue(osum().asq16(), sum().asq16(), smoothratio));
	}

private:
	fixedhoriz target;
	double adjustment;

	void __addadjustment(fixedhoriz value)
	{
		if (!SyncInput())
		{
			adjustment += value.asbuildf();
		}
		else
		{
			horiz += value;
		}
	}

	void __settarget(fixedhoriz value, bool backup)
	{
		if (!SyncInput() && !backup)
		{
			target = value;
			if (!targetset()) target = q16horiz(1);
		}
		else
		{
			horiz = value;
			if (backup) ohoriz = horiz;
		}
	}
};

struct PlayerAngle
{
	binangle ang, oang, look_ang, olook_ang, rotscrnang, orotscrnang;
	double spin;

	void backup()
	{
		oang = ang;
		olook_ang = look_ang;
		orotscrnang = rotscrnang;
	}

	void restore()
	{
		ang = oang;
		look_ang = olook_ang;
		rotscrnang = orotscrnang;
	}

	void addadjustment(double value)
	{
		__addadjustment(buildfang(value));
	}

	void addadjustment(binangle value)
	{
		__addadjustment(value);
	}

	void resetadjustment()
	{
		adjustment = 0;
	}

	void settarget(double value, bool backup = false)
	{
		__settarget(buildfang(value), backup);
	}

	void settarget(binangle value, bool backup = false)
	{
		__settarget(value, backup);
	}

	bool targetset()
	{
		return target.asbam();
	}

	void processhelpers(double const scaleAdjust)
	{
		if (targetset())
		{
			auto delta = getincanglebam(ang, target).signedbuildf();

			if (abs(delta) > 1)
			{
				ang += buildfang(scaleAdjust * delta);
			}
			else
			{
				ang = target;
				target = bamang(0);
			}
		}
		else if (adjustment)
		{
			ang += buildfang(scaleAdjust * adjustment);
		}
	}

	binangle osum()
	{
		return oang + olook_ang;
	}

	binangle sum()
	{
		return ang + look_ang;
	}

	binangle interpolatedsum(double const smoothratio)
	{
		return interpolatedangle(osum(), sum(), smoothratio);
	}

	binangle interpolatedlookang(double const smoothratio)
	{
		return interpolatedangle(olook_ang, look_ang, smoothratio);
	}

	binangle interpolatedrotscrn(double const smoothratio)
	{
		return interpolatedangle(orotscrnang, rotscrnang, smoothratio);
	}

	double look_anghalf(double const smoothratio)
	{
		return (!SyncInput() ? look_ang : interpolatedlookang(smoothratio)).signedbuildf() * 0.5; // Used within draw code for weapon and crosshair when looking left/right.
	}

private:
	binangle target;
	double adjustment;

	void __addadjustment(binangle value)
	{
		if (!SyncInput())
		{
			adjustment += value.asbuildf();
		}
		else
		{
			ang += value;
		}
	}

	void __settarget(binangle value, bool backup)
	{
		if (!SyncInput() && !backup)
		{
			target = value;
			if (!targetset()) target = bamang(1);
		}
		else
		{
			ang = value;
			if (backup) oang = ang;
		}
	}
};

class FSerializer;
FSerializer& Serialize(FSerializer& arc, const char* keyname, PlayerAngle& w, PlayerAngle* def);
FSerializer& Serialize(FSerializer& arc, const char* keyname, PlayerHorizon& w, PlayerHorizon* def);


void updateTurnHeldAmt(double const scaleAdjust);
bool const isTurboTurnTime();
void resetTurnHeldAmt();
void processMovement(InputPacket* currInput, InputPacket* inputBuffer, ControlInfo* const hidInput, double const scaleAdjust, int const drink_amt = 0, bool const allowstrafe = true, double const turnscale = 1);
void sethorizon(PlayerHorizon* horizon, float const horz, ESyncBits* actions, double const scaleAdjust = 1);
void applylook(PlayerAngle* angle, float const avel, ESyncBits* actions, double const scaleAdjust = 1);
void calcviewpitch(vec2_t const pos, fixedhoriz* horizoff, binangle const ang, bool const aimmode, bool const canslopetilt, int const cursectnum, double const scaleAdjust = 1, bool const climbing = false);
