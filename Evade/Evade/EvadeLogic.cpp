#include "EvadeLogic.h"
#include "Evade.h"
#include "Evader.h"
#include "Core.h"
#include "EvadeSpellDB.h"

CEvadeLogic* EvadeLogic = nullptr;

void CEvadeLogic::OnGameUpdate()
{
	CastDelayedSpells();

	Evade::PlayerPosition = GEntityList->Player()->ServerPosition().To2D();

	if (PrevPos.IsValid() && Evade::PlayerPosition.DistanceTo(PrevPos) > 200.f)
	{
		Evade::SetEvading(false);
		Evade::EvadePoint2 = Vec2(0.f, 0.f);
	}

	PrevPos = Evade::PlayerPosition;

	Evade::UpdateSpells();

	// Evasion disabled or not needed
	if (!Configs->Enabled->Enabled() || Util::CommonCheck())
	{
		Evade::SetEvading(false);
		Evade::EvadePoint2		= Vec2(0.f, 0.f);
		return;
	}

	// Check for winding up and last spell cast wasn't AA
	// ...

	// Attempt to shield allies if they're about to get hit by a skill shot
	AttemptToShieldAlly();

	// Position we want our evader to reach
	EvadeToPoint = Evade::EvadePoint2.IsValid() ? Evade::EvadePoint2 : GGame->CursorPosition().To2D();

	auto curPaths = GEntityList->Player()->GetWaypointList();
	auto checkPos = Evader::IsPointSafe(Evade::PlayerPosition);
	auto checkPath = Evader::IsPathSafe(curPaths, 100);

	Evade::HaveSolution = false;

	if (Evade::Evading)
	{
		if (Evader::IsPointSafe(Evade::EvadePoint1).IsSafe)
		{
			if (checkPos.IsSafe)
			{
				Evade::SetEvading(false);
			}
			else
			{
				if (GGame->TickCount() - Evade::LastMoveTick1 > 67)
				{
					Evade::LastMoveTick1 = GGame->TickCount();
					Util::Move(Evade::EvadePoint1);
				}

				return;
			}
		}
		else
		{
			Evade::SetEvading(false);
		}
	}

	// Check if we're inside a skill shot and, if we are, start moving outside of it
	if (!checkPos.IsSafe && !checkPath.IsSafe)
		RunEvasionSpells(checkPos.Spells);

	// Also need to check PathFollow
	if (Evade::HaveSolution || Evade::Evading || !Evade::EvadePoint2.IsValid() || !checkPos.IsSafe || !HasWalkingSpell() ||
		(checkPath.IsSafe && !Evade::ForceFollowPath) || 
		(/*IsFollowingPath() && */GGame->TickCount() - Evade::LastMoveTick2 <= 67))
	{
		return;
	}

	Evade::LastMoveTick2 = GGame->TickCount();

	if (Evade::DetectedSpells.size() == 0)
	{
		if (Evade::EvadePoint2.DistanceTo(Evade::PlayerPosition) > 75)
			Util::Move(Evade::EvadePoint2);

		return;
	}

	std::vector<Vec2> paths;
	GEntityList->Player()->CreatePath2D(Evade::EvadePoint2, paths);

	if (Evader::IsPathSafe(paths, 100).IsSafe)
	{
		if (Evade::EvadePoint2.DistanceTo(Evade::PlayerPosition) > 75)
			Util::Move(Evade::EvadePoint2);

		return;
	}

	// if (newPaths.size() == 0)
// 	{
// 		if (!checkPath.Intersect.Valid && curPaths.size() == 0)
// 			checkPath = Evader::IsPathSafe(paths, 100);
// 
// 		if (checkPath.Intersect.Valid && checkPath.Intersect.Point.DistanceTo(Evade::PlayerPosition) > 75)
// 		{
// 			Util::Move(checkPath.Intersect.Point);
// 			return;
// 		}
//	}
	
	// PathFollow->Start(newPaths);
	// PathFollow->KeepFollowPath();
}

void CEvadeLogic::OnRender()
{
	// Should draw path we're taking here...
}

