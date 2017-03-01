#ifndef PathFinding_h__
#define PathFinding_h__

#include "PluginSDK.h"
#include "Polygons.h"
#include "JPS.h"

class CPathFinding
{
public:
	bool IsPointWalkable(int X, int Y);
	bool FindPath(Vec2 const& StartPosition, Vec2 const& EndPosition, std::vector<Vec2>& Out, bool& DodgingSkillshot);

private:
	void UpdateEndPosition(Vec2& EndPosition);
	void FindNearestNonCollision(Vec2& EndPosition);
	bool IsWithinSkillshot(Vec2 const& EndPosition);
	bool IsWithinWall(Vec2 const& EndPosition);
	bool IsWithinSkillshotPathfindingOuter(Vec2 const& EndPosition);

private:
	bool SkillshotDetected;
};

extern CPathFinding* PathFinding;

struct MapGrid
{
	inline bool operator()(unsigned x, unsigned y) const
	{
		return PathFinding->IsPointWalkable(x, y);
	}
};

#endif // PathFinding_h__
