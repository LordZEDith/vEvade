#ifndef SpellDetector_h__
#define SpellDetector_h__

#include "PluginSDK.h"
#include "SpellData.h"

class CSpellDetector
{
public:
	void OnCreateObject(IUnit* Source);
	void OnDestroyObject(IUnit* Source);
	void OnProcessSpell(CastedSpell const& Args);
	void OnPlayAnimation(IUnit* Source, std::string const Args);

	void AddSpell(IUnit* Source, Vec2 SpellStart, Vec2 SpellEnd, SpellData* Data, IUnit* MissileClient = nullptr, int Type = ST_None, bool CheckExplosion = true, int StartT = 0);
	void AddSpell(IUnit* Source, Vec3 SpellStart, Vec3 SpellEnd, SpellData* Data, IUnit* MissileClient = nullptr, int Type = ST_None, bool CheckExplosion = true, int StartT = 0);

private:
	// Wrappers for various objects being created
	void OnCreateMissile(IUnit* Source);
	void OnCreateMissileDelay(IUnit* Missile, IUnit* Caster, SpellData* Data);
	void OnCreateToggle(IUnit* Source);
	void OnCreateTrap(IUnit* Source);
	void OnCreateTrapDelay(IUnit* Source, SpellData* Data);

	// Wrappers for various objects being deleted 
	void OnDeleteMissile(IUnit* Source);
	void OnDeleteToggle(IUnit* Source);

private:
	static int spellIdCount;
};

extern CSpellDetector* SpellDetector;

#endif // SpellDetector_h__
