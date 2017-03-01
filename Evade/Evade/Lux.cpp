#include "IChampionManager.h"
#include "SpellDetector.h"
#include "HiuManager.h"
#include "Evade.h"
#include "HiuManager.h"

#include <regex>

void Lux::LoadSpecialSpells(SpellData* Data)
{
	if (Data->MenuName != "LuxR")
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

	LuxUnit			= pHero;
	LuxRSpellData	= Data;
}

void Lux::OnCreateObject(IUnit* CreatedObject)
{
	if (LuxUnit == nullptr)
		return;

	OnCreateObjectHiu(CreatedObject);

	if (CreatedObject == nullptr || !CreatedObject->IsValidObject())
		return;

	std::regex rx("Lux_.+_R_mis_beam_middle");
	if (!std::regex_search(CreatedObject->GetObjectName(), rx))
		return;

	auto startT = GGame->TickCount();

	SpellInstance* pSpell = nullptr;

	for (auto i : Evade::DetectedSpells)
	{
		if (i.second->Data.MenuName == LuxRSpellData->MenuName && i.second->Unit == LuxUnit)
		{
			pSpell = i.second;
			break;
		}
	}

	if (pSpell != nullptr)
		return;

	auto pos = CreatedObject->GetPosition().To2D();
	auto dir = HiuManager->GetHiuDirection(startT, pos);

	if (!dir.IsValid())
		return;

	dir *= LuxRSpellData->GetRange() / 2.f;
	SpellDetector->AddSpell(LuxUnit, pos - dir, pos + dir, LuxRSpellData, nullptr, ST_None, true, startT);
}

void Lux::OnCreateObjectHiu(IUnit* CreatedObject)
{
	HiuManager->OnCreateObject(CreatedObject);
}