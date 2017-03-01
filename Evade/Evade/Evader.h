#ifndef Evader_h__
#define Evader_h__

#include "PluginSDK.h"
#include "SpellInstance.h"
#include "Evade.h"
#include "EvadeSpellData.h"

#include <vector>
#include <algorithm>

void ProjectPointOn(Vec2 const& Point, Vec2 const& SegmentStart, Vec2 const& SegmentEnd, ProjectionInfo& Out);

namespace Evader
{
	static SafePath IsPathSafe(std::vector<Vec2> paths, int time, int speed = -1, int delay = 0)
	{
		auto isSafe			= true;
		auto intersects		= std::vector<Intersects>();

		for (auto spell : Evade::Spells)
		{
			auto checkPath = spell->IsSafePath(paths, time, speed, delay);

			isSafe = isSafe && checkPath.IsSafe;

			if (checkPath.Intersect.Valid)
				intersects.push_back(checkPath.Intersect);
		}

		if (intersects.size() > 0)
		{
			std::sort(intersects.begin(), intersects.end(), [](Intersects First, Intersects Second)
			{
				if (First.Distance < Second.Distance)
					return true;

				return false;
			});

			return SafePath(isSafe, isSafe ? Intersects() : intersects.front());
		}

		return SafePath(isSafe, Intersects());
	}

	static SafePath IsPathSafe(std::vector<Vec3> paths, int time, int speed = -1, int delay = 0)
	{
		std::vector<Vec2> vecPaths;

		for (auto i : paths)
			vecPaths.push_back(i.To2D());

		return IsPathSafe(vecPaths, time, speed, delay);
	}

	static SafePath IsPathSafe(Vec2 const& Position, int Time, int Speed = -1, int Delay = 0)
	{
		std::vector<Vec2> vecPath;
		GEntityList->Player()->CreatePath2D(Position, vecPath);

		return IsPathSafe(vecPath, Time, Speed, Delay);
	}

	static SafePoint IsPointSafe(Vec2 const& Position)
	{
		std::vector<SpellInstance*> vecSpells;

		for (auto i : Evade::Spells)
		{
			if (!i->IsSafePoint(Position))
				vecSpells.push_back(i);
		}

		return SafePoint(vecSpells);
	}

	static Vec2 GetFastestPoint(Vec2 pos, SpellInstance* spell)
	{
		ProjectionInfo piout;

		switch (spell->Type)
		{
		case ST_Line:
		case ST_MissileLine:
			ProjectPointOn(pos, spell->Line.Start, spell->Line.End, piout);

			if (piout.IsOnSegment)
				return piout.SegmentPoint.Extend(pos, spell->Line.Radius + 10);

			break;
		case ST_Circle:
			return spell->Circle.Center.Extend(pos, spell->Circle.Radius + 10);
		}

		return Vec2(0.f, 0.f);
	}

	static std::vector<Vec2> GetFastestPoints(Vec2 pos)
	{
		std::vector<Vec2> vecOut;

		for (auto i : Evade::Spells)
		{
			Vec2 j = GetFastestPoint(pos, i);

			if (j.Length() != 0.f)
				vecOut.push_back(j);
		}

		return vecOut;
	}

	static Vec2 CheckClose(Vec2 In)
	{
		if (!GEntityList->Player()->IsMoving() && Evade::PlayerPosition.DistanceTo(In) <= 75)
			return Evade::PlayerPosition;

		return In;
	}

	static bool CheckPath(Vec2 pos)
	{
		std::vector<Vec2> paths;
		if (!GEntityList->Player()->CreatePath2D(pos, paths))
			return false;

		return paths.size() > 0 && (pos.DistanceTo(paths[paths.size() - 1]) > 5 || paths.size() > 2);
	}

	static Vec2 GetExtendedSafePoint(Vec2 to, Vec2 from)
	{
		auto dir = (to - from).VectorNormalize();
		auto pos = to;

		for (float i = 50.f; i <= 100.f; i += 50.f)
		{
			auto newPos = to + dir * i;

			if (CheckPath(newPos))
				return pos;

			pos = newPos;
		}

		return pos;
	}

	static bool IsWallBetween(Vec2 to, Vec2 from)
	{
		auto dir = (to - from).VectorNormalize();

		for (auto i = 0.f; i <= 1.f; i += 0.1f)
		{
			Vec2 vecPoint = from + i * dir;

			if (GPrediction->IsPointWall(Util::Get3DPoint(vecPoint)))
				return true;
		}

		return false;
	}

