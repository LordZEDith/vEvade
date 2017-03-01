#include "IChampionManager.h"
#include "SpellDetector.h"
#include "Evade.h"

void Zilean::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Zilean::OnGameUpdate()
{
	for (auto i : Evade::DetectedSpells)
	{
		auto j = i.second;
		auto iSpellId = j->SpellId;

		if (j->Data.MenuName == "ZileanQ" && j->EndTick <= GGame->TickCount() && j->MissileObject == nullptr && j->ToggleObject == nullptr)
			Evade::EraseDetectedSpellAfter(j, 1);
	}
}