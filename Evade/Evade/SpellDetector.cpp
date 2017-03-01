#include "SpellDetector.h"
#include "Configs.h"
#include "Evade.h"

#undef min
#undef max

#include <algorithm>
#include <regex>
#include <string>

CSpellDetector* SpellDetector = nullptr;
int CSpellDetector::spellIdCount = 0;

void CSpellDetector::OnCreateObject(IUnit* Source)
{
	OnCreateTrap(Source);
	OnCreateToggle(Source);
	OnCreateMissile(Source);
}

void CSpellDetector::OnDestroyObject(IUnit* Source)
{
	OnDeleteToggle(Source);
	OnDeleteMissile(Source);
}

void CSpellDetector::OnPlayAnimation(IUnit* Source, std::string const Args)
{
	for (auto i : Configs->ChampionManagers)
	{
		if (i.second != nullptr)
			i.second->OnPlayAnimation(Source, Args);
	}
}

void CSpellDetector::OnProcessSpell(CastedSpell const& Args)
{
	if (Args.Caster_ == nullptr || !GEntityList->Player()->IsEnemy(Args.Caster_))
		return;

	auto data = Evade::OnProcessSpells[Args.Name_];

	if (data == nullptr || data->MissileOnly)
		return;

	bool bShouldProcess = true;
	SpellData* pNewData = nullptr;

	for (auto i : Configs->ChampionManagers)
	{
		if (i.second != nullptr)
			i.second->OnDetectorProcessSpell(Args, data, &bShouldProcess, &pNewData);
	}

	if (!bShouldProcess)
		return;

	if (pNewData != nullptr)
		data = pNewData;

	AddSpell(Args.Caster_, Args.Caster_->ServerPosition(), Args.EndPosition_, data);
}

