#include "PluginSDK.h"

PluginSetup("Evade");

#include "ObjManager.h"
#include "SpellInstance.h"
#include "SpellDetector.h"
#include "Collision.h"
#include "SpellBlocker.h"
#include "Evade.h"

#include "Path.h"
#include "PathFinding.h"
#include "PathFollow.h"
#include "Core.h"
#include "Evader.h"
#include "EvadeLogic.h"
#include "SpellDB.h"
#include "EvadeSpellDB.h"

CPathFollow* PathFollow = nullptr;

#pragma region Events
PLUGIN_EVENT(void) OnGameUpdate()
{
	Evade::OnUpdate();
	ObjManager->OnGameUpdate();
}

PLUGIN_EVENT(void) OnRender()
{
	Evade::OnDraw();
	EvadeLogic->OnRender();
}

PLUGIN_EVENT(bool) OnIssueOrder(IUnit* Source, DWORD OrderIdx, Vec3* Position, IUnit* Target)
{
	if (!Evade::OnIssueOrder(Source, OrderIdx, Position, Target))
		return false;

	return true;
}

PLUGIN_EVENT(bool) OnPreCast(int Slot, IUnit* Target, Vec3* StartPosition, Vec3* EndPosition)
{
	return Evade::OnPreCast(Slot, Target, StartPosition, EndPosition);
}

PLUGIN_EVENT(void) OnDash(UnitDash* Args)
{
	Evade::OnDash(Args);
}

PLUGIN_EVENT(void) OnOrbwalkBeforeAttack(IUnit* Target)
{
	if (Evade::Evading)
		GOrbwalking->DisableNextAttack();
}

PLUGIN_EVENT(void) OnCreateObject(IUnit* Source)
{
	SpellDetector->OnCreateObject(Source);
}

PLUGIN_EVENT(void) OnDestroyObject(IUnit* Source)
{
	SpellDetector->OnDestroyObject(Source);
}

PLUGIN_EVENT(void) OnSpellCast(CastedSpell const& Args)
{
	SpellDetector->OnProcessSpell(Args);
}

PLUGIN_EVENT(void) OnPlayAnimation(IUnit* Source, std::string const Args)
{
	SpellDetector->OnPlayAnimation(Source, Args);
}
#pragma endregion

#pragma region Callbacks
PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	PluginSDKSetup(PluginSDK);

	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->AddEventHandler(kEventOnRender, OnRender);
	GEventManager->AddEventHandler(kEventOnIssueOrder, OnIssueOrder);
	GEventManager->AddEventHandler(kEventOnPreCast, OnPreCast);
	GEventManager->AddEventHandler(kEventOnDash, OnDash);
	GEventManager->AddEventHandler(kEventOrbwalkBeforeAttack, OnOrbwalkBeforeAttack);
	GEventManager->AddEventHandler(kEventOnCreateObject, OnCreateObject);
	GEventManager->AddEventHandler(kEventOnDestroyObject, OnDestroyObject);
	GEventManager->AddEventHandler(kEventOnSpellCast, OnSpellCast);
	GEventManager->AddEventHandler(kEventOnPlayAnimation, OnPlayAnimation);

	Configs			= new EvadeMenuOptions;
	SpellsDB		= new SpellDB;
	EvadeSpellsDB	= new EvadeSpellDB;

	Configs->Initialize();

	PathFinding		= new CPathFinding;
	PathFollow		= new CPathFollow;
	EvadeLogic		= new CEvadeLogic;

	Collisions::Init();
	SpellBlocker::LoadSpells();

	Evade::OnGameLoad();

	//GUtility->CreateDebugConsole();
}

PLUGIN_API void OnUnload()
{
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
	GEventManager->RemoveEventHandler(kEventOnIssueOrder, OnIssueOrder);
	GEventManager->RemoveEventHandler(kEventOnPreCast, OnPreCast);
	GEventManager->RemoveEventHandler(kEventOnDash, OnDash);
	GEventManager->RemoveEventHandler(kEventOrbwalkBeforeAttack, OnOrbwalkBeforeAttack);
	GEventManager->RemoveEventHandler(kEventOnCreateObject, OnCreateObject);
	GEventManager->RemoveEventHandler(kEventOnDestroyObject, OnDestroyObject);
	GEventManager->RemoveEventHandler(kEventOnSpellCast, OnSpellCast);
	GEventManager->RemoveEventHandler(kEventOnPlayAnimation, OnPlayAnimation);

	Configs->UnloadMenu();
}
#pragma endregion