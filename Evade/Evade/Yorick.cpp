#include "IChampionManager.h"
#include "SpellDetector.h"

void Yorick::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Yorick::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName != "YorickE")
		return;

	auto start = Args.Caster_->ServerPosition().To2D();
	auto end = Args.EndPosition_.To2D();
	auto startPos = end.Extend(start, 120);
	auto endPos = startPos.Extend(start, -1);
	auto startT = GGame->TickCount() - GGame->Latency() / 2 + 350 + (int)(start.DistanceTo(startPos) / 1800 * 1000);

	SpellDetector->AddSpell(Args.Caster_, startPos, endPos, Data, nullptr, ST_None, true, startT);

	*ShouldProcess = false;
}