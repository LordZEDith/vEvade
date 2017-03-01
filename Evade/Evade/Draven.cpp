#include "IChampionManager.h"
#include "SpellDetector.h"
#include "Evade.h"

void Draven::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Draven::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName != "DravenR" || !strstr(Args.Name_, "Double"))
		return;

	SpellInstance* spell = nullptr;

	for (auto i : Evade::DetectedSpells)
	{
		auto j = i.second;

		if (j->Data.MenuName == Data->MenuName && j->Unit == Args.Caster_)
		{
			spell = j;
			break;
		}
	}

	if (spell != nullptr)
		Evade::EraseDetectedSpellById(spell->SpellId);

	*ShouldProcess = false;
}