void CSpellDetector::AddSpell(IUnit* Source, Vec2 SpellStart, Vec2 SpellEnd, SpellData* Data, IUnit* MissileClient /* = nullptr */, int Type /* = ST_None */, bool CheckExplosion /* = true */, int StartT /* = 0 */)
{
	auto isFromFoW = !Source->IsVisible() && MissileClient != nullptr;

	if (isFromFoW && !Configs->DodgeFoW->Enabled())
		return;

	if (Evade::PlayerPosition.DistanceTo(SpellStart) > static_cast<float>((Data->GetRange() + Data->GetRadius() + 1000)) * 1.5)
		return;

	if (Type == ST_None)
		Type = Data->Type;

	auto startPos = SpellStart;
	auto endPos = SpellEnd;
	auto startTime = StartT > 0 ? StartT : GGame->TickCount() - GGame->Latency() / 2;
	auto endTime = Data->Delay;

	if (MissileClient == nullptr)
	{
		if (Data->BehindStart > 0)
		{
			startPos = startPos.Extend(endPos, static_cast<float>(-Data->BehindStart));
		}

		if (Data->InfrontStart > 0)
		{
			startPos = startPos.Extend(endPos, static_cast<float>(Data->InfrontStart));
		}
	}
	else
	{
		if (!Data->MissileDelayed)
		{
			startTime -= Data->Delay;
		}

		if (Data->MissileSpeed != 0)
		{
			startTime -= static_cast<int>(startPos.DistanceTo(MissileClient->GetPosition().To2D()) / static_cast<float>(Data->MissileSpeed) * 1000.f);
		}
	}

	if (Type == ST_Cone || Type == ST_MissileCone || Data->FixedRange
		|| (Data->GetRange() > 0 && endPos.DistanceTo(startPos) > Data->GetRange()))
	{
		endPos = startPos.Extend(endPos, Data->GetRange());
	}

	if (MissileClient == nullptr)
	{
		if (Data->ExtraRange > 0)
		{
			endPos = endPos.Extend(startPos, static_cast<float>(-std::min(Data->ExtraRange, Data->GetRange() - static_cast<int>(endPos.DistanceTo(startPos)))));
		}

		if (Data->Invert)
		{
			endPos = startPos.Extend(endPos, static_cast<float>(-startPos.DistanceTo(endPos)));
		}

		if (Data->Perpendicular)
		{
			auto pDir = (endPos - startPos).VectorNormalize().Perpendicular();
			startPos = SpellEnd - pDir * Data->RadiusEx;
			endPos = SpellEnd + pDir * Data->RadiusEx;
		}
	}

	switch (Type)
	{
	case ST_MissileLine:
		endTime += Data->MissileAccel != 0 ? 5000 : static_cast<int>(startPos.DistanceTo(endPos) / static_cast<float>(Data->MissileSpeed) * 1000.f);
		break;
	case ST_Circle:
		if (Data->MissileSpeed != 0)
		{
			endTime += static_cast<int>(startPos.DistanceTo(endPos) / static_cast<float>(Data->MissileSpeed) * 1000.f);

			if (Data->Type == ST_MissileLine && Data->HasStartExplosion)
			{
				endPos = startPos;
				endTime = Data->Delay;
			}
		}
		else if (Data->GetRange() == 0 && Data->GetRadius() > 0)
		{
			endPos = startPos;
		}
		break;
	case ST_Arc:
	case ST_MissileCone:
		endTime += static_cast<int>(startPos.DistanceTo(endPos) / static_cast<float>(Data->MissileSpeed) * 1000.f);
		break;
	}

	auto dir = (endPos - startPos).VectorNormalize();
	auto alreadyAdded = false;

	if (MissileClient == nullptr)
	{
		if (!Data->DontCheckForDuplicates)
		{
			for (auto spell : Evade::DetectedSpells)
			{
				auto i = spell.second;

				if (i->Data.MenuName == Data->MenuName && i->Unit == Source && dir.AngleBetween(i->Direction) < 3 && i->Start.DistanceTo(startPos) < 100)
				{
					alreadyAdded = i->MissileObject != nullptr && i->MissileObject->IsValidObject();

					if (alreadyAdded)
						break;
				}
			}
		}
	}
	else if (!Data->MissileOnly)
	{
		for (auto spell : Evade::DetectedSpells)
		{
			auto i = spell.second;

			if (i->Data.MenuName == Data->MenuName && i->Unit == Source && dir.AngleBetween(i->Direction) < 3 && i->Start.DistanceTo(startPos) < 100 && i->MissileObject == nullptr)
			{
				i->MissileObject = MissileClient;
				i->Start = GMissileData->GetStartPosition(i->MissileObject).To2D();
				alreadyAdded = true;

				if (i->Data.ToggleName.size() == 0 || i->Type == ST_Circle)
					break;
			}
		}
	}

	if (alreadyAdded)
		return;

	if (CheckExplosion && (Data->HasStartExplosion || Data->HasEndExplosion))
	{
		auto newData = Data->Clone();

		if (Data->HasStartExplosion)
			newData->CollisionObjects = kCollidesWithNothing;

		AddSpell(Source, SpellStart, SpellEnd, newData, MissileClient, ST_Circle, false, StartT);
	}

	auto newSpell = new SpellInstance(*Data, startTime, endTime + Data->ExtraDelay, startPos, endPos, Source, Type);

	newSpell->SpellId		= spellIdCount++;
	newSpell->IsFromFoW		= isFromFoW;
	newSpell->MissileObject = MissileClient;

	Evade::DetectedSpells[newSpell->SpellId] = newSpell;
}

void CSpellDetector::AddSpell(IUnit* Source, Vec3 SpellStart, Vec3 SpellEnd, SpellData* Data, IUnit* MissileClient /* = nullptr */, int Type /* = ST_None */, bool CheckExplosion /* = true */, int StartT /* = 0 */)
{
	AddSpell(Source, SpellStart.To2D(), SpellEnd.To2D(), Data, MissileClient, Type, CheckExplosion, StartT);
}

void CSpellDetector::OnCreateMissile(IUnit* Source)
{
	auto missile = Source;

	if (missile == nullptr || !missile->IsValidObject() || !missile->IsMissile())
		return;

	auto caster = GMissileData->GetCaster(missile);

	if (caster == nullptr || !caster->IsEnemy(GEntityList->Player()))
		return;

	auto data = Evade::OnMissileSpells[GMissileData->GetName(missile)];

	if (data == nullptr)
		return;

	OnCreateMissileDelay(missile, caster, data);
}

