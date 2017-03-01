#include "EvadeSpellDB.h"

EvadeSpellDB* EvadeSpellsDB = nullptr;

#include <string>

EvadeSpellDB::EvadeSpellDB()
{
	Spells.clear();

	std::string szChamp = std::string(GEntityList->Player()->ChampionName());

	Spells.push_back(new EvadeSpellData("Walking", 1));

#pragma region Sivir
	if (szChamp == "Sivir")
		Spells.push_back(new ShieldData("SivirE", kSlotE, 100, 1, true));
#pragma endregion

#pragma region Nocturne
	if (szChamp == "Nocturne")
		Spells.push_back(new ShieldData("NocturneW", kSlotW, 100, 1, true));
#pragma endregion

#pragma region Bard
	if (szChamp == "Bard")
		Spells.push_back(new MoveBuffData("BardW", kSlotW, 150, 3, []() { return GEntityList->Player()->MovementSpeed() * 1.5f; }));
#pragma endregion

#pragma region Blitzcrank
	if (szChamp == "Blitzcrank")
	{
		Spells.push_back(
			new MoveBuffData("BlitzcrankW", kSlotW, 0, 3, []()
		{
			float flArgs[] = { 0.7f, 0.75f, 0.8f, 0.85f, 0.9f };
			return GEntityList->Player()->MovementSpeed() * (1 + flArgs[GEntityList->Player()->GetSpellLevel(kSlotW) - 1]);
		}));
	}
#pragma endregion

#pragma region Draven
	if (szChamp == "Draven")
	{
		Spells.push_back(
			new MoveBuffData("DravenW", kSlotW, 0, 3, []()
		{
			float flArgs[] = { 0.4f, 0.45f, 0.5f, 0.55f, 0.6f };
			return GEntityList->Player()->MovementSpeed() * (1 + flArgs[GEntityList->Player()->GetSpellLevel(kSlotW) - 1]);
		}));
	}
#pragma endregion

#pragma region Evelynn
	if (szChamp == "Evelynn")
	{
		Spells.push_back(
			new MoveBuffData("EvelynnW", kSlotW, 0, 3, []()
		{
			float flArgs[] = { 0.3f, 0.4f, 0.5f, 0.6f, 0.7f };
			return GEntityList->Player()->MovementSpeed() * (1 + flArgs[GEntityList->Player()->GetSpellLevel(kSlotW) - 1]);
		}));
	}
#pragma endregion

#pragma region Garen
	if (szChamp == "Garen")
		Spells.push_back(new MoveBuffData("GarenQ", kSlotQ, 0, 3, []() { return GEntityList->Player()->MovementSpeed() * 1.3f; }));
#pragma endregion

#pragma region Hecarim
	if (szChamp == "Hecarim")
		Spells.push_back(new MoveBuffData("HecarimE", kSlotE, 0, 3, []() { return GEntityList->Player()->MovementSpeed() * 1.25f; }));
#pragma endregion

#pragma region Jayce
	if (szChamp == "Jayce")
		Spells.push_back(new MoveBuffData("JayceR", kSlotR, 0, 3, [](){ return GEntityList->Player()->MovementSpeed() + 40.f; }));
#pragma endregion

#pragma region Karma
	if (szChamp == "Karma")
	{
		Spells.push_back(
			new MoveBuffData("KarmaE", kSlotE, 0, 3, []()
		{
			float flArgs[] = { 0.4f, 0.45f, 0.5f, 0.55f, 0.6f };
			return GEntityList->Player()->MovementSpeed() * (1 + flArgs[GEntityList->Player()->GetSpellLevel(kSlotE) - 1]);
		}));
	}
#pragma endregion

#pragma region Katarina
	if (szChamp == "Katarina")
	{
		Spells.push_back(
			new MoveBuffData("KatarinaW", kSlotW, 0, 3, []()
		{
			float flArgs[] = { 0.5f, 0.6f, 0.7f, 0.8f, 0.9f };
			return GEntityList->Player()->MovementSpeed() * (1 + flArgs[GEntityList->Player()->GetSpellLevel(kSlotW) - 1]);
		}));
	}
#pragma endregion

#pragma region Kayle
	if (szChamp == "Kayle")
	{
		Spells.push_back(
			new MoveBuffData("KayleW", kSlotW, 0, 3, []()
		{
			float flArgs[] = { 0.18f, 0.21f, 0.24f, 0.27f, 0.3f };
			return GEntityList->Player()->MovementSpeed() * (1 + flArgs[GEntityList->Player()->GetSpellLevel(kSlotW) - 1] + GEntityList->Player()->TotalMagicDamage() / 100 * 0.07f);
		}));
	}
#pragma endregion

#pragma region Kennen
	if (szChamp == "Kennen")
		Spells.push_back(new MoveBuffData("KennenE", kSlotE, 0, 3, []() { return GEntityList->Player()->MovementSpeed() + 225.f; }));
#pragma endregion

#pragma region Khazix
	if (szChamp == "Khazix")
		Spells.push_back(new MoveBuffData("KhazixR", kSlotR, 0, 5, []() { return GEntityList->Player()->MovementSpeed() * 1.4f; }));
#pragma endregion

#pragma region Lulu
	if (szChamp == "Lulu")
	{
		Spells.push_back(
			new MoveBuffData("LuluW", kSlotW, 0, 5, []()
		{
			return GEntityList->Player()->MovementSpeed() * (1 + (0.3f + GEntityList->Player()->TotalMagicDamage() / 100 * 0.05f));
		}));
	}
#pragma endregion

#pragma region Nunu
	if (szChamp == "Nunu")
	{
		Spells.push_back(
			new MoveBuffData("NunuW", kSlotW, 0, 3, []()
		{
			float flArgs[] = { 0.08f, 0.09f, 0.1f, 0.11f, 0.12f };
			return GEntityList->Player()->MovementSpeed() * (1 + flArgs[GEntityList->Player()->GetSpellLevel(kSlotW) - 1]);
		}));
	}
#pragma endregion

#pragma region Rumble
	if (szChamp == "Rumble")
	{
		Spells.push_back(
			new MoveBuffData("RumbleW", kSlotW, 0, 5, []()
		{
			float flArgs[] = { 0.1f, 0.15f, 0.2f, 0.25f, 0.3f };
			return GEntityList->Player()->MovementSpeed() * (1 + flArgs[GEntityList->Player()->GetSpellLevel(kSlotW) - 1] * (GEntityList->Player()->ManaPercent() > 50 ? 1.5f : 1.f));
		}));
	}
#pragma endregion

#pragma region Shyvana
	if (szChamp == "Shyvana")
	{
		Spells.push_back(
			new MoveBuffData("ShyvanaW", kSlotW, 0, 4, []()
		{
			float flArgs[] = { 0.3f, 0.35f, 0.4f, 0.45f, 0.5f };
			return GEntityList->Player()->MovementSpeed() * (1 + flArgs[GEntityList->Player()->GetSpellLevel(kSlotW) - 1]);
		}));
	}
#pragma endregion

#pragma region Sivir
	if (szChamp == "Sivir")
	{
		Spells.push_back(
			new MoveBuffData("SivirR", kSlotR, 0, 5, []()
		{
			float flArgs[] = { 0.4f, 0.5f, 0.6f };
			return GEntityList->Player()->MovementSpeed() * (1 + flArgs[GEntityList->Player()->GetSpellLevel(kSlotR) - 1]);
		}));
	}
#pragma endregion

#pragma region Sona
	if (szChamp == "Sona")
	{
		Spells.push_back(
			new MoveBuffData("SonaE", kSlotE, 0, 3, []()
		{
			float flArgs[] = { 0.1f, 0.11f, 0.12f, 0.13f, 0.14f };
			return GEntityList->Player()->MovementSpeed() * (1 + flArgs[GEntityList->Player()->GetSpellLevel(kSlotE) - 1] + GEntityList->Player()->TotalMagicDamage() / 100 * 0.06f);
		}));
	}
#pragma endregion

#pragma region Teemo
	if (szChamp == "Teemo")
	{
		Spells.push_back(
			new MoveBuffData("TeemoW", kSlotW, 0, 3, []()
		{
			float flArgs[] = { 0.2f, 0.28f, 0.36f, 0.44f, 0.52f };
			return GEntityList->Player()->MovementSpeed() * (1 + flArgs[GEntityList->Player()->GetSpellLevel(kSlotW)] - 1);
		}));
	}
#pragma endregion

#pragma region Udyr
	if (szChamp == "Udyr")
	{
		Spells.push_back(
			new MoveBuffData("UdyrE", kSlotE, 0, 3, []()
		{
			float flArgs[] = { 0.15f, 0.2f, 0.25f, 0.3f, 0.35f };
			return GEntityList->Player()->MovementSpeed() * (1 + flArgs[GEntityList->Player()->GetSpellLevel(kSlotE) - 1]);
		}));
	}
#pragma endregion

#pragma region Zilean
	if (szChamp == "Zilean")
		Spells.push_back(new MoveBuffData("ZileanE", kSlotE, 0, 3, []() { return GEntityList->Player()->MovementSpeed() * 1.55f; }));
#pragma endregion

#pragma region Aatrox
	if (szChamp == "Aatrox")
	{
		auto pSpell = new DashData("AatroxQ", kSlotQ, 650, false, 600, 3050, 3);
		pSpell->Invert = true;
		Spells.push_back(pSpell);
	}
	#pragma endregion

#pragma region Akali
	if (szChamp == "Akali")
	{
		auto pSPell = new DashData("AkaliR", kSlotR, 800, false, 100, 2461, 3);
		pSPell->ValidTargets = VT_EnemyChampions | VT_EnemyMinions;
		Spells.push_back(pSPell);
	}
#pragma endregion

#pragma region Alistar
	if (szChamp == "Alistar")
	{
		auto pSPell = new DashData("AlistarW", kSlotW, 650, false, 100, 1900, 3);
		pSPell->ValidTargets = VT_EnemyChampions | VT_EnemyMinions;
		Spells.push_back(pSPell);
	}
#pragma endregion

#pragma region Caitlyn
	if (szChamp == "Caitlyn")
	{
		auto pSpell = new DashData("CaitlynE", kSlotE, 390, true, 250, 1000, 3);
		pSpell->Invert = true;
		Spells.push_back(pSpell);
	}
#pragma endregion

#pragma region Corki
	if (szChamp == "Corki")
		Spells.push_back(new DashData("CorkiW", kSlotW, 600, false, 250, 1044, 3));
#pragma endregion

#pragma region Fizz
	if (szChamp == "Fizz")
	{
		auto pSPell = new DashData("FizzQ", kSlotQ, 550, true, 100, 1400, 4);
		pSPell->ValidTargets = VT_EnemyChampions | VT_EnemyMinions;
		Spells.push_back(pSPell);
	}
#pragma endregion

#pragma region Gnar
	if (szChamp == "Gnar")
	{
		auto pSpell = new DashData("GnarE", kSlotE, 475, false, 100, 900, 3);
		pSpell->CheckSpellName = "GnarE";
		Spells.push_back(pSpell);
	}
#pragma endregion

#pragma region Gragas
	if (szChamp == "Gragas")
		Spells.push_back(new DashData("GragasE", kSlotE, 600, true, 100, 900, 3));
#pragma endregion

#pragma region Graves
	if (szChamp == "Graves")
		Spells.push_back(new DashData("GravesE", kSlotE, 425, true, 100, 1223, 3));
#pragma endregion

#pragma region Irelia
	if (szChamp == "Irelia")
	{
		auto pSpell = new DashData("IreliaQ", kSlotQ, 650, false, 100, 2200, 3);
		pSpell->ValidTargets = VT_EnemyChampions | VT_EnemyMinions;
		Spells.push_back(pSpell);
	}
#pragma endregion

#pragma region Jax
	if (szChamp == "Jax")
	{
		auto pSPell = new DashData("JaxQ", kSlotQ, 700, false, 100, 1400, 3);
		pSPell->ValidTargets = VT_AllyChampions | VT_AllyMinions | VT_AllyWards | VT_EnemyChampions | VT_EnemyMinions | VT_EnemyWards;
		Spells.push_back(pSPell);
	}
#pragma endregion

#pragma region Leblanc
	if (szChamp == "Leblanc")
	{
		auto pSpell = new DashData("LeBlancW", kSlotW, 600, false, 100, 1600, 3);
		pSpell->CheckSpellName = "LeblancSlide";
		Spells.push_back(pSpell);
	}
#pragma endregion

#pragma region LeeSin
	if (szChamp == "LeeSin")
	{
		auto pSPell = new DashData("LeeSinW", kSlotW, 700, false, 100, 2000, 3);
		pSPell->ValidTargets = VT_AllyChampions | VT_AllyMinions | VT_AllyWards;
		pSPell->CheckSpellName = "BlindMonkWOne";
		Spells.push_back(pSPell);
	}

#pragma endregion

#pragma region Lucian
	if (szChamp == "Lucian")
		Spells.push_back(new DashData("LucianE", kSlotE, 425, false, 100, 1350, 2));
#pragma endregion

#pragma region Nidalee
	if (szChamp == "Nidalee")
	{
		auto pSPell = new DashData("NidaleeW", kSlotW, 375, true, 250, 943, 3);
		pSPell->CheckSpellName = "Pounce";
		Spells.push_back(pSPell);
	}
#pragma endregion

#pragma region Pantheon
	if (szChamp == "Pantheon")
	{
		auto pSpell = new DashData("PantheonW", kSlotW, 600, false, 100, 1000, 3);
		pSpell->ValidTargets = VT_EnemyChampions | VT_EnemyMinions;
		Spells.push_back(pSpell);
	}
#pragma endregion

#pragma region Riven
	if (szChamp == "Riven")
	{
		auto pSpell = new DashData("RivenQ", kSlotQ, 222, true, 100, 560, 3);
		pSpell->RequiresPreMove = true;

		Spells.push_back(pSpell);
		Spells.push_back(new DashData("RivenE", kSlotE, 250, false, 100, 1200, 3));
	}
#pragma endregion

#pragma region Tristana
	if (szChamp == "Tristana")
		Spells.push_back(new DashData("TristanaW", kSlotW, 900, false, 300, 1100, 5));
#pragma endregion

#pragma region Tryndamare
	if (szChamp == "Tryndamere")
		Spells.push_back(new DashData("TryndamereE", kSlotE, 650, false, 100, 900, 3));
#pragma endregion

#pragma region Vayne
	if (szChamp == "Vayne")
		Spells.push_back(new DashData("VayneQ", kSlotQ, 300, true, 100, 860, 2));
#pragma endregion

#pragma region Wukong
	if (szChamp == "MonkeyKing")
	{
		auto pSPell = new DashData("WukongE", kSlotE, 650, false, 100, 1400, 3);
		pSPell->ValidTargets = VT_EnemyChampions | VT_EnemyMinions;
		Spells.push_back(pSPell);
	}
#pragma endregion

#pragma region Yasuo
	if (szChamp == "Yasuo")
	{
		auto pSpell = new DashData("YasuoE", kSlotE, 475, true, 100, 1200, 2);
		pSpell->ValidTargets = VT_EnemyChampions | VT_EnemyMinions;
		Spells.push_back(pSpell);
	}
#pragma endregion

#pragma region Ezreal
	if (szChamp == "Ezreal")
		Spells.push_back(new BlinkData("EzrealE", kSlotE, 475, 250, 3));
#pragma endregion

#pragma region Kassadin
	if (szChamp == "Kassadin")
		Spells.push_back(new BlinkData("KassadinR", kSlotR, 500, 250, 5));
#pragma endregion

#pragma region Katarina
	if (szChamp == "Katarina")
	{
		auto pSpell = new BlinkData("KatarinaE", kSlotE, 800, 100, 3);
		pSpell->ValidTargets = VT_AllyChampions | VT_AllyMinions | VT_AllyWards | VT_EnemyChampions | VT_EnemyMinions | VT_EnemyWards;
		Spells.push_back(pSpell);
	}
#pragma endregion

#pragma region Shaco
	if (szChamp == "Shaco")
		Spells.push_back(new BlinkData("ShacoQ", kSlotQ, 400, 100, 3));
#pragma endregion

#pragma region Elise
	if (szChamp == "Elise")
	{
		auto pSpell = new InvulnerabilityData("EliseE", kSlotE, 100, 3);
		pSpell->CheckSpellName = "EliseSpiderEInitial";
		pSpell->SelfCast = true;
		Spells.push_back(pSpell);
	}
#pragma endregion

#pragma region Fizz
	if (szChamp == "Fizz")
		Spells.push_back(new InvulnerabilityData("FizzE", kSlotE, 100, 3));
#pragma endregion

#pragma region Maokai
	if (szChamp == "Maokai")
	{
		auto pSpell = new InvulnerabilityData("MaokaiW", kSlotW, 100, 3);
		pSpell->ValidTargets = VT_EnemyChampions | VT_EnemyMinions;
		pSpell->MaxRange = 525;
		Spells.push_back(pSpell);
	}
#pragma endregion

#pragma region MasterYi
	if (szChamp == "MasterYi")
	{
		auto pSpell = new InvulnerabilityData("MasterYiQ", kSlotQ, 100, 3);
		pSpell->ValidTargets = VT_EnemyChampions | VT_EnemyMinions;
		pSpell->MaxRange = 600;
		Spells.push_back(pSpell);
	}
#pragma endregion

#pragma region Vladimir
	if (szChamp == "Vladimir")
	{
		auto pSpell = new InvulnerabilityData("VladimirW", kSlotW, 100, 3);
		pSpell->SelfCast = true;
		Spells.push_back(pSpell);
	}
#pragma endregion

	if (GEntityList->Player()->GetSpellSlot("SummonerFlash") != kSlotUnknown)
		Spells.push_back(new BlinkData("Flash", GEntityList->Player()->GetSpellSlot("SummonerFlash"), 425, 100, 5));

	auto pZhonyas = new EvadeSpellData("Zhonyas", 5);
	pZhonyas->IsItem = true;
	Spells.push_back(pZhonyas);

#pragma region Janna
	if (szChamp == "Janna")
	{
		auto pSpell = new ShieldData("JannaE", kSlotE, 100, 1);
		pSpell->CanShieldAllies = true;
		pSpell->MaxRange = 800;
		Spells.push_back(pSpell);
	}
#pragma endregion

#pragma region Karma
		if (szChamp == "Karma")
		{
			auto pSpell = new ShieldData("KarmaE", kSlotE, 100, 2);
			pSpell->CanShieldAllies = true;
			pSpell->MaxRange = true;
			Spells.push_back(pSpell);
		}
#pragma endregion

#pragma region Morgana
		if (szChamp == "Morgana")
		{
			auto pSpell = new ShieldData("MorganaE", kSlotE, 100, 3);
			pSpell->CanShieldAllies = true;
			pSpell->MaxRange = 800;
			Spells.push_back(pSpell);
		}
#pragma endregion
}