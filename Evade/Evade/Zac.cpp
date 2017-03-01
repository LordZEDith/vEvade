#include "IChampionManager.h"
#include "SpellDetector.h"

void Zac::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	if (Data->MenuName != "ZacE")
		return;

	Init			= true;
	ZacESpellData	= Data;
	LastETick		= 0;
}

void Zac::OnDash(UnitDash* Args)
{
	if (ZacESpellData == nullptr)
		return;

	auto caster = Args->Source;

	if (caster == nullptr || !caster->IsValidObject() || !caster->IsHero() || !caster->IsEnemy(GEntityList->Player()))
		return;

	if (std::string(caster->ChampionName()) != ZacESpellData->ChampName)
		return;

	if (GGame->TickCount() - LastETick <= 100)
		SpellDetector->AddSpell(caster, Args->StartPosition, Args->EndPosition, ZacESpellData, nullptr, ST_None, true, LastETick);
}

void Zac::OnPlayAnimation(IUnit* Source, std::string const AnimationName)
{
	auto caster = Source;

	if (caster == nullptr || !caster->IsValidObject() || !caster->IsEnemy(GEntityList->Player()))
		return;

	if (std::string(caster->ChampionName()) != ZacESpellData->ChampName)
		return;

	if (AnimationName == "af176358")
		LastETick = GGame->TickCount();
}