#include "IChampionManager.h"
#include "SpellDetector.h"

void Orianna::LoadSpecialSpells(SpellData* Data)
{
	if (Init)
		return;

	Init = true;
}

void Orianna::OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData)
{
	if (Data->MenuName.substr(0, 7) != "Orianna")
		return;

	auto startPos = Vec3(0.f, 0.f, 0.f);

	if (Args.Caster_->HasBuff("OrianaGhostSelf"))
	{
		startPos = Args.Caster_->ServerPosition();
	}
	else
	{
		for (auto i : GEntityList->GetAllHeros(true, true))
		{
			if (!i->IsValidObject())
				continue;

			if (!i->IsDead() && i->IsVisible() && i->GetTeam() == Args.Caster_->GetTeam() && i == Args.Caster_ && i->HasBuff("OrianaGhost"))
			{
				startPos = i->ServerPosition();
				break;
			}
		}

		if (startPos.Length() == 0.f)
		{
			for (auto i : GEntityList->GetAllMinions(true, true, true))
			{
				if (!i->IsValidObject())
					continue;

				if (std::string(i->SkinName()) != "oriannaball")
					continue;

				if (!i->IsDead() && i->IsVisible() && i->GetTeam() == Args.Caster_->GetTeam())
				{
					startPos = i->ServerPosition();
					break;
				}
			}
		}
	}

	if (startPos.Length() != 0.f)
		SpellDetector->AddSpell(Args.Caster_, startPos, Args.EndPosition_, Data);

	*ShouldProcess = false;
}