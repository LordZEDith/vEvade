#ifndef Evade_h__
#define Evade_h__

#include "PluginSDK.h"

#include "SpellData.h"
#include "SpellInstance.h"
#include "Util.h"

#include <unordered_map>
#include <vector>

namespace Evade
{
	extern std::unordered_map<std::string, SpellData*> OnMissileSpells;
	extern std::unordered_map<std::string, SpellData*> OnProcessSpells;
	extern std::unordered_map<std::string, SpellData*> OnTrapSpells;
	extern std::unordered_map<int, SpellInstance*> DetectedSpells;
	extern int LastWardJumpTick;
	extern Vec2 PlayerPosition;
	extern std::vector<SpellInstance*> Spells;
	extern Vec2 EvadePoint1, EvadePoint2;
	extern bool Evading;
	extern bool ForceFollowPath;
	extern bool HaveSolution;
	extern int LastMoveTick1, LastMoveTick2, LastEvadePointChangeTick;
	extern bool IsForcedToFollowpath;
	extern Vec2 NextMovePosition;
	extern bool LastMoveFailed;

	static void SetEvading(bool Val)
	{
		if (Val)
		{
			ForceFollowPath = true;
			LastMoveTick1 = 0;

			if (!Util::Move(EvadePoint1))
			{
				NextMovePosition = EvadePoint1;
				LastMoveFailed = true;
			}
		}

		Evading = Val;
	}

	static void EraseDetectedSpellById(int Id)
	{
		auto it = DetectedSpells.find(Id);

		if (it != DetectedSpells.end())
			DetectedSpells.erase(it);
	}

	static void EraseDetectedSpellsForUnit(IUnit* Source)
	{
		for (auto it = DetectedSpells.begin(); it != DetectedSpells.end();)
		{
			if (it->second->Unit == Source)
				it = DetectedSpells.erase(it);
			else
				it++;
		}
	}

	static void EraseDetectedSpellAfter(SpellInstance* Args, int Time)
	{
		if (Args->WaitingToRemoveTime == 0)
			Args->WaitingToRemoveTime = GGame->TickCount() + Time;
	}

	bool IsAboutToHit(int time, IUnit* unit = nullptr);

	void OnGameLoad();
	void BeforeAttack(IUnit* target);
	bool OnPreCast(int Slot, IUnit* Target, Vec3* StartPosition, Vec3* EndPosition);
	void OnDash(UnitDash* Args);
	void OnDraw();
	bool OnIssueOrder(IUnit* Source, DWORD OrderIdx, Vec3* Position, IUnit* Target);
	void OnUpdate();
	void UpdateSpells();
}

#endif // Evade_h__
