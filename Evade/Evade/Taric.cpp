#include "IChampionManager.h"
#include "SpellDetector.h"

void Taric::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Taric::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName != "TaricE")
		return;

	for (auto i : GEntityList->GetAllHeros(true, true))
	{
		if (!i->IsValidObject() || i->IsDead() || !i->IsVisible() || i->GetTeam() != Args.Caster_->GetTeam())
			continue;

		if (!i->HasBuff("taricwleashactive"))
			continue;

		SpellDetector->AddSpell(i, i->ServerPosition(), Args.EndPosition_, Data);
	}
}