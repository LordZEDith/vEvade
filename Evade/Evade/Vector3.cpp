#include "Vector3.h"
#include <limits>

void ProjectPointOn(Vec2 const& Point, Vec2 const& SegmentStart, Vec2 const& SegmentEnd, ProjectionInfo& Out)
{
	auto cx = Point.x;
	auto cy = Point.y;
	auto ax = SegmentStart.x;
	auto ay = SegmentStart.y;
	auto bx = SegmentEnd.x;
	auto by = SegmentEnd.y;
	auto rL = ((cx - ax) * (bx - ax) + (cy - ay) * (by - ay)) /
		(powf(bx - ax, 2) + powf(by - ay, 2));
	auto pointLine = Vec2(ax + rL * (bx - ax), ay + rL * (by - ay));
	float rS;
	if (rL < 0)
	{
		rS = 0;
	}
	else if (rL > 1)
	{
		rS = 1;
	}
	else
	{
		rS = rL;
	}

	auto isOnSegment = (rS == rL);
	auto pointSegment = isOnSegment ? pointLine : Vec2(ax + rS * (bx - ax), ay + rS * (by - ay));

	Out = ProjectionInfo(isOnSegment, pointSegment, pointLine);
}

float Vec2::Distance(Vec2 const& SegmentStart, Vec2 const& SegmentEnd, bool OnlyIfOnSegment, bool Squared)
{
	ProjectionInfo out;
	ProjectPointOn(*this, SegmentStart, SegmentEnd, out);

	if (out.IsOnSegment || !OnlyIfOnSegment)
		return Squared ? out.SegmentPoint.DistanceToSqr(*this) : out.SegmentPoint.DistanceTo(*this);

	return FLT_MAX;
}