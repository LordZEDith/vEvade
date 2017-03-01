#include "IChampionManager.h"
#include "SpellDetector.h"

void Malzahar::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Malzahar::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName != "MalzaharQ")
		return;

	auto end = Args.EndPosition_.To2D();
	auto dir = (end - Args.Caster_->ServerPosition().To2D()).VectorNormalize().Perpendicular() * (Data->GetRange() / 2.f);
	auto startPos = end - dir;
	auto endPos = end + dir;
	
	SpellDetector->AddSpell(Args.Caster_, startPos, endPos, Data);
	SpellDetector->AddSpell(Args.Caster_, endPos, startPos, Data);

	*ShouldProcess = false;
}