	static bool UnderTurret(Vec2 const& Position)
	{
		for (auto i : GEntityList->GetAllTurrets(false, true))
		{
			if (i->IsDead())
				continue;

			if (i->GetPosition().To2D().DistanceTo(Position) < 950.f)
				return true;
		}

		return false;
	}

	static int CountEnemiesInRange(Vec2 const& Position, float Range)
	{
		int iCount = 0;

		for (auto i : GEntityList->GetAllHeros(false, true))
		{
			if (!i->IsValidTarget())
				continue;

			if (i->GetPosition().To2D().DistanceTo(Position) <= Range)
				iCount++;
		}

		return iCount;
	}

	static Vec2 GetBestPoint(Vec2 pos, int speed = -1, int delay = 0)
	{
		speed = speed == -1 ? static_cast<int>(GEntityList->Player()->MovementSpeed()) : speed;
		auto posChecked = 0;
		auto radiusIndex = 0;
		auto myPos = Evade::PlayerPosition;

		std::vector<Vec2> points = GetFastestPoints(myPos);

		for (auto& i : points)
			i = CheckClose(i);

		while (posChecked < 150)
		{
			radiusIndex++;
			auto curRadius = static_cast<float>(radiusIndex) * 50.f;
			auto curCircleChecks = static_cast<int>(ceil(2.f * 3.141592 * curRadius / 50.f));

			for (auto i = 1; i < curCircleChecks; i++)
			{
				posChecked++;
				auto cRadians = 2.f * 3.141592 / static_cast<float>(curCircleChecks - 1) * i;

				points.push_back(CheckClose(Vec2(floor(myPos.x + curRadius * cosf(cRadians)), floor(myPos.y + curRadius * sinf(cRadians)))));
			}
		}

		points.erase(std::remove_if(points.begin(), points.end(), [](Vec2 Args)
		{
			return (CheckPath(Args));
		}), points.end());

		std::vector<Vec2> vecExtendedSafePoints;

		for (auto i : points)
		{
			Vec2 vecPoint = GetExtendedSafePoint(i, myPos);

			if (IsPathSafe(vecPoint, Configs->EvadingFirstTime, speed, delay).IsSafe || IsPathSafe(vecPoint, Configs->EvadingSecondTime, speed, delay).IsSafe)
			{
				if (!IsWallBetween(vecPoint, myPos))
					vecExtendedSafePoints.push_back(vecPoint);
			}
		}

		if (vecExtendedSafePoints.size() == 0.f)
			return Vec2(0.f, 0.f);

		std::sort(vecExtendedSafePoints.begin(), vecExtendedSafePoints.end(), [&](Vec2 First, Vec2 Second)
		{
			if (!UnderTurret(First) && UnderTurret(Second))
				return true;
			else if (CountEnemiesInRange(First, 200.f) < CountEnemiesInRange(Second, 200.f))
				return true;
			else if (First.DistanceTo(pos) < Second.DistanceTo(pos))
				return true;

			return false;
		});

		return vecExtendedSafePoints.front();
	}

	static Vec2 GetBestPointBlock(Vec2 pos)
	{
		auto posChecked = 0;
		auto radiusIndex = 0;
		auto myPos = Evade::PlayerPosition;
		auto points = std::vector<Vec2>();

		while (posChecked < 50)
		{
			radiusIndex++;
			auto curRadius = static_cast<float>(radiusIndex * 100);
			auto curCircleChecks = static_cast<int>(ceil(2.f * 3.141592f * curRadius / 100.f));

			for (auto i = 1; i < curCircleChecks; i++)
			{
				posChecked++;
				auto cRadians = 2.f * 3.141592f / static_cast<float>((curCircleChecks - 1)) * static_cast<float>(i);

				points.push_back(CheckClose(Vec2(floor(myPos.x + curRadius * cos(cRadians)), myPos.y + curRadius * sin(cRadians))));
			}
		}

		if (points.size() == 0)
			return Vec2(0.f, 0.f);

		std::vector<Vec2> vecFinalPoints;

		for (auto i : points)
		{
			if (!CheckPath(i) && IsPathSafe(i, Configs->EvadingFirstTime).IsSafe && !IsWallBetween(i, myPos))
				vecFinalPoints.push_back(i);
		}

		if (vecFinalPoints.size() == 0)
			return Vec2(0.f, 0.f);

		std::sort(vecFinalPoints.begin(), vecFinalPoints.end(), [&](Vec2 First, Vec2 Second)
		{
			if (!UnderTurret(First) && UnderTurret(Second))
				return true;
			else if (CountEnemiesInRange(First, 200.f) < CountEnemiesInRange(Second, 200.f))
				return true;
			else if (First.DistanceTo(pos) < Second.DistanceTo(pos))
				return true;

			return false;
		});

		return vecFinalPoints.front();
	}

