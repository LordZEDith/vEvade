#include "SpellInstance.h"
#include "Evade.h"
#include "Collision.h"

#undef min;
#undef max;

#include <algorithm>

void ProjectPointOn(Vec2 const& Point, Vec2 const& SegmentStart, Vec2 const& SegmentEnd, ProjectionInfo& Out);

struct IntersectionResult
{
	bool Intersects;
	Vec2 Point;

	IntersectionResult(bool _Intersects = false, Vec2 _Point = Vec2(0, 0))
	{
		Intersects = _Intersects;
		Point = _Point;
	}
};

IntersectionResult Intersection(
	Vec2 lineSegment1Start,
	Vec2 lineSegment1End,
	Vec2 lineSegment2Start,
	Vec2 lineSegment2End)
{
	double deltaACy = lineSegment1Start.y - lineSegment2Start.y;
	double deltaDCx = lineSegment2End.x - lineSegment2Start.x;
	double deltaACx = lineSegment1Start.x - lineSegment2Start.x;
	double deltaDCy = lineSegment2End.y - lineSegment2Start.y;
	double deltaBAx = lineSegment1End.x - lineSegment1Start.x;
	double deltaBAy = lineSegment1End.y - lineSegment1Start.y;

	auto denominator = deltaBAx * deltaDCy - deltaBAy * deltaDCx;
	auto numerator = deltaACy * deltaDCx - deltaACx * deltaDCy;

	if (fabs(denominator) < FLT_EPSILON)
	{
		if (fabs(numerator) < FLT_EPSILON)
		{
			// collinear. Potentially infinite intersection points.
			// Check and return one of them.
			if (lineSegment1Start.x >= lineSegment2Start.x && lineSegment1Start.x <= lineSegment2End.x)
			{
				return IntersectionResult(true, lineSegment1Start);
			}
			if (lineSegment2Start.x >= lineSegment1Start.x && lineSegment2Start.x <= lineSegment1End.x)
			{
				return IntersectionResult(true, lineSegment2Start);
			}
			return IntersectionResult();
		}
		// parallel
		return IntersectionResult();
	}

	auto r = numerator / denominator;

	if (r < 0 || r > 1)
	{
		return IntersectionResult();
	}

	auto s = (deltaACy * deltaBAx - deltaACx * deltaBAy) / denominator;

	if (s < 0 || s > 1)
	{
		return IntersectionResult();
	}

	return IntersectionResult(
		true,
		Vec2((float)(lineSegment1Start.x + r * deltaBAx), (float)(lineSegment1Start.y + r * deltaBAy)));
}

SpellInstance::SpellInstance(SpellData data, int startT, int endT, Vec2 start, Vec2 end, IUnit* unit, int type)
{
	InitializeDefaults();

	Data = data;
	StartTick = startT;
	EndTick = StartTick + endT;
	Start = start;
	End = end;
	Direction = (end - start).VectorNormalize();
	Unit = unit;
	Type = type;
	radius = GetRawRadius();

	switch (Type)
	{
	case ST_Line:
	case ST_MissileLine:
		Line = Polygons::Line(Start, End, GetRadius());
		break;
	case ST_Cone:
	case ST_MissileCone:
		Cone = Polygons::Cone(Start, Direction, GetRadius(), data.GetRange());
		break;
	case ST_Circle:
		Circle = Polygons::Circle(End, GetRadius());
		break;
	case ST_Ring:
		Ring = Polygons::Ring(End, data.RadiusEx, GetRadius());
		break;
	case ST_Arc:
		Arc = Polygons::Arc(Start, End, GetRadius());
		break;
	}

	UpdatePolygon();
}

bool SpellInstance::IsEnabled()
{
	if (ForceDisabled)
		return false;

	if (GGame->TickCount() - cachedValueTick <= 100)
		return cachedValue;

	cachedValueTick = GGame->TickCount();

	if (GetAsyncKeyState(Configs->DodgeDangerous->GetInteger()) && !GetValue("IsDangerous")->Enabled())
	{
		cachedValue = false;
		return cachedValue;
	}

	cachedValue = GetValue("Enabled")->Enabled();

	if (cachedValue)
	{
		switch (Type)
		{
		case ST_Line:
		case ST_MissileLine:
			cachedValue = Configs->DodgeLine->Enabled();
			break;
		case ST_Cone:
		case ST_MissileCone:
			cachedValue = Configs->DodgeCone->Enabled();
			break;
		case ST_Circle:
			cachedValue = (Data.TrapName.size() != 0) ? Configs->DodgeTrap->Enabled() : Configs->DodgeCircle->Enabled();
			break;
		}

		if (Configs->CheckHp->Enabled())
			cachedValue = (GEntityList->Player()->HealthPercent() <= static_cast<float>(GetValue("IgnoreHp")->GetInteger()));

		if (IsFromFoW && !Configs->DodgeFoW->Enabled())
			cachedValue = false;
	}

	return cachedValue;
}

