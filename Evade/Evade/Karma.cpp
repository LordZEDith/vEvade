#include "IChampionManager.h"
#include "SpellDetector.h"
#include "Evade.h"

void Karma::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Karma::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName != "KarmaQ" || !Args.Caster_->HasBuff("KarmaMantra"))
		return;

	SpellData* mantraData = Evade::OnProcessSpells["KarmaQMantra"];

	if (mantraData != nullptr)
		SpellDetector->AddSpell(Args.Caster_, Args.Caster_->ServerPosition(), Args.EndPosition_, mantraData);

	*ShouldProcess = false;
}