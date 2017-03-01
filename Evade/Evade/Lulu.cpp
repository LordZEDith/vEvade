#include "IChampionManager.h"
#include "SpellDetector.h"

void Lulu::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Lulu::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName != "LuluQ")
		return;

	for (auto i : GEntityList->GetAllUnits())
	{
		if (!i->IsValidObject() || i->GetType() != FL_CREEP)
			continue;

		if (std::string(i->SkinName()) != "lulufaerie")
			continue;

		if (!i->IsDead() || i->IsVisible() && i->GetTeam() == Args.Caster_->GetTeam())
			SpellDetector->AddSpell(Args.Caster_, i->ServerPosition(), Args.EndPosition_, Data);
	}
}