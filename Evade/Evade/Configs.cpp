#include "Configs.h"
#include "SpellDB.h"
#include "Evade.h"
#include "EvadeSpellDB.h"
#include "SpellBlocker.h"

#include <string>

std::vector<int> SpellBlocker::Spells;

EvadeMenuOptions* Configs	= nullptr;
IMenu* EvadeParent			= nullptr;

void EvadeMenuOptions::UnloadMenu()
{
	EvadeParent->Remove();
}

IMenuOption* EvadeMenuOptions::GetOptionForSpellName(std::string const Name)
{
	for (auto i : SpellMenuOptions)
	{
		if (i.first == Name)
			return i.second;
	}

	return nullptr;
}

void EvadeMenuOptions::LoadMenuOptions()
{
	EvadeParent		= GPluginSDK->AddMenu("vEvade++");
	IMenu* pSpells	= EvadeParent->AddMenu("Evade - Spells");

	LoadSpecialSpellPlugins();

	auto SpellDBContainsKey = [](std::string const _NameCheck)
	{
		for (auto i : Evade::OnProcessSpells)
		{
			if (i.first == _NameCheck)
				return true;
		}

		return false;
	};

	auto SlotToString = [](int SlotNum)
	{
		switch (SlotNum)
		{
		case kSlotQ:
			return "Q";
		case kSlotW:
			return "W";
		case kSlotE:
			return "E";
		case kSlotR:
			return "R";
		}

		return "(#)";
	};

	// Load all spells from DB from enemy champions in game
	for (auto i : GEntityList->GetAllHeros(false, true))
	{
		for (auto j : SpellsDB->Spells)
		{
			if (SpellDBContainsKey(j->SpellName))
				continue;

			if (!j->IsSummoner && j->ChampName != std::string(i->ChampionName()))
				continue;

			if (j->IsSummoner)
			{
				auto iSummonerSlot = i->GetSpellSlot(j->SpellName.c_str());

				if (iSummonerSlot != kSummonerSlot1 && iSummonerSlot != kSummonerSlot2)
					continue;
			}

			Evade::OnProcessSpells[j->SpellName] = j;

			for (auto k : j->ExtraSpellNames)
				Evade::OnProcessSpells[k] = j;

			if (j->MissileName.size() != 0)
				Evade::OnMissileSpells[j->MissileName] = j;

			for (auto k : j->ExtraMissileNames)
				Evade::OnMissileSpells[k] = j;

			if (j->TrapName.size() != 0)
				Evade::OnTrapSpells[j->TrapName] = j;

			LoadSpecialSpell(j);			

			std::string szIdentifier = "S_" + j->MenuName;

			auto szSubMenuName	= j->IsSummoner ? j->SpellName : j->ChampName + " (" + SlotToString(j->Slot) + ")";
			auto pSubMenu		= pSpells->AddMenu(szSubMenuName.c_str());

			SpellMenuOptions[szIdentifier + "_DangerLvl"]		= pSubMenu->AddInteger("Danger Level", 1, 5, j->DangerValue);
			SpellMenuOptions[szIdentifier + "_IsDangerous"]		= pSubMenu->CheckBox("Is Dangerous", j->IsDangerous);
			SpellMenuOptions[szIdentifier + "_IgnoreHp"]		= pSubMenu->AddInteger("Ignore if HP Greater", 25, 100, !j->IsDangerous ? 65 : 80);
			SpellMenuOptions[szIdentifier + "_Draw"]			= pSubMenu->CheckBox("Draw", true);
			SpellMenuOptions[szIdentifier + "_Enabled"]			= pSubMenu->CheckBox("Enabled", !j->DisabledByDefault);
		}
	}

	// Load all evasion spells
	IMenu* pEvadeSpells = EvadeParent->AddMenu("Evade - Evade Spells");

	for (auto i : EvadeSpellsDB->Spells)
	{
		std::string szIdentifier	= std::string("ES_") + i->MenuName.c_str();
		IMenu* pSubMenu				= pEvadeSpells->AddMenu(i->MenuName.c_str());

		SpellMenuOptions[szIdentifier + "_DangerLvl"] = pSubMenu->AddInteger("Danger Level", 1, 5, i->dangerLevel);

		if (i->IsTargetted() && i->ValidTargets & VT_AllyWards)
			SpellMenuOptions[szIdentifier + "_WardJump"] = pSubMenu->CheckBox("Ward Jump", false);

		SpellMenuOptions[szIdentifier + "_Enabled"] = pSubMenu->CheckBox("Enabled", true);
	}

	// Load all shield ally options
	IMenu* pShieldSpells = EvadeParent->AddMenu("Evade - Shield Ally");

	for (auto i : GEntityList->GetAllHeros(true, false))
	{
		if (i == GEntityList->Player())
			continue;

		std::string szIdentifier = "SA_" + std::string(i->ChampionName());
		SpellMenuOptions[szIdentifier] = pShieldSpells->CheckBox(i->ChampionName(), false);
	}

	auto pMisc = EvadeParent->AddMenu("Evade - Misc");

	CheckCollision = pMisc->CheckBox("Check Collisions", false);
	CheckHp = pMisc->CheckBox("Check Player Hp", false);
	CheckBlock = pMisc->CheckBox("Block Cast While Dodge Dangerous", true);
	DodgeFoW = pMisc->CheckBox("Dodge FoW Spells", true);
	DodgeLine = pMisc->CheckBox("Dodge Line Spells", true);
	DodgeCircle = pMisc->CheckBox("Dodge Circle Spells", true);
	DodgeCone = pMisc->CheckBox("Dodge Cone Spells", true);
	DodgeTrap = pMisc->CheckBox("Dodge Traps", false);

	auto pDraw = EvadeParent->AddMenu("Evade - Draw");

	DrawSpells = pDraw->CheckBox("Draw Spells", true);
	DrawStatus = pDraw->CheckBox("Draw Status", true);

	Enabled = EvadeParent->CheckBox("Enabled", true);
	Enabledkey = EvadeParent->AddKey("Turn ON - OFF", 75);
	DodgeDangerous = EvadeParent->AddKey("Dodge Only Dangerous Key", VK_MBUTTON);
}

