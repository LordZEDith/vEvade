#include "IChampionManager.h"
#include "SpellDetector.h"

void Aatrox::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Aatrox::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName != "AatroxE")
		return;

	auto startPos = Args.Caster_->ServerPosition().To2D();
	auto endPos = Args.EndPosition_.To2D();
	auto dir = (endPos - startPos).VectorNormalize().Perpendicular() * Data->GetRadius();
	SpellDetector->AddSpell(Args.Caster_, startPos + dir, endPos, Data);
	SpellDetector->AddSpell(Args.Caster_, startPos - dir, endPos, Data);

	*ShouldProcess = false;
}