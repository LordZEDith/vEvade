#include "IChampionManager.h"
#include "SpellDetector.h"

void AllChampions::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void AllChampions::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName == "MalphiteR" || Data->MenuName == "SionR")
	{
		auto pNewData = Data->Clone();
		pNewData->MissileSpeed += Args.Caster_->MovementSpeed();
		*NewData = pNewData;
	}

	if (Data->MultipleNumber == -1 || (Data->MenuName == "KhazixW" && std::string(Args.Name_) == Data->SpellName))
		return;

	auto start = Args.Caster_->ServerPosition().To2D();
	auto end = Args.EndPosition_.To2D();
	auto dir = (end - start).VectorNormalize();

	for (auto i = -(Data->MultipleNumber - 1) / 2; i <= (Data->MultipleNumber - 1) / 2; i++)
		SpellDetector->AddSpell(Args.Caster_, start, start + Data->GetRange() * dir.Rotated(Data->MultipleAngle * i), Data);

	*ShouldProcess = false;
}