IMenuOption* SpellInstance::GetValue(std::string Name)
{
	return Configs->GetOptionForSpellName("S_" + Data.MenuName + "_" + Name);
}

int SpellInstance::GetRadius()
{
	return radius + Configs->ExtraSpellRadius;
}

int SpellInstance::GetRawRadius()
{
	if (Type == ST_Circle && (Data.HasStartExplosion || Data.HasEndExplosion))
		return Data.RadiusEx + static_cast<int>(GEntityList->Player()->BoundingRadius());

	return Data.GetRadius();
}

bool SpellInstance::IsGlobal()
{
	return (Data.RawRange == 25000);
}

Vec2 SpellInstance::GetPredictEnd()
{
	if (PredEnd.IsValid())
		return PredEnd;

	if (IsGlobal())
		return GetGlobalMissilePosition(0) + Direction * static_cast<float>(Data.MissileSpeed) * (0.5f + static_cast<float>(GetRadius()) * 2.f / GEntityList->Player()->MovementSpeed());

	return End;
}

void SpellInstance::Draw(Vec4 const color)
{
	if (!GetValue("Draw")->Enabled())
		return;

	DrawPolygon.Draw(color);

	if (Type == ST_MissileLine)
	{
		auto pos = GetMissilePosition(0);
		Util::DrawLine(pos + GetRadius() * Direction.Perpendicular(), pos - GetRadius() * Direction.Perpendicular(), Vec4(50, 205, 50, 255));
	}
}

Vec2 SpellInstance::GetGlobalMissilePosition(int time)
{
	auto t = std::max(0, GGame->TickCount() + time - StartTick - Data.Delay);
	t = std::max(0, std::min(static_cast<int>((End - Start).Length()), static_cast<int>(static_cast<float>(static_cast<float>(t) * static_cast<float>(Data.MissileSpeed) / 1000.f))));

	return Start + Direction * static_cast<float>(t);
}

Vec2 SpellInstance::GetMissilePosition(int time)
{
	if (Data.MissileSpeed == 0)
		return Start;

	auto t = std::max(0, GGame->TickCount() + time - StartTick - Data.Delay);
	int x;

	if (Data.MissileAccel == 0)
	{
		x = t * Data.MissileSpeed / 1000;
	}
	else
	{
		auto t1 = (Data.MissileAccel > 0
			? Data.MissileMaxSpeed
			: Data.MissileMinSpeed - Data.MissileSpeed) * 1000 / Data.MissileAccel;
		x =
			(int)
			(t <= t1
			? t * Data.MissileSpeed / 1000 + 0.5 * Data.MissileAccel * pow(t / 1000, 2)
			: t1 * Data.MissileSpeed / 1000 + 0.5 * Data.MissileAccel * pow(t1 / 1000, 2)
			+ (t - t1) / 1000
			* (Data.MissileAccel < 0 ? Data.MissileMaxSpeed : Data.MissileMinSpeed));
	}

	t = std::max(0, std::min(static_cast<int>(GetPredictEnd().DistanceTo(Start)), x));

	return Start + Direction * static_cast<float>(t);
}

bool SpellInstance::IsSafePoint(Vec2 pos)
{
	return Polygon.IsOutside(pos);
}

bool SpellInstance::IsAboutToHit(int time, IUnit* unit)
{
	if (IsSafePoint(unit->ServerPosition().To2D()))
		return false;

	if (Type == ST_MissileLine)
		return unit->ServerPosition().To2D().Distance(GetMissilePosition(0), GetMissilePosition(time), true) < GetRadius();

	return Data.ExtraDuration + (EndTick - StartTick) - (GGame->TickCount() - StartTick) <= time;
}

Vec2 PositionAfter(std::vector<Vec2> self, int t, int s, int delay = 0)
{
	auto distance = std::max(0, t - delay) * s / 1000;

	for (size_t i = 0; i < self.size() - 1; i++)
	{
		auto from = self[i];
		auto to = self[i + 1];
		auto d = static_cast<int>(to.DistanceTo(from));

		if (d > distance)
			return from + static_cast<float>(distance) * (to - from).VectorNormalize();

		distance -= d;
	}

	return self.back();
}

