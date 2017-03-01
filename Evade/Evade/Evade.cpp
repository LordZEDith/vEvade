#include "Evade.h"
#include "SpellBlocker.h"
#include "EvadeSpellDB.h"
#include "Evader.h"
#include "PathFollow.h"
#include "PathFinding.h"
#include "EvadeLogic.h"

#include <algorithm>
#include <vector>
#include <string>

std::unordered_map<std::string, SpellData*> Evade::OnMissileSpells;
std::unordered_map<std::string, SpellData*> Evade::OnProcessSpells;
std::unordered_map<std::string, SpellData*> Evade::OnTrapSpells;
std::unordered_map<int, SpellInstance*> Evade::DetectedSpells;
int Evade::LastWardJumpTick = 0;
Vec2 Evade::PlayerPosition(0.f, 0.f);
std::vector<SpellInstance*> Evade::Spells;
Vec2 Evade::EvadePoint1, Evade::EvadePoint2;
bool Evade::Evading = false;
bool Evade::ForceFollowPath = false;
bool Evade::HaveSolution = false;
int Evade::LastMoveTick1 = 0;
int Evade::LastMoveTick2 = 0;
int Evade::LastEvadePointChangeTick = 0;
bool Evade::IsForcedToFollowpath = false;
Vec2 Evade::NextMovePosition = Vec2(0.f, 0.f);
extern bool Evade::LastMoveFailed = false;

bool Evade::IsAboutToHit(int time, IUnit* unit /* = nullptr */)
{
	time += 150;

	if (unit == nullptr)
		unit = GEntityList->Player();

	for (auto i : Spells)
	{
		if (i->IsAboutToHit(time, unit))
			return true;
	}

	return false;
}

void Evade::OnGameLoad()
{
	GRender->Notification(Vec4(0, 255, 255, 255), 10, "vEvade++ Loaded!");
}

void Evade::BeforeAttack(IUnit* target)
{
	if (Evading)
		GOrbwalking->DisableNextAttack();
}

bool Evade::OnPreCast(int Slot, IUnit* Target, Vec3* StartPosition, Vec3* EndPosition)
{
	if (Slot == kSlotRecall)
		EvadePoint1 = Vec2(0.f, 0.f);

	if (!Evading)
		return true;

	if (!Configs->CheckBlock->Enabled())
		return true;

	bool bIsDangerous = false;

	for (auto i : Spells)
	{
		if (i->IsSafePoint(PlayerPosition))
			continue;

		bIsDangerous = i->GetValue("IsDangerous")->Enabled();

		if (bIsDangerous)
			break;
	}

	if (!bIsDangerous)
		return true;

	return !SpellBlocker::CanBlock(Slot);
}

void Evade::OnDash(UnitDash* Args)
{
	if (Args->Source != GEntityList->Player())
		return;

	EvadePoint2 = Args->EndPosition.To2D();
}

void Evade::OnDraw()
{
	if (GEntityList->Player()->IsDead())
		return;

	static IFont* pFont = nullptr;
	
	if (pFont == nullptr)
	{
		pFont = GRender->CreateFont("Tahoma", 16.f, kFontWeightNormal);
		pFont->SetOutline(true);
		pFont->SetLocationFlags(kFontLocationCenter);
	}

	if (Configs->DrawStatus->Enabled())
	{
		Vec2 pos;
		if (GGame->Projection(GEntityList->Player()->GetPosition(), &pos))
		{
			std::string text = std::string("Evade: ") + (Configs->Enabled->Enabled() ? "On" : "Off");
			Vec4 clr = Vec4(128, 128, 128, 255);

			if (Configs->Enabled->Enabled())
				clr = Evading ? Vec4(255, 0, 0, 255) : (GetAsyncKeyState(Configs->DodgeDangerous->Enabled()) ? Vec4(255, 255, 0, 255) : Vec4(255, 255, 255, 255));

			pFont->SetColor(clr);
			pFont->Render(pos.x, pos.y, text.c_str());
		}
	}

	if (Configs->DrawSpells->Enabled())
	{
		for (auto i : DetectedSpells)
		{
			auto pSpell = i.second;
			pSpell->Draw(pSpell->IsEnabled() ? Vec4(255, 255, 255, 255) : Vec4(255, 0, 0, 255));
		}
	}

	EvadeLogic->OnRender();
}

bool Evade::OnIssueOrder(IUnit* Source, DWORD OrderIdx, Vec3* Position, IUnit* Target)
{
	if (Source != GEntityList->Player())
		return true;

	return EvadeLogic->OnIssueOrder(Source, OrderIdx, Position, Target);
}

void Evade::OnUpdate()
{
	EvadeLogic->OnGameUpdate();
}

void Evade::UpdateSpells()
{
	for (auto& i : DetectedSpells)
	{
		auto spell = i.second;

		if (spell->Data.IsDash && GGame->TickCount() - spell->StartTick > spell->Data.Delay + 100 && !spell->Unit->IsDashing())
			Evade::EraseDetectedSpellAfter(spell, 50);
		
		if (spell->TrapObject != nullptr && (!GEntityList->DoesObjectExist(spell->TrapObject) || spell->TrapObject->IsDead()))
			Evade::EraseDetectedSpellAfter(spell, 1);

		if (spell->EndTick + spell->Data.ExtraDuration <= GGame->TickCount())
			Evade::EraseDetectedSpellAfter(spell, 1);
	}

	for (auto i = DetectedSpells.begin(); i != DetectedSpells.end();)
	{
		if (i->second->WaitingToRemoveTime != 0 && GGame->TickCount() >= i->second->WaitingToRemoveTime)
			i = DetectedSpells.erase(i);
		else
			i++;
	}

	for (auto i : DetectedSpells)
		i.second->OnUpdate();

	std::vector<SpellInstance*> vecEnabledSpells;

	for (auto i : DetectedSpells)
	{ 
		if (i.second->IsEnabled())
			vecEnabledSpells.push_back(i.second);
	}

	Spells = vecEnabledSpells;
}