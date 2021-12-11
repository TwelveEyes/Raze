/*
/*
** sectorgeometry.cpp
**
** caches the triangle meshes used for rendering sector planes.
**
**---------------------------------------------------------------------------
** Copyright 2021 Christoph Oelckers
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**---------------------------------------------------------------------------
**
**
*/  

#include "sectorgeometry.h"
#include "build.h"
#include "gamefuncs.h"
#include "texturemanager.h"
#include "earcut.hpp"
#include "hw_sections2.h"
#include "tesselator.h"
#include "nodebuilder/nodebuild.h"

SectorGeometry sectorGeometry;

//==========================================================================
//
// CalcPlane fixme - this should be stored in the sector, not be recalculated each frame.
//
//==========================================================================

static FVector3 CalcNormal(sectortype* sector, int plane)
{
	FVector3 pt[3];

	auto wal = sector->firstWall();
	auto wal2 = wal->point2Wall();

	pt[0] = { (float)WallStartX(wal), (float)WallStartY(wal), 0 };
	pt[1] = { (float)WallEndX(wal), (float)WallEndY(wal), 0 };
	PlanesAtPoint(sector, wal->x, wal->y, plane ? &pt[0].Z : nullptr, plane? nullptr : &pt[0].Z);
	PlanesAtPoint(sector, wal2->x, wal2->y, plane ? &pt[1].Z : nullptr, plane ? nullptr : &pt[1].Z);

	if (pt[0].X == pt[1].X)
	{
		if (pt[0].Y == pt[1].Y) return { 0.f, 0.f, plane ? -1.f : 1.f };
		pt[2].X = pt[0].X + 4;
		pt[2].Y = pt[0].Y;
	}
	else
	{
		pt[2].X = pt[0].X;
		pt[2].Y = pt[0].Y + 4;
	}
	PlanesAtPoint(sector, pt[2].X * 16, pt[2].Y * 16, plane ? &pt[2].Z : nullptr, plane ? nullptr : &pt[2].Z);

	auto normal = (pt[2] - pt[0]) ^ (pt[1] - pt[0]);

	if ((pt[2].Z < 0 && !plane) || (pt[2].Z > 0 && plane)) return -pt[2];
	return pt[2];
}

//==========================================================================
//
// The math used here to calculate texture positioning was derived from
// Polymer but required several fixes for correctness. 
//
//==========================================================================
class UVCalculator
{
	sectortype* sect;
	int myplane;
	int stat;
	float z1;
	int ix1;
	int iy1;
	int ix2;
	int iy2;
	float sinalign, cosalign;
	FGameTexture* tex;
	float xpanning, ypanning;
	float xscaled, yscaled;
	FVector2 offset;

public:

	UVCalculator(sectortype* sec, int plane, FGameTexture* tx, const FVector2& off)
	{
		float xpan, ypan;

		sect = sec;
		tex = tx;
		myplane = plane;
		offset = off;

		auto firstwall = sec->firstWall();
		ix1 = firstwall->x;
		iy1 = firstwall->y;
		ix2 = firstwall->point2Wall()->x;
		iy2 = firstwall->point2Wall()->y;

		if (plane == 0)
		{
			stat = sec->floorstat;
			xpan = sec->floorxpan_;
			ypan = sec->floorypan_;
			PlanesAtPoint(sec, ix1, iy1, nullptr, &z1);
		}
		else
		{
			stat = sec->ceilingstat;
			xpan = sec->ceilingxpan_;
			ypan = sec->ceilingypan_;
			PlanesAtPoint(sec, ix1, iy1, &z1, nullptr);
		}

		DVector2 dv = { double(ix2 - ix1), -double(iy2 - iy1) };
		auto vang = dv.Angle() - 90.;

		cosalign = float(vang.Cos());
		sinalign = float(vang.Sin());

		int pow2width = 1 << sizeToBits((int)tx->GetDisplayWidth());
		int pow2height = 1 << sizeToBits((int)tx->GetDisplayHeight());

		xpanning = xpan / 256.f;
		ypanning = ypan / 256.f;

		float scalefactor = (stat & CSTAT_SECTOR_TEXHALF) ? 8.0f : 16.0f;

		if ((stat & (CSTAT_SECTOR_SLOPE | CSTAT_SECTOR_ALIGN)) == (CSTAT_SECTOR_ALIGN))
		{
			// This is necessary to adjust for some imprecisions in the math.
			// To calculate the inverse Build performs an integer division with significant loss of precision
			// that can cause the texture to be shifted by multiple pixels.
			// The code below calculates the amount of this deviation so that it can be added back to the formula.
			int len = ksqrt(uhypsq(ix2 - ix1, iy2 - iy1));
			if (len != 0)
			{
				int i = 1048576 / len;
				scalefactor *= 1048576.f / (i * len);
			}
		}

		xscaled = scalefactor * pow2width;
		yscaled = scalefactor * pow2height;
	}

