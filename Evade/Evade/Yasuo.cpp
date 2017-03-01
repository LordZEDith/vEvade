#include "IChampionManager.h"
#include "SpellDetector.h"

void Yasuo::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Yasuo::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName != "YasuoQ")
		return;

	auto delay = (int)(Args.Caster_->GetSpellBook()->GetCastTime() - GGame->Time()) * 1000;

	if (delay <= 0)
		return;

	auto newData = Data->Clone();
	newData->Delay = delay;

	*NewData = newData;
}