bool CEvadeLogic::OnIssueOrder(IUnit* Source, DWORD OrderIdx, Vec3* Position, IUnit* Target)
{
	Evade::EvadePoint2 = (OrderIdx == kMoveTo || OrderIdx == kAttackTo) ? Position->To2D() : Vec2(0.f, 0.f);

	if (Evade::DetectedSpells.size() == 0)
		Evade::ForceFollowPath = false;

	if (Evade::HaveSolution)
		return true;

	if (!Configs->Enabled->Enabled() || !HasWalkingSpell() || Util::ShieldCheck())
		return true;

	std::vector<Vec2> paths;
	GEntityList->Player()->CreatePath2D(Position->To2D(), paths);

	if (Evade::Evading || !Evader::IsPointSafe(Evade::PlayerPosition).IsSafe)
	{
		if (OrderIdx == kMoveTo)
		{
			auto willMove = false;

			if (Evade::Evading && GGame->TickCount() - Evade::LastEvadePointChangeTick > Configs->EvadePointChangeTime)
			{
				auto point = Evader::GetBestPointBlock(Position->To2D());

				if (point.IsValid())
				{
					Evade::EvadePoint1 = point;
					Evade::SetEvading(true);
					willMove = true;
					Evade::LastEvadePointChangeTick = GGame->TickCount();
				}
			}

			if (Evader::IsPathSafe(paths, Configs->EvadingRouteChangeTime).IsSafe && Evader::IsPointSafe(paths.back()).IsSafe)
			{
				Evade::EvadePoint1 = paths.back();
				Evade::SetEvading(true);
				willMove = true;
			}

			if (!willMove)
				Evade::ForceFollowPath = true;
		}

		return false;
	}

	auto checkPath = Evader::IsPathSafe(paths, Configs->CrossingTime);

	if (checkPath.IsSafe)
		return true;

	if (OrderIdx != kAttackUnit)
	{
		Evade::ForceFollowPath = true;
		return false;
	}
	else
	{
		auto target = Target;

		if (target == nullptr || !target->IsValidObject() || !target->IsVisible() || Evade::PlayerPosition.DistanceTo(target->GetPosition().To2D()) <= GOrbwalking->GetAutoAttackRange(target))
			return true;

		if (checkPath.Intersect.Valid)
			Util::Move(checkPath.Intersect.Point);

		return false;
	}

	return true;
}

void CEvadeLogic::CastDelayedSpells()
{
	for (auto i = DelayedSpells.begin(); i != DelayedSpells.end();)
	{
		auto j = (*i);

		if (GGame->TickCount() > j.TimeToCast)
		{
			CastSpell(j.Slot, j.Target, Util::Get3DPoint(j.Point));
			i = DelayedSpells.erase(i);
		}
		else
		{
			i++;
		}
	}
}

bool CEvadeLogic::IsInsideSkillshot(Vec2& ClosestOutsidePoint)
{
	if (Evader::IsPathSafe(GEntityList->Player()->GetWaypointList(), 100).IsSafe && Evader::IsPointSafe(Evade::PlayerPosition).IsSafe)
		return false;

	return true;

	ClosestOutsidePoint = GEntityList->Player()->GetPosition().To2D();

	auto outerPolys = std::vector<Geometry::IPolygon>();

	for (auto i : Evade::Spells)
		outerPolys.push_back(i->PathFindingOuterPolygon);

	bool bInside = false;

	auto outerPolygons = Geometry::ToPolygons(Geometry::ClipPolygons(outerPolys));

	for (auto i : outerPolygons)
	{
		if (i.IsInside(ClosestOutsidePoint))
		{
			bInside = true;
			ClosestOutsidePoint = Core::GetClosestOutsidePoint(ClosestOutsidePoint, outerPolygons);
		}
	}

	return bInside;
}

int CEvadeLogic::GetMaxDangerLevel(std::vector<SpellInstance *> SpellsList)
{
	auto iMaxDangerLevel = 0;

	for (auto i : SpellsList)
	{
		auto iDanger = i->GetDangerLevel();

		if (iDanger > iMaxDangerLevel)
			iMaxDangerLevel = iDanger;
	}

	return iMaxDangerLevel;
}

void CEvadeLogic::AttemptToShieldAlly()
{
	for (auto i : GEntityList->GetAllHeros(true, false))
	{
		if (i == GEntityList->Player() || !GEntityList->Player()->IsValidTarget(i, 1000.f))
			continue;

		auto checkSafe = Evader::IsPointSafe(i->ServerPosition().To2D());

		if (checkSafe.IsSafe)
			continue;

		auto iMaxDangerLevel = GetMaxDangerLevel(checkSafe.Spells);

		for (auto j : EvadeSpellsDB->Spells)
		{
			if (j->IsReady() && j->IsShield && j->CanShieldAllies && iMaxDangerLevel >= j->GetDangerLevel())
			{
				if (i->ServerPosition().To2D().DistanceTo(Evade::PlayerPosition) < j->MaxRange && Evade::IsAboutToHit(j->Delay, i))
				{
					if (CastSpell(j->Slot, i))
						return;
				}
			}
		}
	}
}

