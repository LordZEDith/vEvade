#include "IChampionManager.h"
#include "SpellDetector.h"
#include "Evade.h"
#include "Polygons.h"

void Syndra::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Syndra::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName.substr(0, 7) != "SyndraE")
		return;

	SpellDetector->AddSpell(Args.Caster_, Args.Caster_->ServerPosition(), Args.EndPosition_, Data);

	SpellInstance* pSpell = nullptr;

	for (auto i : Evade::DetectedSpells)
	{
		auto j = i.second;

		if (j->Data.MenuName == Data->MenuName && j->Unit == Args.Caster_)
		{
			pSpell = j;
			break;
		}
	}

	SpellData* eqData = Evade::OnProcessSpells["SyndraEQ"];

	if (pSpell != nullptr && eqData != nullptr)
	{
		auto cone = Polygons::Cone(pSpell->Cone.Center, pSpell->Cone.Direction, pSpell->Data.RawRadius + 30, pSpell->Data.RawRange + 200).ToPolygon();

		std::vector<Vec2> orbs;

		for (auto i : Evade::DetectedSpells)
		{
			auto j = i.second;

			if (j->Data.MenuName == "SyndraQ" && j->Unit == pSpell->Unit)
			{
				if (GGame->TickCount() - j->StartTick < 400)
					orbs.push_back(j->End);
			}
		}

		for (auto i : GEntityList->GetAllMinions(true, true, true))
		{
			if (!i->IsValidObject() || i->IsDead() || i->GetTeam() != Args.Caster_->GetTeam())
				continue;

			if (std::string(i->SkinName()) != "syndrasphere")
				continue;

			orbs.push_back(i->ServerPosition().To2D());
		}

		for (auto i : orbs)
		{
			if (cone.IsOutside(i))
				continue;

			auto startPos = i.Extend(pSpell->Start, 100);
			auto endPos = pSpell->Start.Extend(i, pSpell->Start.DistanceTo(i) > 200 ? 1300 : 1100);
			auto startT = pSpell->StartTick + Data->Delay + (int)(pSpell->Start.DistanceTo(i) / Data->MissileSpeed * 1000);

			SpellDetector->AddSpell(Args.Caster_, startPos, endPos, eqData, nullptr, ST_None, true, startT);
		}
	}

	*ShouldProcess = false;
}