	static bool IsPointBlinkSafe(Vec2 pos, int time, int delay)
	{
		for (auto i : Evade::Spells)
		{
			if (i->IsSafeToBlink(pos, time, delay))
				return true;
		}

		return false;
	}

	static IUnit* GetBestTarget(Vec2 pos, EvadeSpellData* evadeSpell)
	{
		auto myPos = Util::Get3DPoint(Evade::PlayerPosition);
		auto targets = std::vector<IUnit*>();
		auto result = std::vector<std::pair<IUnit*, Vec2>>();
		auto delay = evadeSpell->IsInvulnerability ? 0 : evadeSpell->Delay;
		auto type = evadeSpell->ValidTargets;

		if (type & VT_AllyChampions)
		{
			for (auto i : GEntityList->GetAllHeros(true, false))
			{
				if (i == GEntityList->Player() || !GEntityList->Player()->IsValidTarget(i, FLT_MAX))
					continue;

				if (myPos.DistanceTo(i->GetPosition()) < evadeSpell->MaxRange)
					targets.push_back(i);
			}
		}

		if (type & VT_AllyMinions)
		{
			for (auto i : GEntityList->GetAllMinions(true, false, false))
			{
				if (!GEntityList->Player()->IsValidTarget(i, FLT_MAX) || i->IsWard())
					continue;

				if (myPos.DistanceTo(i->GetPosition()) < evadeSpell->MaxRange)
					targets.push_back(i);
			}
		}

		if (type & VT_AllyWards)
		{
			for (auto i : GEntityList->GetAllMinions(true, false, false))
			{
				if (!GEntityList->Player()->IsValidTarget(i, FLT_MAX) || !i->IsWard())
					continue;

				if (myPos.DistanceTo(i->GetPosition()) < evadeSpell->MaxRange)
					targets.push_back(i);
			}
		}

		if (type & VT_EnemyChampions)
		{
			for (auto i : GEntityList->GetAllHeros(false, true))
			{
				if (GEntityList->Player()->IsValidTarget(i, FLT_MAX) && myPos.DistanceTo(i->GetPosition()) < evadeSpell->MaxRange)
					targets.push_back(i);
			}
		}

		if (type & VT_EnemyMinions)
		{
			for (auto i : GEntityList->GetAllMinions(false, true, true))
			{
				if (GEntityList->Player()->IsValidTarget(i, FLT_MAX) && !i->IsWard() && myPos.DistanceTo(i->GetPosition()) < evadeSpell->MaxRange)
					targets.push_back(i);
			}
		}

		if (type & VT_EnemyWards)
		{
			for (auto i : GEntityList->GetAllMinions(false, true, true))
			{
				if (!GEntityList->Player()->IsValidTarget(i, FLT_MAX) || !i->IsWard())
					continue;

				if (myPos.DistanceTo(i->GetPosition()) < evadeSpell->MaxRange)
					targets.push_back(i);
			}
		}

		for (auto target : targets)
		{
			auto end = target->ServerPosition().To2D();

			if (evadeSpell->MenuName == "YasuoE" && target->HasBuff("YasuoDashWrapper"))
				continue;

			if (evadeSpell->FixedRange)
				end = myPos.To2D().Extend(end, evadeSpell->MaxRange);

			if (!evadeSpell->IsInvulnerability && !IsPointSafe(end).IsSafe)
				continue;
			
			auto canAdd = GGame->TickCount() - Evade::LastWardJumpTick < 250
				|| (evadeSpell->IsDash
				? IsPathSafe(end, Configs->EvadingFirstTime, evadeSpell->Speed, delay).IsSafe
				|| IsPathSafe(end, Configs->EvadingSecondTime, evadeSpell->Speed, delay).IsSafe
				: IsPointBlinkSafe(end, Configs->EvadingFirstTime, delay)
				|| IsPointBlinkSafe(end, Configs->EvadingSecondTime, delay));

			if (canAdd)
				result.push_back(std::pair<IUnit*, Vec2>(target, end));
		}

		if (result.size() == 0)
			return nullptr;

		std::sort(result.begin(), result.end(), [&](std::pair<IUnit*, Vec2> First, std::pair<IUnit*, Vec2> Second)
		{
			if (!UnderTurret(First.second) && UnderTurret(Second.second))
				return true;
			else if (CountEnemiesInRange(First.second, 200.f) < CountEnemiesInRange(Second.second, 200.f))
				return true;
			else if (pos.DistanceTo(First.second) < pos.DistanceTo(Second.second))
				return true;

			return false;
		});

		return result.front().first;
	}

