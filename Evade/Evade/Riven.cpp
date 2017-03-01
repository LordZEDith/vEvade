#include "IChampionManager.h"
#include "SpellDetector.h"

void Riven::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	if (Data->MenuName != "RivenQ3")
		return;

	Init				= true;
	RivenQSpellData		= Data;
	LastQTick			= 0;
}

void Riven::OnDash(UnitDash* Args)
{
	auto caster = Args->Source;

	if (caster == nullptr || !caster->IsValidObject() || !caster->IsHero() || !caster->IsEnemy(GEntityList->Player()))
		return;

	if (std::string(caster->ChampionName()) != RivenQSpellData->ChampName)
		return;

	if (GGame->TickCount() - LastQTick > 100)
		return;

	auto newData = RivenQSpellData->Clone();

	if (caster->HasBuff("RivenFengShuiEngine"))
		newData->RawRadius += 75;

	SpellDetector->AddSpell(caster, Args->StartPosition, Args->EndPosition, newData, nullptr, ST_None, true, LastQTick);
}

void Riven::OnPlayAnimation(IUnit* Source, std::string const AnimationName)
{
	auto caster = Source;

	if (caster == nullptr || !caster->IsValidObject() || !caster->IsEnemy(GEntityList->Player()))
		return;

	if (std::string(caster->ChampionName()) != RivenQSpellData->ChampName)
		return;

	if (AnimationName != "c49a3951")
		LastQTick = GGame->TickCount();
}