void CEvadeLogic::RunEvasionSpells(SafePoint Point)
{
	auto iHighestDangerLevel = GetMaxDangerLevel(Point.Spells);
	
	for (auto i : EvadeSpellsDB->Spells)
	{
		if (i->IsEnabled() && iHighestDangerLevel >= i->GetDangerLevel())
		{
			if (i->MenuName == "Walking" && WalkEvade())
				return;

			EvadeSpell = i;

			if (i->IsReady())
			{
				if (i->IsSpellShield && SpellShieldEvade())
					return;

				if (i->IsMovementSpeedBuff && MovementSpeedEvade())
					return;

				if (i->IsDash && DashEvade())
					return;

				if (i->IsBlink && BlinkEvade())
					return;

				if (i->IsInvulnerability && InvulnerableEvade())
					return;
			}

			if (ItemEvade())
				return;

			if (i->IsReady() && i->IsShield && ShieldEvade())
				return;
		}
	}
}

bool CEvadeLogic::WalkEvade()
{
	auto point = Evader::GetBestPoint(EvadeToPoint);

	if (point.IsValid())
	{
		Vec2 vecExtended = point.Extend(Evade::PlayerPosition, -100.f);

		if (Evader::IsPathSafe(point, Configs->EvadingSecondTime, -1, 100).IsSafe)
			point = vecExtended;

		Evade::EvadePoint1 = point;
		Evade::SetEvading(true);

		return true;
	}

	return false;
}

bool CEvadeLogic::SpellShieldEvade()
{
	if (Evade::IsAboutToHit(EvadeSpell->Delay))
		CastSpell(EvadeSpell->Slot);

	Evade::HaveSolution = true;

	return true;
}

bool CEvadeLogic::MovementSpeedEvade()
{
	auto point = Evader::GetBestPoint(EvadeToPoint, static_cast<int>(EvadeSpell->MoveSpeedTotalAmount()), EvadeSpell->Delay);

	if (point.IsValid())
	{
		Evade::EvadePoint1 = point;
		Evade::SetEvading(true);
		CastSpell(EvadeSpell->Slot);

		return true;
	}

	return false;
}

bool CEvadeLogic::DashEvade()
{
	if (EvadeSpell->IsTargetted())
	{
		auto target = Evader::GetBestTarget(EvadeToPoint, EvadeSpell);

		if (target != nullptr)
		{
			Evade::EvadePoint1 = target->ServerPosition().To2D();
			Evade::SetEvading(true);
			CastSpell(EvadeSpell->Slot, target);

			return true;
		}

		if (GGame->TickCount() - Evade::LastWardJumpTick < 250)
		{
			Evade::HaveSolution = true;
			return true;
		}

		auto pOption = Configs->GetOptionForSpellName(std::string("ES_") + EvadeSpell->MenuName + "_WardJump");

		if (EvadeSpell->ValidTargets & VT_AllyWards && pOption != nullptr && pOption->Enabled())
		{
			auto iWardSlot = Util::GetWardSlot();

			if (iWardSlot != kSlotUnknown)
			{
				auto point = Evader::GetBestPointDash(EvadeToPoint, EvadeSpell->Speed, EvadeSpell->Delay, 600);

				if (point.IsValid())
				{
					CastSpell(iWardSlot, nullptr, Util::Get3DPoint(point));
					Evade::LastWardJumpTick = GGame->TickCount();
					Evade::HaveSolution = true;

					return true;
				}
			}
		}
	}
	else
	{
		auto point = Evader::GetBestPointDash(
			EvadeToPoint,
			EvadeSpell->Speed,
			EvadeSpell->Delay,
			EvadeSpell->MaxRange,
			EvadeSpell->FixedRange);

		if (point.IsValid())
		{
			Evade::EvadePoint1 = point;
			Evade::SetEvading(true);

			if (!EvadeSpell->Invert)
			{
				if (EvadeSpell->RequiresPreMove)
				{
					Util::Move(Evade::EvadePoint1);

					DelayedCastSpell dc;
					dc.Slot = EvadeSpell->Slot;
					dc.Point = Evade::EvadePoint1;
					dc.Target = nullptr;
					dc.TimeToCast = GGame->TickCount() + (GGame->Latency() / 2 + 100);

					DelayedSpells.push_back(dc);
				}
				else
				{
					CastSpell(EvadeSpell->Slot, nullptr, Util::Get3DPoint(Evade::EvadePoint1));
				}
			}
			else
			{
				CastSpell(EvadeSpell->Slot, nullptr, Util::Get3DPoint((Evade::PlayerPosition - (Evade::EvadePoint1 - Evade::PlayerPosition))));
			}

			return true;
		}
	}

	return false;
}

