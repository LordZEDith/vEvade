#include "IChampionManager.h"
#include "SpellDetector.h"

void RekSai::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init				= true;
	RekSaiWSpellData	= Data;
}

void RekSai::OnPlayAnimation(IUnit* Source, std::string const AnimationName)
{
	if (Source == nullptr || !Source->IsHero())
		return;

	if (!Source->IsEnemy(GEntityList->Player()) || std::string(Source->ChampionName()) != RekSaiWSpellData->ChampName)
		return;

	if ((AnimationName != "c0362dea" && AnimationName != "c6352f63") || !GEntityList->Player()->HasBuff("reksaiknockupimmune"))
		return;

	Vec2 vecPosition = Source->ServerPosition().To2D();
	SpellDetector->AddSpell(Source, vecPosition, vecPosition, RekSaiWSpellData);
}