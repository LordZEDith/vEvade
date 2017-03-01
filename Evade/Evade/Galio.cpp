#include "IChampionManager.h"
#include "SpellDetector.h"

void Galio::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Galio::OnDetectorCreateSpell(IUnit* Sender, IUnit* Missile, SpellData* Data, SpellData** NewData, bool* ShouldProcess)
{
	if (Data->MenuName != "GalioE")
		return;

	IUnit* pHero = nullptr;

	for (auto i : GEntityList->GetAllHeros(false, true))
	{
		if (i->IsValidObject() && std::string(i->ChampionName()) == Data->ChampName)
		{
			pHero = i;
			break;
		}
	}

	if (pHero != nullptr)
		SpellDetector->AddSpell(pHero, GMissileData->GetStartPosition(Missile), GMissileData->GetEndPosition(Missile), Data, Missile);

	*ShouldProcess = false;
}