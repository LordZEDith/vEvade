#include "PathFinding.h"
#include "Evade.h"
#include "Core.h"

CPathFinding* PathFinding = nullptr;

bool CPathFinding::FindPath(Vec2 const& StartPosition, Vec2 const& EndPosition, std::vector<Vec2>& Out, bool& DodgingSkillshot)
{
	Out.clear();

	SkillshotDetected = false;

	MapGrid grid;
	JPS::PathVector path;

	Vec2 vecStart = StartPosition;
	FindNearestNonCollision(vecStart);

	Vec2 vecEnd = EndPosition;
	FindNearestNonCollision(vecEnd);

	auto outerPolys = std::vector<Geometry::IPolygon>();
	auto innerPolys = std::vector<Geometry::IPolygon>();

	for (auto spell : Evade::Spells)
	{
		outerPolys.push_back(spell->PathFindingOuterPolygon);
		innerPolys.push_back(spell->PathFindingInnerPolygon);
	}

	auto outerPolygons = Geometry::ToPolygons(Geometry::ClipPolygons(outerPolys));
	auto innerPolygons = Geometry::ToPolygons(Geometry::ClipPolygons(innerPolys));

	for (auto i : outerPolygons)
	{
		if (i.IsInside(vecEnd))
			vecEnd = Core::GetClosestOutsidePoint(vecEnd, outerPolygons);
	}

	for (auto i : outerPolygons)
	{
		if (i.IsInside(vecStart))
			vecStart = Core::GetClosestOutsidePoint(vecStart, outerPolygons);
	}

	if (Core::CanReach(vecStart, vecEnd, innerPolygons))
	{
		Out.push_back(vecStart);
		Out.push_back(vecEnd);

		return true;
	}
	
	if (JPS::findPath(path, grid, static_cast<int>(vecStart.x), static_cast<int>(vecStart.y), static_cast<int>(vecEnd.x), static_cast<int>(vecEnd.y), 0, 20))
	{
		for (auto i : path)
			Out.push_back(Vec2(i.x, i.y));

		DodgingSkillshot = SkillshotDetected;

		return true;
	}

	return false;
}

bool CPathFinding::IsPointWalkable(int X, int Y)
{
	Vec2 vecPoint(static_cast<float>(X), static_cast<float>(Y));

	if (GPrediction->IsPointWall(Util::Get3DPoint(vecPoint)))
		return false;

	if (IsWithinSkillshot(vecPoint))
	{
		SkillshotDetected = true;
		return false;
	}

	return true;
}

void CPathFinding::UpdateEndPosition(Vec2& EndPosition)
{
	FindNearestNonCollision(EndPosition);

	auto outerPolys = std::vector<Geometry::IPolygon>();

	for (auto i : Evade::DetectedSpells)
		outerPolys.push_back(i.second->Polygon);

	auto outerPolygons = Geometry::ToPolygons(Geometry::ClipPolygons(outerPolys));

	for (auto i : outerPolygons)
	{
		if (i.IsInside(EndPosition))
		{
			Vec2 vecNewEndPosition = Core::GetClosestOutsidePoint(EndPosition, outerPolygons);

			if (!IsWithinSkillshot(vecNewEndPosition) && !IsWithinWall(vecNewEndPosition))
				EndPosition = vecNewEndPosition;
		}
	}
}

void CPathFinding::FindNearestNonCollision(Vec2& EndPosition)
{
	float flSegments	= 10;
	float flRadius		= 100.f;
	float flWedge		= 2.f * 3.141519f / flSegments;

	for (float i = 1.f; i <= flSegments; i += 1.f)
	{
		float flTheta		= (i * flWedge);
		Vec2 vecNewPoint	= Vec2(EndPosition.x + flRadius * cosf(flTheta), EndPosition.y - flRadius * sinf(flTheta));

		if (!IsWithinWall(vecNewPoint))
		{
			EndPosition = vecNewPoint;
			return;
		}
	}
}

bool CPathFinding::IsWithinSkillshot(Vec2 const& EndPosition)
{
	for (auto i : Evade::Spells)
	{
		if (i->Polygon.IsInside(EndPosition))
			return true;
	}

	return false;
}

bool CPathFinding::IsWithinSkillshotPathfindingOuter(Vec2 const& EndPosition)
{
	auto outerPolys = std::vector<Geometry::IPolygon>();

	for (auto i : Evade::DetectedSpells)
		outerPolys.push_back(i.second->PathFindingOuterPolygon);

	auto outerPolygons = Geometry::ToPolygons(Geometry::ClipPolygons(outerPolys));

	for (auto i : outerPolygons)
	{
		if (i.IsInside(EndPosition))
			return true;
	}

	return false;
}

bool CPathFinding::IsWithinWall(Vec2 const& EndPosition)
{
	int iFlags = GPrediction->GetCollisionFlagsForPoint(Util::Get3DPoint(EndPosition));

	if (iFlags & kWallMesh)
		return true;

	return false;
}