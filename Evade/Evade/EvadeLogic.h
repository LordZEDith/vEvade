#ifndef EvadeLogic_h__
#define EvadeLogic_h__

#include "PluginSDK.h"
#include "SpellInstance.h"
#include "EvadeSpellData.h"

struct DelayedCastSpell
{
	int TimeToCast;
	int Slot;
	IUnit* Target;
	Vec2 Point;
};

class CEvadeLogic
{
public:
	CEvadeLogic()
	{
		SpellCaster				= GPluginSDK->CreateSpell(kSlotUnknown);
		ExpectedArrivalTime		= 0;
		EvasionPoint			= Vec2(0.f, 0.f);
		PrevPos					= Vec2(0.f, 0.f);
	}


	void OnGameUpdate();
	void OnRender();
	bool OnIssueOrder(IUnit* Source, DWORD OrderIdx, Vec3* Position, IUnit* Target);
	bool CastSpell(int SpellSlot, IUnit* Target = nullptr, Vec3 CastPosition = Vec3(0.f, 0.f, 0.f));

private:
	bool IsWithinSkillshot();
	bool HasWalkingSpell();
	bool IsInsideSkillshot(Vec2& ClosestOutsidePoint);
	int GetMaxDangerLevel(std::vector<SpellInstance*> SpellsList);
	void CastDelayedSpells();

private:
	void AttemptToShieldAlly();
	void RunEvasionSpells(SafePoint Point);
	bool WalkEvade();
	bool SpellShieldEvade();
	bool MovementSpeedEvade();
	bool DashEvade();
	bool BlinkEvade();
	bool InvulnerableEvade();
	bool ShieldEvade();
	bool ItemEvade();

private:
	Vec2 EvasionPoint;
	Vec2 PrevPos;
	Vec2 EvadeToPoint;
	int ExpectedArrivalTime;
	EvadeSpellData* EvadeSpell;
	ISpell* SpellCaster;

	std::vector<DelayedCastSpell> DelayedSpells;
};

extern CEvadeLogic* EvadeLogic;

#endif // EvadeLogic_h__
