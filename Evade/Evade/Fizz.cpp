#include "IChampionManager.h"
#include "SpellDetector.h"
#include "Evade.h"

void Fizz::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Fizz::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName != "FizzQ")
		return;

	auto pTarget = Args.Target_;

	if (pTarget == GEntityList->Player())
		SpellDetector->AddSpell(Args.Caster_, Args.Caster_->ServerPosition(), pTarget->ServerPosition(), Data);

	*ShouldProcess = false;
}

void Fizz::OnDetectorCreateSpell(IUnit* Sender, IUnit* Missile, SpellData* Data, SpellData** NewData, bool* ShouldProcess)
{
	if (Data->MenuName != "FizzR")
		return;

	auto dist = GMissileData->GetStartPosition(Missile).DistanceTo(GMissileData->GetEndPosition(Missile));
	auto radius = dist >= 910 ? 450 : (dist >= 455 ? 325 : 0);

	if (radius == 0)
		return;

	SpellInstance* pSpell = nullptr;

	for (auto i : Evade::DetectedSpells)
	{
		auto j = i.second;

		if (j->Data.MenuName == Data->MenuName && j->Unit == Sender && j->Type == ST_Circle)
		{
			pSpell = j;
			break;
		}
	}

	if (pSpell != nullptr)
	{
		Evade::DetectedSpells[pSpell->SpellId]->radius = radius + GEntityList->Player()->BoundingRadius();
		return;
	}

	auto newData = Data->Clone();
	newData->RadiusEx = radius;

	*NewData = newData;
}