void CSpellDetector::OnCreateMissileDelay(IUnit* Missile, IUnit* Caster, SpellData* Data)
{
	SpellData* pNewData		= nullptr;
	bool bShouldProcess		= true;

	for (auto i : Configs->ChampionManagers)
	{
		if (i.second != nullptr)
			i.second->OnDetectorCreateSpell(Caster, Missile, Data, &pNewData, &bShouldProcess);
	}

	if (!bShouldProcess)
		return;

	if (pNewData != nullptr)
		Data = pNewData;

	AddSpell(Caster, GMissileData->GetStartPosition(Missile), GMissileData->GetEndPosition(Missile), Data, Missile);
}

void CSpellDetector::OnCreateToggle(IUnit* Source)
{
	auto toggle = Source;

	if (toggle == nullptr || !toggle->IsValidObject())
		return;

	auto pszIdentifier = toggle->GetClassIdentifier();

	if (pszIdentifier == nullptr || _stricmp(pszIdentifier, "Obj_GeneralParticleEmitter") != 0)
		return;

	auto pos = toggle->GetPosition().To2D();

	for (auto i : Evade::DetectedSpells)
	{
		auto spell = i.second;

		if (spell->Data.ToggleName.size() != 0 && spell->Type == ST_Circle && spell->MissileObject != nullptr && spell->ToggleObject == nullptr)
		{
			std::regex rx(spell->Data.ToggleName);

			if (std::regex_search(std::string(toggle->GetObjectName()), rx))
			{
				if (spell->End.DistanceTo(pos) < 100)
				{
					spell->MissileObject = nullptr;
					spell->ToggleObject = toggle;
					spell->End = pos;
				}
			}
		}
	}
}

void CSpellDetector::OnCreateTrap(IUnit* Source)
{
	auto trap = Source;

	if (trap == nullptr || !trap->IsValidObject() || trap->GetType() != FL_CREEP || !trap->IsEnemy(GEntityList->Player()))
		return;

	auto data = Evade::OnTrapSpells[trap->SkinName()];

	if (data != nullptr)
		OnCreateTrapDelay(trap, data);
}

void CSpellDetector::OnCreateTrapDelay(IUnit* Source, SpellData* Data)
{
	auto pos = Source->GetPosition().To2D();

	IUnit* caster = nullptr;

	for (auto i : GEntityList->GetAllHeros(false, true))
	{
		if (std::string(i->ChampionName()) == Data->ChampName)
		{
			caster = i;
			break;
		}
	}

	auto spell = new SpellInstance(*Data, GGame->TickCount() - GGame->Latency() / 2, 0, pos, pos, caster, Data->Type);

	spell->SpellId = spellIdCount++;
	spell->TrapObject = Source;

	Evade::DetectedSpells[spell->SpellId] = spell;
}

void CSpellDetector::OnDeleteMissile(IUnit* Source)
{
	auto missile = Source;

	if (missile == nullptr || !missile->IsValidObject() || !missile->IsMissile())
		return;

	auto caster = GMissileData->GetCaster(missile);

	if (caster == nullptr || !caster->IsValidObject() || !caster->IsEnemy(GEntityList->Player()))
		return;

	for (auto spell : Evade::DetectedSpells)
	{
		auto j = spell.second;

		int iSpellId = j->SpellId;

		if (j->Data.CanBeRemoved && j->MissileObject != nullptr && j->MissileObject->IsValidObject() && j->MissileObject == missile)
		{
			if (j->Data.ToggleName.size() == 0 || j->Type != ST_Circle)
			{
				Evade::EraseDetectedSpellAfter(j, 1);
			}
			else
			{
				j->Data.CollisionObjects = kCollidesWithNothing;
				j->PredEnd = Vec2(0.f, 0.f);
				j->End = missile->GetPosition().To2D(); 

				if (j->ToggleObject != nullptr)
					Evade::EraseDetectedSpellAfter(j, 100);
			}
		}
	}
}

void CSpellDetector::OnDeleteToggle(IUnit* Source)
{
	auto toggle = Source;

	if (toggle == nullptr || !toggle->IsValidObject())
		return;

	auto pszIdentifier = toggle->GetClassIdentifier();

	if (pszIdentifier == nullptr || _stricmp(pszIdentifier, "Obj_GeneralParticleEmitter") != 0)
		return;

	for (auto i : Evade::DetectedSpells)
	{
		auto spell = i.second;

		int iSpellId = spell->SpellId;

		if (spell->Data.ToggleName.size() != 0 && spell->ToggleObject != nullptr && spell->ToggleObject == toggle)
			Evade::EraseDetectedSpellAfter(spell, 5);
	}
}