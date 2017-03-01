#ifndef Collision_h__
#define Collision_h__

#include "PluginSDK.h"
#include "SpellInstance.h"
#include "ObjManager.h"

#undef min
#undef max

#include <algorithm>

void ProjectPointOn(Vec2 const& Point, Vec2 const& SegmentStart, Vec2 const& SegmentEnd, ProjectionInfo& Out);;

namespace Collisions
{
	static bool HasYasuo;
	static Vec2 YasuoWallPos;
	static int YasuoWallTick;

	struct Prediction
	{
		Prediction(bool _IsMoving, Vec2 _Position)
		{
			IsMoving = _IsMoving;
			Position = _Position;
		}

		bool IsMoving;
		Vec2 Position;
	};

	static void Init()
	{
		HasYasuo = false;

		for (auto i : GEntityList->GetAllHeros(true, false))
		{
			if (std::string(i->ChampionName()) == "Yasuo")
			{
				HasYasuo = true;
				return;
			}
		}
	}

	static void CutPath(std::vector<Vec3> const& InPath, std::vector<Vec3>& OutPath, float Distance)
	{
		float flDistance = Distance;

		if (flDistance < 0.f)
		{
			OutPath = InPath;
			OutPath[0] = OutPath[0] + Distance * (OutPath[1] - OutPath[0]).VectorNormalize();

			return;
		}

		OutPath.clear();

		for (auto i = 0; i < InPath.size() - 1; i++)
		{
			Vec3 vecIn1 = InPath[i];
			Vec3 vecIn2 = InPath[i + 1];

			float flDist = vecIn1.Distance2D(vecIn2);

			if (flDist > Distance)
			{
				OutPath.push_back(InPath[i] + Distance * (InPath[i + 1] - InPath[i]).VectorNormalize());

				for (auto j = i + 1; j < InPath.size(); j++)
					OutPath.push_back(InPath[j]);

				break;
			}

			Distance -= flDist;
		}

		if (OutPath.size() == 0)
			OutPath.push_back(InPath.back());
	}

	static Prediction FastPrediction(Vec2 from, IUnit* unit, int delay, int speed)
	{
		auto GetWaypointLength = [](std::vector<Vec3> Path)
		{
			float flPathLength = 0.f;

			if (Path.size() > 1)
			{
				for (auto i = 0; i < Path.size() - 1; i++)
					flPathLength += Path[i].Distance2D(Path[i + 1]);
			}

			return flPathLength;
		};

		auto d = (delay / 1000 + (speed == 0 ? 0 : from.DistanceTo(unit->ServerPosition().To2D()) / speed)) * unit->MovementSpeed();
		auto path = unit->GetWaypointList();

		if (GetWaypointLength(path) > d)
		{
			std::vector<Vec3> vecCutPath;
			CutPath(path, vecCutPath, d);

			return Prediction(true, vecCutPath[0].To2D());
		}

		return Prediction(false, path[path.size() - 1].To2D());
	}

