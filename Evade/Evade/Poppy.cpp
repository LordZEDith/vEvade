#include "IChampionManager.h"
#include "SpellDetector.h"
#include "Evade.h"

void Poppy::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Poppy::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName != "PoppyRCharge")
		return;

	auto newData = Data->Clone();
	newData->FixedRange = true;
	*NewData = newData;
}

void Poppy::OnDetectorCreateSpell(IUnit* Sender, IUnit* Missile, SpellData* Data, SpellData** NewData, bool* ShouldProcess)
{
	if (Data->MenuName != "PoppyRCharge")
		return;

	SpellInstance* pSPell = nullptr;

	for (auto i : Evade::DetectedSpells)
	{
		auto j = i.second;

		if (j->Data.MenuName == Data->MenuName && j->Unit == Sender)
		{
			pSPell = j;
			break;
		}
	}

	if (pSPell != nullptr)
		Evade::EraseDetectedSpellById(pSPell->SpellId);
}