	FVector2 GetUV(int x, int y, float z)
	{
		float tv, tu;

		if (stat & CSTAT_SECTOR_ALIGN)
		{
			float dx = (float)(x - ix1);
			float dy = (float)(y - iy1);

			tu = -(dx * sinalign + dy * cosalign);
			tv = (dx * cosalign - dy * sinalign);

			if (stat & CSTAT_SECTOR_SLOPE)
			{
				float dz = (z - z1) * 16;
				float newtv = sqrt(tv * tv + dz * dz);
				tv = tv < 0 ? -newtv : newtv;
			}
		}
		else 
		{
			tu = x - offset.X;
			tv = -y - offset.Y;
		}

		if (stat & CSTAT_SECTOR_SWAPXY)
			std::swap(tu, tv);

		if (stat & CSTAT_SECTOR_XFLIP) tu = -tu;
		if (stat & CSTAT_SECTOR_YFLIP) tv = -tv;



		return { tu / xscaled + xpanning, tv / yscaled + ypanning };

	}
};


enum class ETriangulateResult
{
	Ok = 0,
	Failed = 1,		// unable to triangulate
	Invalid = 2,	// input data invalid.
};

//==========================================================================
//
// Convert the outline to render coordinates.
//
//==========================================================================

static int OutlineToFloat(Outline& outl, FOutline& polygon)
{
	polygon.resize(outl.Size());
	unsigned count = 0;
	for (unsigned i = 0; i < outl.Size(); i++)
	{
		polygon[i].resize(outl[i].Size());
		count += outl[i].Size();
		for (unsigned j = 0; j < outl[i].Size(); j++)
		{
			float X = RenderX(outl[i][j].x);
			float Y = RenderX(outl[i][j].y);
				if (fabs(X) > 32768.f || fabs(Y) > 32768.f)
				{
					// If we get here there's some fuckery going around with the coordinates. Let's better abort and wait for things to realign.
					// Do not try alternative methods if this happens.
				return -1;
				}
			polygon[i][j] = { X, Y };
			}
		}
	return count;
}

//==========================================================================
//
// Try to triangulate a given outline with Earcut.
//
//==========================================================================

ETriangulateResult TriangulateOutlineEarcut(const FOutline& polygon, int count, TArray<FVector2>& points, TArray<int>& indicesOut)
{
	// Sections are already validated so we can assume that the data is well defined here.

	auto indices = mapbox::earcut(polygon);
	if (indices.size() < 3 * count)
	{
		// this means that full triangulation failed.
		return ETriangulateResult::Failed;
	}
	points.Resize(count);
	int p = 0;
	for (size_t a = 0; a < polygon.size(); a++)
	{
		for (auto& pt : polygon[a])
		{
			points[p++] = { pt.first, pt.second };
		}
	}
	indicesOut.Resize(count * 3);
	for (unsigned i = 0; i < indices.size(); i++)
	{
		indicesOut[i] = indices[i];
	}
	return ETriangulateResult::Ok;
}

//==========================================================================
//
// Try to triangulate a given outline with libtess2.
//
//==========================================================================

ETriangulateResult TriangulateOutlineLibtess(const FOutline& polygon, int count, TArray<FVector2>& points, TArray<int>& indicesOut)
{
	FMemArena tessArena(100000);

	auto poolAlloc = [](void* userData, unsigned int size) -> void*
	{
		FMemArena* pool = (FMemArena*)userData;
		return pool->Alloc(size);
	};

	auto poolFree = [](void*, void*)
	{
	};

	TESSalloc ma{};
	ma.memalloc = poolAlloc;
	ma.memfree = poolFree;
	ma.userData = (void*)&tessArena;
	ma.extraVertices = 256; // realloc not provided, allow 256 extra vertices.

	auto tess = tessNewTess(&ma);
	if (!tess)
		return ETriangulateResult::Failed;

	//tessSetOption(tess, TESS_CONSTRAINED_DELAUNAY_TRIANGULATION, 1);

	// Add contours.
	for (auto& loop : polygon)
		tessAddContour(tess, 2, &loop.data()->first, (int)sizeof(*loop.data()), loop.size());

	int result = tessTesselate(tess, TESS_WINDING_POSITIVE, TESS_POLYGONS, 3, 2, 0);
	if (!result)
		{
		tessDeleteTess(tess);
		return ETriangulateResult::Failed;
		}

	const float* verts = tessGetVertices(tess);
	const int* vinds = tessGetVertexIndices(tess);
	const int* elems = tessGetElements(tess);
	const int nverts = tessGetVertexCount(tess);
	const int nelems = tessGetElementCount(tess);

	points.Resize(nverts);
	indicesOut.Resize(nelems);
	for (int i = 0; i < nverts; i++)
			{
		points[i] = { verts[i * 2], verts[i * 2 + 1] };
			}
	for (int i = 0; i < nelems; i++)
			{
		indicesOut[i] = elems[i];
	}
	return ETriangulateResult::Ok;
}