#define AddManager(Name) if (szChamp == std::string(#Name)){ ChampionManagers[szChamp] = new Name; }

void EvadeMenuOptions::LoadSpecialSpellPlugins()
{
	ChampionManagers.clear();

	ChampionManagers["AllChampions"] = new AllChampions;

	for (auto i : GEntityList->GetAllHeros(false, true))
	{
		std::string szChamp = i->ChampionName();

		if (ChampionManagers[szChamp] != nullptr)
			continue;

		AddManager(Aatrox);
		AddManager(Azir);
		AddManager(Bard);
		AddManager(Diana);
		AddManager(Draven);
		AddManager(Ekko);
		AddManager(Fizz);
		AddManager(Galio);
		AddManager(JarvanIV);
		AddManager(Karma);
		AddManager(Lucian);
		AddManager(Lulu);
		AddManager(Lux);
		AddManager(Malzahar);
		AddManager(Orianna);
		AddManager(Poppy);
		AddManager(RekSai);
		AddManager(Riven);
		AddManager(Sion);
		AddManager(Syndra);
		AddManager(Taric);
		AddManager(Viktor);
		AddManager(Yasuo);
		AddManager(Yorick);
		AddManager(Zac);
		AddManager(Zed);
		AddManager(Zilean);
	}
}

void EvadeMenuOptions::LoadSpecialSpell(SpellData* Args)
{
	if (ChampionManagers[Args->ChampName] != nullptr)
		ChampionManagers[Args->ChampName]->LoadSpecialSpells(Args);

	ChampionManagers["AllChampions"]->LoadSpecialSpells(Args);
}

void EvadeMenuOptions::KeyTurnOnOff()
{
	keystate = GetAsyncKeyState(Enabledkey->GetInteger());
	if (GUtility->IsLeagueWindowFocused() == false || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
		return;
	if (keystate < 0)
	{
		if (KeyWasDown == false )
		{			
			if (Enabled->GetInteger() == 0)
			{
				Enabled->UpdateInteger(1);
			}
			else
			{
				Enabled->UpdateInteger(0);
			}

			KeyWasDown = true;
		}
	}
	else
	{
		KeyWasDown = false;
	}
}