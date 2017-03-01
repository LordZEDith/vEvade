#ifndef ObjManager_h__
#define ObjManager_h__

#include "PluginSDK.h"

#include <unordered_map>
#include <string>
#include <algorithm>
#include <regex>

struct ObjManagerInfo
{
	std::string Name;
	IUnit* Obj;
	int Time;

	ObjManagerInfo(IUnit* _Obj, std::string _Name)
	{
		Obj = _Obj;
		Name = _Name;
		Time = GGame->TickCount();
	}
};

enum JungleType
{
	JT_Unknown,
	JT_Small,
	JT_Large,
	JT_Legendary
};

enum MinionTypes
{
	MT_Unknown = 0,
	MT_Normal = 1 << 0,
	MT_Ranged = 1 << 1,
	MT_Melee = 1 << 2,
	MT_Siege = 1 << 3,
	MT_Super = 1 << 4,
	MT_Ward = 1 << 5
};

class CObjManager
{
public:
	CObjManager()
	{
		ObjCache.clear();

		CloneList.push_back("leblanc");
		CloneList.push_back("shaco");
		CloneList.push_back("monkeyking");

		NormalMinionList.push_back("SRU_ChaosMinionMelee"); NormalMinionList.push_back("SRU_ChaosMinionRanged");
		NormalMinionList.push_back("SRU_OrderMinionMelee"); NormalMinionList.push_back("SRU_OrderMinionRanged");
		NormalMinionList.push_back("HA_ChaosMinionMelee"); NormalMinionList.push_back("HA_ChaosMinionRanged");
		NormalMinionList.push_back("HA_OrderMinionMelee"); NormalMinionList.push_back("HA_OrderMinionRanged");
		NormalMinionList.push_back("OdinRedSuperminion"); NormalMinionList.push_back("Odin_Red_Minion_Caster");
		NormalMinionList.push_back("OdinBlueSuperminion"); NormalMinionList.push_back("Odin_Blue_Minion_Caster");

		PetList.push_back("annietibbers"); PetList.push_back("elisespiderling"); PetList.push_back("heimertyellow");
		PetList.push_back("heimertblue"); PetList.push_back("malzaharvoidling"); PetList.push_back("shacobox");
		PetList.push_back("yorickspectralghoul"); PetList.push_back("yorickdecayedghoul"); PetList.push_back("yorickravenousghoul"); 
		PetList.push_back("zyrathornplant"); PetList.push_back("zyragraspingplant");

		SiegeMinionList.push_back("SRU_ChaosMinionSiege"); SiegeMinionList.push_back("SRU_OrderMinionSiege");
		SiegeMinionList.push_back("HA_ChaosMinionSiege"); SiegeMinionList.push_back("HA_OrderMinionSiege");

		SuperMinionList.push_back("SRU_ChaosMinionSuper"); SuperMinionList.push_back("SRU_OrderMinionSuper");
		SuperMinionList.push_back("HA_ChaosMinionSuper"); SuperMinionList.push_back("HA_OrderMinionSuper");
		SuperMinionList.push_back("OdinRedUltraminion"); SuperMinionList.push_back("OdinBlueUltraminion");
	}

	void OnGameUpdate()
	{
		for (auto it = std::begin(ObjCache); it != std::end(ObjCache);)
		{
			int iDeltaTicksMs = GGame->TickCount() - it->second->Time;

			if (iDeltaTicksMs > 250)
				it = ObjCache.erase(it);
			else
				++it;
		}

		// Seriously C++, still no uniform container erasure? http://www.open-std.org/JTC1/SC22/wg21/docs/papers/2014/n4161.htm
// 		ObjCache.erase(std::remove_if(ObjCache.begin(), ObjCache.end(), [](std::pair<const int, ObjManagerInfo*> Args)
// 		{
// 			return ((GGame->TickCount() - Args->Time) > 250);
// 		}));
	}

	bool IsJungle(IUnit* minion)
	{
		return (GetJungleType(minion) != JT_Unknown);
	}

	bool IsMinion(IUnit* minion)
	{
		auto type = GetMinionType(minion);
		return ((type & MT_Melee) || (type & MT_Ranged));
	}

	bool IsPet(IUnit* minion, bool includeClones = true)
	{
		std::string szName = std::string(minion->SkinName());
		
		if (std::find(PetList.begin(), PetList.end(), szName) != PetList.end())
			return true;

		if (includeClones && std::find(CloneList.begin(), CloneList.end(), szName) != CloneList.end())
			return true;

		return false;
	}

	bool IsValid(IUnit* unit)
	{
		return (unit != nullptr && unit->IsValidTarget());
	}

	bool IsWard(IUnit* minion)
	{
// 		auto type = GetMinionType(minion);
// 		if (type == MT_Ward && std::string(minion->SkinName()) == "BlueTrinket");
		return minion->IsWard();
	}

	int GetJungleType(IUnit* minion)
	{
		if (minion->GetTeam() != kNeutral || !minion->IsJungleCreep())
			return JT_Unknown;

		std::regex SmallNameRegex[] =
		{
			std::regex("SRU_[a-zA-Z](.*?)Mini"),
			std::regex("Sru_Crab")
		};

		std::regex LargeNameRegex[] =
		{
			std::regex("SRU_Murkwolf[0-9.]{1,}"),
			std::regex("SRU_Gromp"),
			std::regex("SRU_Blue[0-9.]{1,}"),
			std::regex("SRU_Razorbeak[0-9.]{1,}"),
			std::regex("SRU_Red[0-9.]{1,}"),
			std::regex("SRU_Krug[0-9]{1,}")
		};

		std::regex LegendaryNameRegex[] =
		{
			std::regex("SRU_Dragon"),
			std::regex("SRU_Baron"),
			std::regex("SRU_RiftHerald")
		};

		std::string szMinion = std::string(minion->GetObjectName());

		for (auto i : SmallNameRegex)
		{
			if (std::regex_search(szMinion, i))
				return JT_Small;
		}

		for (auto i : LargeNameRegex)
		{
			if (std::regex_search(szMinion, i))
				return JT_Large;
		}

		for (auto i : LegendaryNameRegex)
		{
			if (std::regex_search(szMinion, i))
				return JT_Legendary;
		}

		return JT_Unknown;
	}

	int GetMinionType(IUnit* minion)
	{
		if (minion->GetType() != FL_CREEP)
			return MT_Unknown;

		std::string szBaseSkinName = std::string(minion->SkinName());

		if (std::find(NormalMinionList.begin(), NormalMinionList.end(), szBaseSkinName) != NormalMinionList.end())
			return MT_Normal | (minion->IsMelee() ? MT_Melee : MT_Ranged);

		if (std::find(SiegeMinionList.begin(), SiegeMinionList.end(), szBaseSkinName) != SiegeMinionList.end())
			return MT_Siege | MT_Ranged;

		if (std::find(SuperMinionList.begin(), SuperMinionList.end(), szBaseSkinName) != SuperMinionList.end())
			return MT_Super | MT_Melee;

		if (minion->IsWard())
			return MT_Ward;

		return MT_Unknown;
	}

protected:
	friend class CHiuManager;
	std::unordered_map<int, ObjManagerInfo*> ObjCache;

private:
	std::vector<std::string> CloneList;
	std::vector<std::string> NormalMinionList;
	std::vector<std::string> PetList;
	std::vector<std::string> SiegeMinionList;
	std::vector<std::string> SuperMinionList;
};

extern CObjManager* ObjManager;

#endif // ObjManager_h__