//==========================================================================
//
// Try to triangulate a given outline with the ZDoom node builder.
//
//==========================================================================

ETriangulateResult TriangulateOutlineNodeBuild(const FOutline& polygon, int count, TArray<FVector2>& points, TArray<int>& indicesOut)
{
	TArray<vertex_t> vertexes(count, true);
	TArray<line_t> lines(count, true);
	TArray<side_t> sides(count, true);
	sectortype dummy;

	int n = 0;
	for (size_t i = 0; i < polygon.size(); i++)
				{
		int firstn = n;
		for (size_t j = 0; j < polygon[i].size(); i++)
				{
			vertexes[n].set(polygon[i][j].first, polygon[i][j].second);

			lines[n].backsector = nullptr;
			lines[n].frontsector = &dummy;
			lines[n].linenum = (int)j;
			lines[n].wallnum = n;
			lines[n].sidedef[0] = &sides[n];
			lines[n].sidedef[1] = nullptr;
			lines[n].v1 = &vertexes[n];
			lines[n].v2 = j == polygon[i].size() ? &vertexes[firstn] : &vertexes[n + 1];

			sides[n].sidenum = n;
			sides[n].sector = &dummy;

				}
				}

	FNodeBuilder::FLevel leveldata =
	{
		&vertexes[0], count,
		&sides[0], count,
		&lines[0], count,
		0, 0, 0, 0
	};
	leveldata.FindMapBounds();
	FNodeBuilder builder(leveldata);

	FLevelLocals Level;
	builder.Extract(Level);

	points.Resize(Level.vertexes.Size());
	for (unsigned i = 0; i < Level.vertexes.Size(); i++)
	{
		points[i] = { (float)Level.vertexes[i].fX(), (float)Level.vertexes[i].fY() };
	}

	indicesOut.Clear();
	for (auto& sub : Level.subsectors)
	{
		if (sub.numlines <= 2) continue;
		auto v0 = Level.vertexes.IndexOf(sub.firstline->v1);
		for (unsigned i = 1; i < sub.numlines - 1; i++)
		{
			auto v1 = Level.vertexes.IndexOf(sub.firstline->v1);
			auto v2 = Level.vertexes.IndexOf(sub.firstline->v2);
			indicesOut.Push(v0);
			indicesOut.Push(v1);
			indicesOut.Push(v2);
		}
	}
	return ETriangulateResult::Ok;
}

//==========================================================================
//
//
//
//==========================================================================

bool SectionGeometry::ValidateSection(Section2* section, int plane, const FVector2& offset)
{
	auto sec = section->sector;
	auto& sdata = data[section->index];

	auto compare = &sdata.compare[plane];
	if (plane == 0)
	{
		if (sec->floorheinum == compare->floorheinum &&
			sec->floorpicnum == compare->floorpicnum &&
			((sec->floorstat ^ compare->floorstat) & (CSTAT_SECTOR_ALIGN | CSTAT_SECTOR_YFLIP | CSTAT_SECTOR_XFLIP | CSTAT_SECTOR_TEXHALF | CSTAT_SECTOR_SWAPXY)) == 0 &&
			sec->floorxpan_ == compare->floorxpan_ &&
			sec->floorypan_ == compare->floorypan_ &&
			sec->firstWall()->pos == sdata.poscompare[0] &&
			sec->firstWall()->point2Wall()->pos == sdata.poscompare2[0] &&
			!(sdata.dirty & EDirty::FloorDirty) && sdata.planes[plane].vertices.Size() ) return true;

		sdata.dirty &= EDirty::FloorDirty;
	}
	else
	{
		if (sec->ceilingheinum == compare->ceilingheinum &&
			sec->ceilingpicnum == compare->ceilingpicnum &&
			((sec->ceilingstat ^ compare->ceilingstat) & (CSTAT_SECTOR_ALIGN | CSTAT_SECTOR_YFLIP | CSTAT_SECTOR_XFLIP | CSTAT_SECTOR_TEXHALF | CSTAT_SECTOR_SWAPXY)) == 0 &&
			sec->ceilingxpan_ == compare->ceilingxpan_ &&
			sec->ceilingypan_ == compare->ceilingypan_ &&
			sec->firstWall()->pos == sdata.poscompare[1] &&
			sec->firstWall()->point2Wall()->pos == sdata.poscompare2[1] &&
			!(sdata.dirty & EDirty::CeilingDirty) && sdata.planes[1].vertices.Size()) return true;

		sdata.dirty &= ~EDirty::CeilingDirty;
	}
	*compare = *sec;
	sdata.poscompare[plane] = sec->firstWall()->pos;
	sdata.poscompare2[plane] = sec->firstWall()->point2Wall()->pos;
	return false;
}


void SectionGeometry::MarkDirty(sectortype* sector)
{
	for (auto section : sections2PerSector[sectnum(sector)])
	{
		data[section->index].dirty = sector->dirty;
	}
}
