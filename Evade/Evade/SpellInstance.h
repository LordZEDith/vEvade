#ifndef SpellInstance_h__
#define SpellInstance_h__

#include "PluginSDK.h"
#include "Configs.h"
#include "Polygons.h"
#include "SpellData.h"
#include "Util.h"

struct SpellInstance;

struct Intersects
{
	float Distance;
	Vec2 Point;
	int Time;
	bool Valid;
	
	Intersects(float _Distance, int _Time, Vec2 _Point, Vec2 const _ComingFrom)
	{
		Distance	= _Distance;
		Valid		= (_Point.Length() != 0.f);
		Point		= _Point + Configs->GridSize * (_ComingFrom - _Point).VectorNormalize();
		Time		= _Time;
	}

	Intersects()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct SafePath
{
	Intersects	Intersect;
	bool		IsSafe;

	SafePath(bool _IsSafe, Intersects _Intersect)
	{
		IsSafe		= _IsSafe;
		Intersect	= _Intersect;
	}
};

struct SafePoint
{
	bool IsSafe;
	std::vector<SpellInstance*> Spells;

	SafePoint(std::vector<SpellInstance*> spells)
	{
		Spells = spells;
		IsSafe = Spells.size() == 0;
	}
};

struct SpellInstance
{
	void InitializeDefaults()
	{
		memset(this, 0, sizeof(*this));
	}

	Polygons::Arc Arc;
	Polygons::Circle Circle;
	Polygons::Cone Cone;
	SpellData Data;
	Vec2 Direction;
	Geometry::IPolygon DrawPolygon;
	Vec2 End;
	int EndTick;
	bool ForceDisabled;
	bool IsFromFoW;
	Polygons::Line Line;
	IUnit* MissileObject;
	Geometry::IPolygon PathFindingInnerPolygon;
	Geometry::IPolygon PathFindingOuterPolygon;
	Geometry::IPolygon Polygon;
	Vec2 PredEnd;
	Polygons::Ring Ring;
	int SpellId;
	Vec2 Start;
	int StartTick;
	IUnit* ToggleObject;
	int Type; // SpellType
	IUnit* TrapObject;
	IUnit* Unit;
	bool cachedValue;
	int cachedValueTick;
	int lastCalcColTick;
	int radius;

	int WaitingToRemoveTime;

	SpellInstance(SpellData data, int startT, int endT, Vec2 start, Vec2 end, IUnit* unit, int type);
	bool IsEnabled();
	IMenuOption* GetValue(std::string Name);
	int GetRadius();
	int GetRawRadius();
	bool IsGlobal();
	Vec2 GetPredictEnd();
	void Draw(Vec4 const color);
	Vec2 GetGlobalMissilePosition(int time);
	Vec2 GetMissilePosition(int time);
	void UpdatePolygon();
	bool IsAboutToHit(int time, IUnit* unit);
	bool IsSafePoint(Vec2 pos);
	SafePath IsSafePath(std::vector<Vec2> paths, int time, int speed, int delay);
	bool IsSafeToBlink(Vec2 point, int time, int delay);
	void OnUpdate();
	int GetDangerLevel();
};

#endif // SpellInstance_h__
