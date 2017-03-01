#ifndef EvadeSpellDB_h__
#define EvadeSpellDB_h__

#include "EvadeSpellData.h"

struct EvadeSpellDB
{
	std::vector<EvadeSpellData*> Spells;

	EvadeSpellData* CreateSpell();
	EvadeSpellDB();
};

extern EvadeSpellDB* EvadeSpellsDB;

#endif // EvadeSpellDB_h__
