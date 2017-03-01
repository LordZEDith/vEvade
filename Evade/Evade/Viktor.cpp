#include "IChampionManager.h"
#include "SpellDetector.h"
#include "Evade.h"

void Viktor::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Viktor::OnDetectorCreateSpell(IUnit* Sender, IUnit* Missile, SpellData* Data, SpellData** NewData, bool* ShouldProcess)
{
	if (Data->MenuName != "ViktorE")
		return;

	if (std::string(GMissileData->GetName(Missile)) != Data->MissileName)
		return;

	auto pSPell = Evade::OnProcessSpells["ViktorEExplosion"];

	if (pSPell != nullptr)
		SpellDetector->AddSpell(Sender, GMissileData->GetStartPosition(Missile), GMissileData->GetEndPosition(Missile), pSPell);
}