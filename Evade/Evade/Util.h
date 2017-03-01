#ifndef Util_h__
#define Util_h__

#include "PluginSDK.h"

namespace Util
{
	static void CreateConsoleWindow()
	{
		AllocConsole();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
		SetConsoleTitleA("Debug Console");
	}

	static void LogConsole(char* Fmt, ...)
	{
		DWORD dwBytes = 0;
		char szBuffer[1024] = { 0 };

		va_list va;
		va_start(va, Fmt);
		vsnprintf_s(szBuffer, sizeof(szBuffer), Fmt, va);
		va_end(va);

		strcat_s(szBuffer, "\n");
		
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), szBuffer, strlen(szBuffer), &dwBytes, nullptr);
	}

	static Vec3 Get3DPoint(Vec2 const& Position)
	{
		return Vec3(Position.x, GNavMesh->GetHeightForPoint(Position), Position.y);
	}

	static void DrawLine(Vec2 const Start, Vec2 const End, Vec4 const Color)
	{
		Vec3 vecStart3D = Get3DPoint(Start);
		Vec3 vecEnd3D = Get3DPoint(End);

		Vec3 vecOut1;
		GGame->Projection(vecStart3D, &vecOut1);

		Vec3 vecOut2;
		GGame->Projection(vecEnd3D, &vecOut2);

		if (vecOut1.z < 1.f && vecOut2.z < 1.f)
			GRender->DrawLine(Vec2(vecOut1.x, vecOut1.y), Vec2(vecOut2.x, vecOut2.y), Color);
	}

	static bool Move(Vec2 Pos)
	{
		return GGame->IssueOrder(GEntityList->Player(), kMoveTo, Get3DPoint(Pos));
	}

	static bool ShieldCheck()
	{
		if (GEntityList->Player()->HasBuffOfType(BUFF_SpellShield) || GEntityList->Player()->HasBuffOfType(BUFF_SpellImmunity))
			return true;

		std::string szChamp = std::string(GEntityList->Player()->ChampionName());

		if (szChamp == "Olaf")
		{
			if (GEntityList->Player()->HasBuff("OlafRagnarok"))
				return true;
		}
		else if (szChamp == "Sion")
		{
			if (GEntityList->Player()->HasBuff("SionR"))
				return true;
		}

		return false;

// 			if (ObjectManager.Player.LastCastedSpellName() == "SivirE"
// 				&& Utils.TickCount - ObjectManager.Player.LastCastedSpellT() < 300)
// 			{
// 				return true;
// 			}
// 
// 			if (ObjectManager.Player.LastCastedSpellName() == "BlackShield"
// 				&& Utils.TickCount - ObjectManager.Player.LastCastedSpellT() < 300)
// 			{
// 				return true;
// 			}
// 
// 			if (ObjectManager.Player.LastCastedSpellName() == "NocturneShit"
// 				&& Utils.TickCount - ObjectManager.Player.LastCastedSpellT() < 300)
// 			{
// 				return true;
// 			}
// 
// 			return false;
// 		}
	}

	static bool ImmobileCheck()
	{
		std::vector<void*> vecOut;
		GEntityList->Player()->GetAllBuffsData(vecOut);

		float flImmobileEndTime = 0.f;

		for (auto i : vecOut)
		{
			if (!GBuffData->IsValid(i))
				continue;

			auto iType = GBuffData->GetBuffType(i);

			if (iType == BUFF_Charm || iType == BUFF_Knockup || iType == BUFF_Stun || iType == BUFF_Suppression || iType == BUFF_Snare)
			{
				float flBuffTime = GBuffData->GetEndTime(i);

				if (flBuffTime > flImmobileEndTime)
					flImmobileEndTime = flBuffTime;
			}
		}

		if (flImmobileEndTime == 0.f)
			return false;

		auto time = GGame->TickCount() + static_cast<int>((flImmobileEndTime - GGame->Time()) * 1000.f);

		return (time - GGame->Time() > static_cast<float>(GGame->Latency() / 1000.f) / 2.f + 0.07f);
	}

	static bool CommonCheck()
	{
		auto pPlayer = GEntityList->Player();
		return (pPlayer->IsDead() || pPlayer->IsInvulnerable() || !pPlayer->IsTargetable() || ShieldCheck() || pPlayer->IsCastingImportantSpell(nullptr) || pPlayer->IsDashing() || ImmobileCheck());
	}

	// This really needs to be less ghetto...
	static int GetWardSlot()
	{
		static IInventoryItem* pWardItems[] = { nullptr };

		if (pWardItems[0] == nullptr)
		{
			int wardIds[] =
			{
				2045, 2049, 2050, 2301, 2302, 2303, 3340, 3361, 3362, 3711, 1408, 1409, 1410, 1411, 2043, 2055
			};

			for (auto i = 0; i < 16; i++)
				pWardItems[i] = GPluginSDK->CreateItemForId(i, FLT_MAX);
		}

		for (auto i : pWardItems)
		{
			if (i->IsOwned() && i->IsReady())
				return i->ItemSpellSlot();
		}

		return kSlotUnknown;
	}
}
#endif // Util_h__
