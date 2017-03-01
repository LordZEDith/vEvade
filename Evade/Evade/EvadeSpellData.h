#ifndef EvadeSpellData_h__
#define EvadeSpellData_h__

#include "PluginSDK.h"
#include "Configs.h"

enum SpellValidTargets
{
	VT_None				= 0,
	VT_AllyMinions		= (1<<0),
	VT_EnemyMinions		= (1<<1),
	VT_AllyWards		= (1<<2),
	VT_EnemyWards		= (1<<3),
	VT_AllyChampions	= (1<<4),
	VT_EnemyChampions	= (1<<5)
};

struct EvadeSpellData
{
	void Initialize()
	{
		memset(this, 0, sizeof(*this));
	}

	bool CanShieldAllies;
	std::string CheckSpellName;
	int Delay;
	bool FixedRange;
	bool Invert;
	bool IsBlink;
	bool IsDash;
	bool IsInvulnerability;
	bool IsItem;
	bool IsMovementSpeedBuff;
	bool IsShield;
	bool IsSpellShield;
	float MaxRange;
	std::string MenuName;
	std::function<float()> MoveSpeedTotalAmount;
	bool RequiresPreMove;
	bool SelfCast;
	int Slot;
	int Speed;
	int ValidTargets;
	int dangerLevel;

	EvadeSpellData()
	{
	}

	EvadeSpellData(std::string menuName, int _dangerLevel)
	{
		Initialize();

		MenuName		= menuName;
		dangerLevel		= _dangerLevel;
	}

	std::function<float()> MoveSpeedAmount;

	int GetDangerLevel()
	{
		auto pConfig = Configs->GetOptionForSpellName(std::string("ES_") + MenuName + "_DangerLvl");

		if (pConfig != nullptr)
			return pConfig->GetInteger();

		return dangerLevel;
	}

	bool IsEnabled()
	{
		auto pConfig = Configs->GetOptionForSpellName(std::string("ES_") + MenuName + "_Enabled");

		if (pConfig != nullptr)
			return pConfig->Enabled();

		return true;
	}

	bool IsReady()
	{
		return (!IsItem && (CheckSpellName.size() == 0 || std::string(GEntityList->Player()->GetSpellName(Slot)) == CheckSpellName) && GEntityList->Player()->GetSpellBook()->GetSpellState(Slot) == Ready);
	}

	bool IsTargetted()
	{
		return (ValidTargets != 0);
	}
};

struct DashData : public EvadeSpellData
{
	DashData(std::string menuName, int slot, float range, bool fixedRange, int delay, int speed, int _dangerLevel)
	{
		Initialize();

		MenuName = menuName;
		Slot = slot;
		MaxRange = range;
		FixedRange = fixedRange;
		Delay = delay;
		Speed = speed;
		dangerLevel = _dangerLevel;
		IsDash = true;
	}
};

struct BlinkData : public EvadeSpellData
{
	BlinkData(std::string menuName, int slot, float range, int delay, int _dangerLevel)
	{
		Initialize();

		MenuName = menuName;
		Slot = slot;
		MaxRange = range;
		Delay = delay;
		dangerLevel = _dangerLevel;
		IsBlink = true;
	}
};

struct InvulnerabilityData : public EvadeSpellData
{
	InvulnerabilityData(std::string menuName, int slot, int delay, int _dangerLevel)
	{
		Initialize();

		MenuName = menuName;
		Slot = slot;
		Delay = delay;
		dangerLevel = _dangerLevel;
		IsInvulnerability = true;
	}
};

struct ShieldData : public EvadeSpellData
{
	ShieldData(std::string menuName, int slot, int delay, int _dangerLevel, bool isSpellShield = false)
	{
		Initialize();

		MenuName = menuName;
		Slot = slot;
		Delay = delay;
		dangerLevel = _dangerLevel;
		IsSpellShield = isSpellShield;
		IsShield = !IsSpellShield;
	}
};

struct MoveBuffData : public EvadeSpellData
{
	MoveBuffData(std::string menuName, int slot, int delay, int _dangerLevel, std::function<float()> amount)
	{
		Initialize();

		MenuName = menuName;
		Slot = slot;
		Delay = delay;
		dangerLevel = _dangerLevel;
		MoveSpeedTotalAmount = amount;
		IsMovementSpeedBuff = true;
	}
};

#endif // EvadeSpellData_h__
