#include "IChampionManager.h"
#include "SpellDetector.h"
#include "Evade.h"

#include <regex>

void Sion::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	if (Data->MenuName != "SionEMinion")
		return;

	IUnit* pHero = nullptr;

	for (auto i : GEntityList->GetAllHeros(false, true))
	{
		if (std::string(i->ChampionName()) == Data->ChampName)
		{
			pHero = i;
			break;
		}
	}

	if (pHero == nullptr)
		return;

	Init			= true;
	SionUnit		= pHero;
	SionESpellData	= Data;
}

void Sion::OnCreateObject(IUnit* CreatedObject)
{
	if (CreatedObject == nullptr || !CreatedObject->IsValidObject())
		return;

	std::regex rx("Sion_.+_E_Minion");

	if (!std::regex_search(std::string(CreatedObject->GetObjectName()), rx))
		return;

	SpellInstance* pSPell = nullptr;

	for (auto i : Evade::DetectedSpells)
	{
		auto j = i.second;

		if (j->Data.MenuName == "SionE" && j->Unit == SionUnit)
		{
			pSPell = j;
			break;
		}
	}

	if (pSPell != nullptr)
		SpellDetector->AddSpell(pSPell->Unit, CreatedObject->GetPosition().To2D(), pSPell->Start + pSPell->Direction * SionESpellData->GetRange(), SionESpellData);
}