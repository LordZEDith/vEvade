#ifndef HiuManager_h__
#define HiuManager_h__

#include "ObjManager.h"

class CHiuManager
{
public:
	CHiuManager() { }

	Vec2 GetHiuDirection(int Time, Vec2 Position)
	{
		std::vector<ObjManagerInfo*> vecInfo;

		for (auto i : ObjManager->ObjCache)
		{
			auto hiu = i.second;

			if (hiu->Name == "Hiu" && Time - hiu->Time < 20 && (hiu->Obj->GetPosition().To2D() - Position).Length() < 750.f)
				vecInfo.push_back(hiu);
		}

		std::sort(vecInfo.begin(), vecInfo.end(), [](ObjManagerInfo* First, ObjManagerInfo* Second)
		{
			return (Second->Time < First->Time);
		});

		return (vecInfo.size() >= 2 ? (vecInfo.at(1)->Obj->GetPosition() - vecInfo.at(0)->Obj->GetPosition()).To2D().VectorNormalize() : Vec2(0.f, 0.f));
	}

	void OnCreateObject(IUnit* Source)
	{
		if (!Source->IsValidObject() || Source->UnitFlags() != FL_CREEP || !Source->IsEnemy(GEntityList->Player()))
			return;

		if (!strstr(Source->SkinName(), "TestCube") || ObjManager->ObjCache[Source->GetNetworkId()] != nullptr)
			return;

		ObjManager->ObjCache[Source->GetNetworkId()] = new ObjManagerInfo(Source, "Hiu");
	}
};

extern CHiuManager* HiuManager;

#endif // HiuManager_h__
