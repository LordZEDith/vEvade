#include "IChampionManager.h"
#include "SpellDetector.h"
#include "Evade.h"

#include <regex>

void Ekko::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Ekko::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName != "EkkoR")
		return;

	for (auto i : GEntityList->GetAllUnits())
	{
		if (i->GetType() != FL_CREEP)
			continue;

		if (!i->IsValidObject() || i->IsDead())
			continue;

		if (std::string(i->GetObjectName()) == "Ekko" && i->GetTeam() == Args.Caster_->GetTeam())
			SpellDetector->AddSpell(Args.Caster_, Args.Caster_->ServerPosition(), i->ServerPosition(), Data);
	}

	*ShouldProcess = false;
}

void Ekko::OnDetectorCreateSpell(IUnit* Sender, IUnit* Missile, SpellData* Data, SpellData** NewData, bool* ShouldProcess)
{
	if (Data->MenuName != "EkkoW")
		return;

	SpellInstance* pSpell = nullptr;

	for (auto i : Evade::DetectedSpells)
	{
		auto j = i.second;

		if (j->Data.MenuName == Data->MenuName && j->Unit == Sender && j->End.DistanceTo(GMissileData->GetEndPosition(Missile).To2D()) < 100)
		{
			pSpell = j;
			break;
		}
	}

	if (pSpell != nullptr)
		Evade::EraseDetectedSpellById(pSpell->SpellId);
}

void Ekko::OnCreateObject(IUnit* CreatedObject)
{
	if (CreatedObject == nullptr || !CreatedObject->IsValidObject())
		return;

	std::regex rxDetector("Ekko_.+_W_Detonate.troy");

	if (!std::regex_search(std::string(CreatedObject->GetObjectName()), rxDetector))
		return;

	SpellInstance* pSpell = nullptr;

	for (auto i : Evade::DetectedSpells)
	{
		auto j = i.second;

		if (j->Data.MenuName == "EkkoW" && j->End.DistanceTo(CreatedObject->GetPosition().To2D()) < 200)
		{
			pSpell = j;
			break;
		}
	}

	if (pSpell != nullptr)
		Evade::EraseDetectedSpellById(pSpell->SpellId);
}