	static Vec2 GetBestPointDash(Vec2 pos, int speed, int delay, float range, bool fixedRange = false)
	{
		auto posChecked = 0;
		auto radiusIndex = 0;
		auto myPos = Evade::PlayerPosition;
		auto points = std::vector<Vec2>();
		auto minDist = fixedRange ? range : 50;

		while (posChecked < 100)
		{
			radiusIndex++;
			auto curRadius = radiusIndex * 100 + (minDist - 100);
			auto curCircleChecks = static_cast<int>(ceil(2.f * 3.141592 * curRadius / 100.f));

			for (auto i = 1; i < curCircleChecks; i++)
			{
				posChecked++;
				auto cRadians = 2 * 3.141592f / static_cast<float>((curCircleChecks - 1)) * i;

				points.push_back(CheckClose(Vec2(floorf(myPos.x + curRadius * cosf(cRadians)), floorf(myPos.y + curRadius * sinf(cRadians)))));
			}

			if (curRadius >= range)
				break;
		}

		std::vector<Vec2> vecFinalPoints;

		for (auto i : points)
		{
			if (!CheckPath(i)
				&& (IsPathSafe(i, Configs->EvadingFirstTime, speed, delay).IsSafe
				|| IsPathSafe(i, Configs->EvadingSecondTime, speed, delay).IsSafe) && !GPrediction->IsPointWall(Util::Get3DPoint(i)))
			{
				vecFinalPoints.push_back(i);
			}
		}

		if (vecFinalPoints.size() == 0)
			return Vec2(0.f, 0.f);

		std::sort(vecFinalPoints.begin(), vecFinalPoints.end(), [&](Vec2 First, Vec2 Second)
		{
			if (!UnderTurret(First) && UnderTurret(Second))
				return true;
			else if (CountEnemiesInRange(First, 200.f) < CountEnemiesInRange(Second, 200.f))
				return true;
			else if (First.DistanceTo(pos) < Second.DistanceTo(pos))
				return true;

			return false;
		});

		return vecFinalPoints.front();
	}

	static Vec2 GetBestPointBlink(Vec2 pos, int delay, float range)
	{
		auto posChecked = 0;
		auto radiusIndex = 0;
		auto myPos = Evade::PlayerPosition;
		auto points = std::vector<Vec2>();

		while (posChecked < 100)
		{
			radiusIndex++;
			auto curRadius = radiusIndex * 100;
			auto curCircleChecks = static_cast<int>(ceil(2.f * 3.141592 * curRadius / 100.f));

			for (auto i = 1; i < curCircleChecks; i++)
			{
				posChecked++;
				auto cRadians = 2 * 3.141592f / static_cast<float>((curCircleChecks - 1)) * i;

				points.push_back(CheckClose(Vec2(floorf(myPos.x + curRadius * cosf(cRadians)), floorf(myPos.y + curRadius * sinf(cRadians)))));
			}

			if (curRadius >= range)
				break;
		}

		std::vector<Vec2> vecFinalPoints;

		for (auto i : points)
		{
			if (!CheckPath(i)
				&& (IsPointBlinkSafe(i, Configs->EvadingFirstTime, delay)
				|| IsPointBlinkSafe(i, Configs->EvadingSecondTime, delay)) && !GPrediction->IsPointWall(Util::Get3DPoint(i)))
			{
				vecFinalPoints.push_back(i);
			}
		}

		if (vecFinalPoints.size() == 0)
			return Vec2(0.f, 0.f);

		std::sort(vecFinalPoints.begin(), vecFinalPoints.end(), [&](Vec2 First, Vec2 Second)
		{
			if (!UnderTurret(First) && UnderTurret(Second))
				return true;
			else if (CountEnemiesInRange(First, 200.f) < CountEnemiesInRange(Second, 200.f))
				return true;
			else if (First.DistanceTo(pos) < Second.DistanceTo(pos))
				return true;

			return false;
		});

		return vecFinalPoints.front();
	}
}

#endif // Evader_h__