bool CEvadeLogic::BlinkEvade()
{
	if (EvadeSpell->IsTargetted())
	{
		auto target = Evader::GetBestTarget(EvadeToPoint, EvadeSpell);

		if (target != nullptr)
		{
			if (Evade::IsAboutToHit(EvadeSpell->Delay))
			{
				Evade::EvadePoint1 = target->ServerPosition().To2D();
				Evade::SetEvading(true);
				CastSpell(EvadeSpell->Slot, target);
			}

			Evade::HaveSolution = true;

			return true;
		}

		if (GGame->TickCount() - Evade::LastWardJumpTick < 250)
		{
			Evade::HaveSolution = true;
			return true;
		}

		auto pOption = Configs->GetOptionForSpellName(std::string("ES_") + EvadeSpell->MenuName + "_WardJump");

		if (EvadeSpell->ValidTargets & VT_AllyWards && pOption != nullptr && pOption->Enabled())
		{
			auto ward = Util::GetWardSlot();

			if (ward != kSlotUnknown)
			{
				auto point = Evader::GetBestPointBlink(EvadeToPoint, EvadeSpell->Delay, 600);

				if (point.IsValid())
				{
					CastSpell(ward, nullptr, Util::Get3DPoint(point));
					Evade::LastWardJumpTick = GGame->TickCount();
					Evade::HaveSolution = true;

					return true;
				}
			}
		}
	}
	else
	{
		auto point = Evader::GetBestPointBlink(EvadeToPoint, EvadeSpell->Delay, EvadeSpell->MaxRange);

		if (point.IsValid())
		{
			if (Evade::IsAboutToHit(EvadeSpell->Delay))
			{
				Evade::EvadePoint1 = point;
				Evade::SetEvading(true);
				CastSpell(EvadeSpell->Slot, nullptr, Util::Get3DPoint(Evade::EvadePoint1));
			}

			Evade::HaveSolution = true;

			return true;
		}
	}
	return false;
}

bool CEvadeLogic::InvulnerableEvade()
{
	if (EvadeSpell->IsTargetted())
	{
		auto target = Evader::GetBestTarget(EvadeToPoint, EvadeSpell);

		if (target != nullptr)
		{
			if (Evade::IsAboutToHit(EvadeSpell->Delay))
			{
				Evade::EvadePoint1 = target->ServerPosition().To2D();
				Evade::SetEvading(true);
				CastSpell(EvadeSpell->Slot, target);
			}

			Evade::HaveSolution = true;

			return true;
		}
	}
	else
	{
		if (Evade::IsAboutToHit(EvadeSpell->Delay))
		{
			if (EvadeSpell->SelfCast)
				CastSpell(EvadeSpell->Slot);
			else
				CastSpell(EvadeSpell->Slot, nullptr, Util::Get3DPoint(Evade::PlayerPosition));
		}

		Evade::HaveSolution = true;

		return true;
	}

	return false;
}

bool CEvadeLogic::ShieldEvade()
{
	if (Evade::IsAboutToHit(EvadeSpell->Delay))
		CastSpell(EvadeSpell->Slot);

	Evade::HaveSolution = true;

	return true;
}

bool CEvadeLogic::ItemEvade()
{
	static IInventoryItem* pZhonya = GPluginSDK->CreateItemForId(3157, FLT_MAX);

	if (EvadeSpell->MenuName == "Zhonyas" && pZhonya->IsOwned() && pZhonya->IsReady())
	{
		if (Evade::IsAboutToHit(100))
			pZhonya->CastOnPlayer();

		Evade::HaveSolution = true;

		return true;
	}

	return false;
}

bool CEvadeLogic::CastSpell(int SpellSlot, IUnit* Target /* = nullptr */, Vec3 CastPosition /* = Vec3(0.f, 0.f, 0.f) */)
{
	if (SpellSlot == kSlotUnknown)
		return false;

	SpellCaster->SetSpellSlot(static_cast<eSpellSlot>(SpellSlot));

	if (Target != nullptr)
		return SpellCaster->CastOnUnit(Target);
	else if (CastPosition.Length() != 0.f)
		return SpellCaster->CastOnPosition(CastPosition);
	else
		return SpellCaster->CastOnPlayer();

	return false;
}

bool CEvadeLogic::HasWalkingSpell()
{
	for (auto i : EvadeSpellsDB->Spells)
	{
		if (i->MenuName == "Walking" && i->IsEnabled())
			return true;
	}

	return false;
}