SafePath SpellInstance::IsSafePath(std::vector<Vec2> paths, int time, int speed, int delay)
{
	auto dist = 0.f;
	time += GGame->Latency() / 2;
	speed = (speed == -1) ? static_cast<int>(GEntityList->Player()->MovementSpeed()) : speed;
	auto intersects = std::vector<Intersects>();
	
	if (paths.size() > 1)
	{
		for (size_t i = 0; i < paths.size() - 1; i++)
		{
			auto from = paths[i];
			auto to = paths[i + 1];
			auto segments = std::vector<Intersects>();

			for (size_t j = 0; j < Polygon.Points.size(); j++)
			{
				auto inter = Intersection(from, to, Polygon.Points[j], Polygon.Points[(j == Polygon.Points.size() - 1) ? 0 : j + 1]);

				if (!inter.Intersects)
					continue;

				auto d = dist + inter.Point.DistanceTo(from);
				segments.push_back(Intersects(d, static_cast<int>(d / static_cast<float>(speed) * 1000.f), inter.Point, from));
			}

			dist += from.DistanceTo(to);

			std::sort(segments.begin(), segments.end(), [](Intersects First, Intersects Second)
			{
				return (First.Distance < Second.Distance);
			});

			intersects.insert(intersects.end(), segments.begin(), segments.end());
		}
	}

	if (Type == ST_MissileLine || Type == ST_MissileCone || Type == ST_Arc)
	{
		if (IsSafePoint(Evade::PlayerPosition))
		{
			if (intersects.size() == 0)
				return SafePath(true, Intersects());

			if (Data.DontCross)
				return SafePath(false, intersects[0]);

			for (size_t i = 0; i < intersects.size(); i += 2)
			{
				auto enterInter = intersects[i];

				ProjectionInfo piout;
				ProjectPointOn(enterInter.Point, Start, End, piout);
				auto enterInterSegment = piout.SegmentPoint;

				if (i == intersects.size() - 1)
					return SafePath(End.DistanceTo(GetMissilePosition(enterInter.Time - time)) + 50 <= End.DistanceTo(enterInterSegment) && static_cast<int>(GEntityList->Player()->MovementSpeed()) < Data.MissileSpeed, intersects[0]);

				auto exitInter = intersects[i + 1];

				ProjectionInfo piout2;
				ProjectPointOn(exitInter.Point, Start, End, piout2);

				auto exitInterSegment = piout2.SegmentPoint;

				if (GetMissilePosition(enterInter.Time - time).DistanceTo(End) + 50 > enterInterSegment.DistanceTo(End) && GetMissilePosition(exitInter.Time + time).DistanceTo(End) <= exitInterSegment.DistanceTo(End))
					return SafePath(false, intersects[0]);
			}

			return SafePath(true, intersects[0]);
		}

		if (intersects.size() == 0)
			return SafePath(false, Intersects());

		auto exit = intersects[0];

		ProjectionInfo piout3;
		ProjectPointOn(exit.Point, Start, End, piout3);

		auto exitSegment = piout3.SegmentPoint;

		if (GetMissilePosition(exit.Time + time).DistanceTo(End) <= exitSegment.DistanceTo(End))
			return SafePath(false, intersects[0]);
	}

	if (intersects.size() == 0)
		return SafePath(IsSafePoint(Evade::PlayerPosition), Intersects());

	if (Data.DontCross && IsSafePoint(Evade::PlayerPosition))
		return SafePath(false, intersects[0]);

	auto endT = (Data.DontAddExtraDuration ? 0 : Data.ExtraDuration) + (EndTick - StartTick) - (GGame->TickCount() - StartTick);

	return (!IsSafePoint(PositionAfter(paths, endT, speed, delay)) ? SafePath(false, intersects[0]) : SafePath(IsSafePoint(PositionAfter(paths, endT, speed, time)), intersects[0]));
}

bool SpellInstance::IsSafeToBlink(Vec2 point, int time, int delay)
{
	time /= 2;

	if (IsSafePoint(point))
		return true;

	if (Type == ST_MissileLine)
	{
		return point.Distance(GetMissilePosition(0), GetMissilePosition(delay + time), true) < GetRadius();
	}

	return Data.ExtraDuration + (EndTick - StartTick) - (GGame->TickCount() - StartTick) > time + delay;
}

