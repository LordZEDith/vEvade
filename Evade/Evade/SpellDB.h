#include "SpellData.h"

struct SpellDB
{
	SpellDB();
	SpellData* CreateSpell();

	std::vector<SpellData*> Spells;
};

extern SpellDB* SpellsDB;