#ifndef IChampionManager_h__
#define IChampionManager_h__

#include "SpellData.h"

class IChampionManager
{
public:
	IChampionManager()
	{
		Init = false;
	}

	virtual void LoadSpecialSpells(SpellData* Data) = 0;

	// Events
	virtual void OnGameUpdate() { }
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) { }
	virtual void OnDetectorCreateSpell(IUnit* Sender, IUnit* Missile, SpellData* Data, SpellData** NewData, bool* ShouldProcess) { }
	virtual void OnCreateObject(IUnit* CreatedObject) { }
	virtual void OnDash(UnitDash* Args) { }
	virtual void OnPlayAnimation(IUnit* Source, std::string const AnimationName) { }

protected:
	bool Init;
};

class AllChampions : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Aatrox : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Azir : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Bard : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorCreateSpell(IUnit* Sender, IUnit* Missile, SpellData* Data, SpellData** NewData, bool* ShouldProcess) override;
};

class Diana : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Draven : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Ekko : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
	virtual void OnDetectorCreateSpell(IUnit* Sender, IUnit* Missile, SpellData* Data, SpellData** NewData, bool* ShouldProcess) override;
	virtual void OnCreateObject(IUnit* CreatedObject) override;
};

class Fizz : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
	virtual void OnDetectorCreateSpell(IUnit* Sender, IUnit* Missile, SpellData* Data, SpellData** NewData, bool* ShouldProcess) override;
};

class Galio : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorCreateSpell(IUnit* Sender, IUnit* Missile, SpellData* Data, SpellData** NewData, bool* ShouldProcess) override;
};

class JarvanIV : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Karma : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Lucian : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Lulu : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Lux : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnCreateObject(IUnit* CreatedObject) override;
	
private:
	void OnCreateObjectHiu(IUnit* CreatedObject);

private:
	IUnit* LuxUnit;
	SpellData* LuxRSpellData;
};

class Malzahar : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Orianna : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Poppy : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
	virtual void OnDetectorCreateSpell(IUnit* Sender, IUnit* Missile, SpellData* Data, SpellData** NewData, bool* ShouldProcess) override;
};

class RekSai : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnPlayAnimation(IUnit* Source, std::string const AnimationName) override;

private:
	SpellData* RekSaiWSpellData;
};

class Riven : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDash(UnitDash* Args) override;
	virtual void OnPlayAnimation(IUnit* Source, std::string const AnimationName) override;

private:
	SpellData* RivenQSpellData;
	int LastQTick;
};

class Sion : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnCreateObject(IUnit* CreatedObject) override;

private:
	IUnit* SionUnit;
	SpellData* SionESpellData;
};

class Syndra : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Taric : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Viktor : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorCreateSpell(IUnit* Sender, IUnit* Missile, SpellData* Data, SpellData** NewData, bool* ShouldProcess) override;
};

class Yasuo : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Yorick : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Zac : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDash(UnitDash* Args) override;
	virtual void OnPlayAnimation(IUnit* Source, std::string const AnimationName) override;

private:
	SpellData* ZacESpellData;
	int LastETick;
};

class Zed : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnDetectorProcessSpell(CastedSpell const& Args, SpellData* Data, bool* ShouldProcess, SpellData** NewData) override;
};

class Zilean : public IChampionManager
{
public:
	virtual void LoadSpecialSpells(SpellData* Data) override;
	virtual void OnGameUpdate() override;
};

#endif // IChampionManager_h__
