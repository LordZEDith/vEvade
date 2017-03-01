#include "IChampionManager.h"
#include "SpellDetector.h"
#include "SpellInstance.h"
#include "Evade.h"

void Bard::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Bard::OnDetectorCreateSpell(IUnit* Sender, IUnit* Missile, SpellData* Data, SpellData** NewData, bool* ShouldProcess)
{
	if (Data->MenuName != "BardR" || !strstr(GMissileData->GetName(Missile), "Fixed"))
		return;

	auto newData = Data->Clone();
	newData->MissileSpeed = newData->MissileMinSpeed;

	SpellInstance* spell = nullptr;

	for (auto i : Evade::DetectedSpells)
	{
		auto j = i.second;

		if (j->Data.MenuName == Data->MenuName && j->Unit == Sender)
		{
			spell = j;
			break;
		}
	}

	if (spell == nullptr)
	{
		*NewData = newData;
		return;
	}

	Evade::EraseDetectedSpellById(spell->SpellId);
	SpellDetector->AddSpell(Sender, GMissileData->GetStartPosition(Missile), GMissileData->GetEndPosition(Missile), newData, Missile);
}