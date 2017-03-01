#include "IChampionManager.h"
#include "SpellDetector.h"
#include "Evade.h"

void JarvanIV::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void JarvanIV::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName != "JarvanIVQ")
		return;

	SpellData* qeData = Evade::OnProcessSpells["JarvanIVQE"];

	if (qeData == nullptr)
		return;

	auto startPos = Args.Caster_->ServerPosition().To2D();
	auto qeEnd = startPos.Extend(Args.EndPosition_.To2D(), qeData->RawRange);

	std::vector<Vec2> vecEndPos;

	for (auto i : Evade::DetectedSpells)
	{
		auto j = i.second;

		if (j->Data.MenuName == "JarvanIVE" && j->Unit == Args.Caster_)
		{
			if (j->End.Distance(startPos, qeEnd, true) < qeData->RadiusEx)
				vecEndPos.push_back(j->End);
		}
	}

	for (auto i : GEntityList->GetAllUnits())
	{
		if (!i->IsValidObject() || i->GetType() != FL_CREEP)
			continue;

		if (!i->IsDead() && std::string(i->SkinName()) == "jarvanivstandard" && i->GetTeam() == Args.Caster_->GetTeam())
		{
			if (i->ServerPosition().To2D().Distance(startPos, qeEnd, true) < qeData->RadiusEx)
				vecEndPos.push_back(i->ServerPosition().To2D());
		}
	}

	if (vecEndPos.size() == 0)
		return;

	Vec2 vecClosestPos		= vecEndPos.front();
	float flClosestDist		= vecClosestPos.DistanceTo(startPos);

	for (auto i : vecEndPos)
	{
		float flDist = i.DistanceTo(startPos);

		if (flDist < flClosestDist)
		{
			flClosestDist = flDist;
			vecClosestPos = i;
		}
	}

	SpellDetector->AddSpell(Args.Caster_, startPos, vecClosestPos, qeData);
}