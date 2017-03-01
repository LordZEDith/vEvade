#include "IChampionManager.h"
#include "SpellDetector.h"
#include "Evade.h"

void Diana::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Diana::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName != "DianaQCircle")
		return;

	SpellData* pData = Evade::OnProcessSpells["DianaQArc"];

	if (pData != nullptr)
		SpellDetector->AddSpell(Args.Caster_, Args.Caster_->ServerPosition(), Args.EndPosition_, pData);
}