	static Vec2 GetCollision(SpellInstance* spell)
	{
		auto result = std::vector<Vec2>();
		auto curPos = spell->GetMissilePosition(0);
		spell->ForceDisabled = false;

		int iCollisionFlags = spell->Data.CollisionObjects;

		if (iCollisionFlags & kCollidesWithMinions)
		{
			for (auto i : GEntityList->GetAllMinions(true, false, true))
			{
				if (!i->IsValidTarget())
					continue;

				float flDistanceToPos = (curPos - i->ServerPosition().To2D()).Length();

				if (flDistanceToPos > 1200.f)
					continue;

				if (ObjManager->IsJungle(i) || (i->GetTeam() == GEntityList->Player()->GetTeam() && (ObjManager->IsMinion(i) || ObjManager->IsPet(i))))
				{
					auto pred = FastPrediction(curPos, i, std::max(0, spell->Data.Delay - (GGame->TickCount() - spell->StartTick)), spell->Data.MissileSpeed);

					if (spell->Data.RawRadius + (!pred.IsMoving ? i->BoundingRadius() - 15 : 0) - pred.Position.Distance(curPos, spell->End, true) > 0)
					{
						ProjectionInfo piout;
						ProjectPointOn(pred.Position, spell->End, spell->Start, piout);

						result.push_back(piout.LinePoint + spell->Direction * 30);
					}
				}
			}
		}

		if (iCollisionFlags & kCollidesWithHeroes)
		{
			for (auto i : GEntityList->GetAllHeros(true, false))
			{
				if (i == GEntityList->Player() || !GEntityList->Player()->IsValidTarget(i, 1200.f))
					continue;

				Vec2 pos = FastPrediction(curPos, i, std::max(0, spell->Data.Delay - (GGame->TickCount() - spell->StartTick)), spell->Data.MissileSpeed).Position;

				if (spell->Data.RawRadius + 30 - pos.Distance(curPos, spell->End, true) > 0)
				{
					ProjectionInfo piout;
					ProjectPointOn(pos, spell->End, spell->Start, piout);

					result.push_back(piout.LinePoint + spell->Direction * 30);
				}
			}
		}

		if (iCollisionFlags & kCollidesWithYasuoWall && HasYasuo && spell->Data.MissileMaxSpeed != 0)
		{
// 			std::regex szRegExWall("Yasuo_.+_W_windwall.\\.troy");
// 			IUnit* pWall = nullptr;
// 
// 			for (auto i : GEntityList->GetAllUnits())
// 			{
// 
// 			}
// 			if (!haveYasuo || spell.Data.MissileSpeed == 0)
// 			{
// 				continue;
// 			}
// 
// 			var wall =
// 				ObjectManager.Get<Obj_GeneralParticleEmitter>()
// 				.FirstOrDefault(
// 				i = > i.IsValid && new Regex("Yasuo_.+_W_windwall.\\.troy").IsMatch(i.Name));
// 
// 			if (wall == null)
// 			{
// 				continue;
// 			}
// 
// 			var wallWidth = 300 + 50 * Convert.ToInt32(wall.Name.Substring(wall.Name.Length - 6, 1));
// 			var wallDirection = (wall.Position.To2D() - yasuoWallPos).Normalized().Perpendicular();
// 			var wallStart = wall.Position.To2D() + wallWidth / 2f * wallDirection;
// 			var wallEnd = wallStart - wallWidth * wallDirection;
// 			var wallPolygon = new Geometry.Polygon.Rectangle(wallStart, wallEnd, 75);
// 			var intersects = wallPolygon.GetIntersectPointsLine(curPos, spell.End);
// 
// 			if (intersects.Count > 0)
// 			{
// 				var intersect = intersects.OrderBy(i = > i.Distance(curPos)).First();
// 				var time = Utils.GameTimeTickCount
// 					+ Math.Max(0, spell.Data.Delay - (Utils.GameTimeTickCount - spell.StartTick))
// 					+ 100 + intersect.Distance(curPos) / spell.Data.MissileSpeed * 1000;
// 
// 				if (time - yasuoWallTick < 4000)
// 				{
// 					if (spell.Type != SpellType.MissileLine)
// 					{
// 						spell.ForceDisabled = true;
// 					}
// 
// 					return intersect;
// 				}
// 			}
// 			break
		}

		if (result.size() > 0)
		{
			std::sort(result.begin(), result.end(), [](Vec2 First, Vec2 Second)
			{
				float flDistance1 = First.DistanceTo(GGame->CursorPosition().To2D());
				float flDistance2 = Second.DistanceTo(GGame->CursorPosition().To2D());

				return (flDistance2 > flDistance1);
			});
		}

		return Vec2(0.f, 0.f);
	}

	static void OnProcessSpell(CastedSpell const& Args)
	{
		if (Args.Caster_ == nullptr || !Args.Caster_->IsValidObject() || std::string(Args.Name_) != "YasuoWMovingWall")
			return;

		YasuoWallTick = GGame->TickCount();
		YasuoWallPos = Args.Caster_->ServerPosition().To2D();
	}
};

#endif // Collision_h__
