#include "IChampionManager.h"
#include "SpellDetector.h"

void Zed::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Zed::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName.substr(0, 3) != "Zed")
		return;

	for (auto i : GEntityList->GetAllMinions(true, true, true))
	{
		if (!i->IsValidObject() || i->IsDead() || !i->IsVisible() || i->GetTeam() != Args.Caster_->GetTeam())
			continue;

		if (std::string(i->GetObjectName()) != "Shadow")
			continue;

		SpellDetector->AddSpell(Args.Caster_, i->ServerPosition(), Args.EndPosition_, Data);
	}
}
