#include "IChampionManager.h"
#include "SpellDetector.h"

void Azir::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Azir::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName != "AzirR")
		return;

	auto pNewData = Data->Clone();
	int iArgs[] = { 4, 5, 6 };

	pNewData->RawRadius = Data->GetRadius() * iArgs[Args.Caster_->GetSpellLevel(Data->Slot) - 1];

	// *NewData = pNewData;
}