void SpellInstance::OnUpdate()
{
	if (Data.CollisionObjects != kCollidesWithNothing)
	{
		if (GGame->TickCount() - lastCalcColTick > 50 && Configs->CheckCollision->Enabled())
		{
			lastCalcColTick = GGame->TickCount();
			PredEnd = Collisions::GetCollision(this);
		}
	}
	else if (PredEnd.IsValid())
	{
		PredEnd = Vec2(0.f, 0.f);
	}

	if (Type == ST_Line || Type == ST_MissileLine)
	{
		Line = Polygons::Line(GetMissilePosition(0), GetPredictEnd(), GetRadius());
		UpdatePolygon();
	}

	if (Type == ST_Circle && Data.TrapName.size() == 0)
	{
		Circle = Polygons::Circle(GetPredictEnd(), GetRadius());
		UpdatePolygon();
	}

	if (!Unit->IsVisible())
		return;

	if (Data.MissileToUnit)
	{
		End = Unit->ServerPosition().To2D();
		Direction = (End - Start).VectorNormalize();

		if (Type == ST_Ring)
		{
			Ring.Center = End;
			UpdatePolygon();
		}
	}

	if (Data.MissileFromUnit)
	{
		Start = Unit->ServerPosition().To2D();
		End = Start + Direction * Data.GetRange();
	}

	if (Data.MenuName == "GalioR" && !Unit->HasBuff("GalioIdolOfDurand") && GGame->TickCount() - StartTick > Data.Delay + 300)
	{
		EndTick = 0;
	}

	if (Data.MenuName == "SionR")
	{
		if (Unit->HasBuff("SionR"))
		{
			Start = Unit->ServerPosition().To2D();
			End = Start + Unit->Direction().To2D().Perpendicular() * Data.GetRange();
			Direction = (End - Start).VectorNormalize();
			Data.MissileSpeed = static_cast<int>(Unit->MovementSpeed());
			StartTick = GGame->TickCount();
			EndTick = StartTick + static_cast<int>(Start.DistanceTo(End) * 1000.f / static_cast<float>(Data.MissileSpeed));
		}
		else
		{
			EndTick = GGame->TickCount() - StartTick > 500 ? 0 : GGame->TickCount() + 100;
		}
	}

	if (Data.MenuName == "MonkeyKingR")
	{
		if (Unit->HasBuff("MonkeyKingSpinToWin"))
		{
			StartTick = GGame->TickCount();
			EndTick = StartTick + 10;
		}
		else
		{
			EndTick = GGame->TickCount() - StartTick > 500 ? 0 : GGame->TickCount() + 100;
		}
	}
}

void SpellInstance::UpdatePolygon()
{
	auto iExtraRadius = 8;

	switch (Type)
	{
	case ST_Line:
	case ST_MissileLine:
		Polygon = Line.ToPolygon(iExtraRadius);
		DrawPolygon = Line.ToPolygon(
			0,
			GetRadius() - (!Data.AddHitbox ? 0 : static_cast<int>(GEntityList->Player()->BoundingRadius())));
		PathFindingOuterPolygon = Line.ToPolygon(Configs->PathFindingOuterDistance);
		PathFindingInnerPolygon = Line.ToPolygon(Configs->PathFindingInnerDistance);
		break;
	case ST_Cone:
	case ST_MissileCone:
		Polygon = Cone.ToPolygon(iExtraRadius);
		DrawPolygon = Polygon;
		PathFindingOuterPolygon = Cone.ToPolygon(Configs->PathFindingOuterDistance);
		PathFindingInnerPolygon = Cone.ToPolygon(Configs->PathFindingInnerDistance);
		break;
	case ST_Circle:
		Polygon = Circle.ToPolygon(iExtraRadius);

		if (Data.HasStartExplosion || Data.HasEndExplosion)
			DrawPolygon = Circle.ToPolygon(0, GetRadius() - static_cast<int>(GEntityList->Player()->BoundingRadius()));
		else
			DrawPolygon = Circle.ToPolygon(0, GetRadius() - (!Data.AddHitbox ? 0 : static_cast<int>(GEntityList->Player()->BoundingRadius())));

		PathFindingOuterPolygon = Circle.ToPolygon(Configs->PathFindingOuterDistance);
		PathFindingInnerPolygon = Circle.ToPolygon(Configs->PathFindingInnerDistance);
		break;
	case ST_Ring:
		Polygon = Ring.ToPolygon(iExtraRadius);
		DrawPolygon = Polygon;
		PathFindingOuterPolygon = Ring.ToPolygon(Configs->PathFindingOuterDistance);
		PathFindingInnerPolygon = Ring.ToPolygon(Configs->PathFindingInnerDistance);
		break;
	case ST_Arc:
		Polygon = Arc.ToPolygon(iExtraRadius);
		DrawPolygon = Polygon;
		PathFindingOuterPolygon = Arc.ToPolygon(Configs->PathFindingOuterDistance);
		PathFindingInnerPolygon = Arc.ToPolygon(Configs->PathFindingInnerDistance);
		break;
	}
}

int SpellInstance::GetDangerLevel()
{
	auto pOption = GetValue("DangerLvl");

	if (pOption != nullptr)
		return pOption->GetInteger();

	return Data.DangerValue;
}