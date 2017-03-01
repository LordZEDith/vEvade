#include "SpellDB.h"
#include "Configs.h"
#include "Util.h"

SpellDB* SpellsDB	= nullptr;
const float M_PI	= static_cast<float>(3.141592);

int SpellData::GetRadius()
{
	int iExtraRadius = 0;

	if (AddHitbox && Type != ST_Ring && Type != ST_Cone && Type != ST_MissileCone && Type != ST_Arc)
		iExtraRadius = static_cast<int>(GEntityList->Player()->BoundingRadius());

	return RawRadius + iExtraRadius;
}

int SpellData::GetRange()
{
	int iExtraRange = 0;

	if (Type == ST_Line || Type == ST_MissileLine)
		iExtraRange = Configs->ExtraSpellRange;

	return RawRange + iExtraRange;
}

SpellData* SpellDB::CreateSpell()
{
	auto pData = new SpellData;
	return pData->InitializeDefaults();
}

SpellDB::SpellDB()
{
	Spells.clear();

#pragma region AllChampions
	Spells.push_back(CreateSpell()->_ChampName("AllChampions")->_MenuName("SnowBall")->_SpellName("SummonerSnowball")->_Delay(0)->_MissileSpeed(1200)->_Slot(kSummonerSlot1)->_Range(1600)->_Radius(50)->_FixedRange(true)->_IsSummoner(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("AllChampions")->_MenuName("Poro")->_SpellName("SummonerPoroThrow")->_Delay(0)->_MissileSpeed(1200)->_Slot(kSummonerSlot1)->_Range(2500)->_Radius(50)->_FixedRange(true)->_IsSummoner(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Aatrox
	Spells.push_back(CreateSpell()->_ChampName("Aatrox")->_DangerValue(3)->_MenuName("AatroxQ")->_SpellName("AatroxQ")->_Range(650)->_Delay(600)->_IsDangerous(true)->_MissileSpeed(3050)->_Type(ST_Circle)->_Radius(285)->_AddHitbox(false)->_IsDash(true));
	Spells.push_back(CreateSpell()->_ChampName("Aatrox")->_MenuName("AatroxE")->_SpellName("AatroxE")->_DangerValue(2)->_MissileName("AatroxEConeMissile")->_Slot(kSlotE)->_Range(1075)->_Radius(60)->_MissileSpeed(1250)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Ahri
	Spells.push_back(CreateSpell()->_ChampName("Ahri")->_MenuName("AhriQ")->_SpellName("AhriOrbofDeception")->_DangerValue(2)->_MissileName("AhriOrbMissile")->_MissileSpeed(2500)->_Range(1000)->_Radius(100)->_FixedRange(true)->_MissileAccel(-3200)->_MissileMaxSpeed(2500)->_MissileMinSpeed(400)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Ahri")->_MenuName("AhriQReturn")->_SpellName("AhriOrbReturn")->_DangerValue(3)->_Delay(0)->_MissileName("AhriOrbReturn")->_Range(20000)->_Radius(100)->_MissileSpeed(60)->_MissileAccel(1900)->_MissileMaxSpeed(2600)->_MissileMinSpeed(60)->_MissileToUnit(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Ahri")->_MenuName("AhriE")->_SpellName("AhriSeduce")->_DangerValue(3)->_MissileName("AhriSeduceMissile")->_Slot(kSlotE)->_Range(1000)->_Radius(60)->_MissileSpeed(1550)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Akali
	Spells.push_back(CreateSpell()->_ChampName("Akali")->_DangerValue(2)->_MenuName("AkaliE")->_SpellName("AkaliShadowSwipe")->_AddHitbox(false)->_Slot(kSlotE)->_Type(ST_Circle)->_Radius(325));
#pragma endregion

#pragma region Alistar
	Spells.push_back(CreateSpell()->_ChampName("Alistar")->_DangerValue(3)->_MenuName("AlistarQ")->_SpellName("Pulverize")->_AddHitbox(false)->_IsDangerous(true)->_Type(ST_Circle)->_Radius(365));
#pragma endregion

#pragma region Amumu
	Spells.push_back(CreateSpell()->_ChampName("Amumu")->_MenuName("AmumuQ")->_SpellName("BandageToss")->_DangerValue(3)->_MissileName("SadMummyBandageToss")->_Range(1100)->_Radius(80)->_MissileSpeed(2000)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Amumu")->_DangerValue(2)->_MenuName("AmumuE")->_SpellName("Tantrum")->_AddHitbox(false)->_Slot(kSlotE)->_Type(ST_Circle)->_Radius(350));
	Spells.push_back(CreateSpell()->_ChampName("Amumu")->_DangerValue(5)->_MenuName("AmumuR")->_SpellName("CurseoftheSadMummy")->_AddHitbox(false)->_IsDangerous(true)->_Slot(kSlotR)->_Type(ST_Circle)->_Radius(550));
#pragma endregion

#pragma region Anivia
	Spells.push_back(CreateSpell()->_ChampName("Anivia")->_MenuName("AniviaQ")->_SpellName("FlashFrostSpell")->_DangerValue(3)->_MissileName("FlashFrostSpell")->_MissileSpeed(850)->_Range(1100)->_Radius(110)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Annie
	Spells.push_back(CreateSpell()->_ChampName("Annie")->_DangerValue(2)->_MenuName("AnnieW")->_SpellName("Incinerate")->_Slot(kSlotW)->_Type(ST_Cone)->_Range(560)->_Radius(50));
	Spells.push_back(CreateSpell()->_ChampName("Annie")->_DangerValue(5)->_MenuName("AnnieR")->_SpellName("InfernalGuardian")->_IsDangerous(true)->_Slot(kSlotR)->_Type(ST_Circle)->_Range(600)->_Radius(250));
#pragma endregion

#pragma region Ashe
	Spells.push_back(CreateSpell()->_ChampName("Ashe")->_MenuName("AsheW")->_MissileName("VolleyAttack")->_SpellName("Volley")->_DangerValue(2)->_MissileSpeed(1500)->_Slot(kSlotW)->_Range(1150)->_Radius(20)->_FixedRange(true)->_MultipleNumber(9)->_InfrontStart(75)->_MultipleAngle(M_PI)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Ashe")->_MenuName("AsheR")->_SpellName("EnchantedCrystalArrow")->_DangerValue(5)->_MissileName("EnchantedCrystalArrow")->_Slot(kSlotR)->_Range(25000)->_MissileSpeed(1600)->_Radius(130)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Aurelion Sol
	Spells.push_back(CreateSpell()->_ChampName("AurelionSol")->_MenuName("AurelionSolQ")->_SpellName("AurelionSolQ")->_DangerValue(2)->_MissileName("AurelionSolQMissile")->_Range(550)->_Radius(210)->_FixedRange(true)->_MissileSpeed(850)->_MissileAccel(-500)->_MissileMaxSpeed(1000)->_MissileMinSpeed(600)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("AurelionSol")->_MenuName("AurelionSolR")->_SpellName("AurelionSolR")->_DangerValue(3)->_MissileName("AurelionSolRBeamMissile")->_Slot(kSlotR)->_Range(1500)->_Delay(350)->_FixedRange(true)->_IsDangerous(true)->_MissileSpeed(4500)->_Radius(120));
#pragma endregion

#pragma region Azir
	Spells.push_back(CreateSpell()->_ChampName("Azir")->_MenuName("AzirQ")->_SpellName("AzirQWrapper")->_DangerValue(2)->_Delay(0)->_MissileName("AzirSoldierMissile")->_Range(20000)->_Radius(70)->_MissileSpeed(1600)->_MissileOnly(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Azir")->_DangerValue(3)->_MenuName("AzirR")->_Slot(kSlotR)->_SpellName("AzirR")->_MissileSpeed(1400)->_Range(750)->_Radius(150)->_BehindStart(300)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Bard
	Spells.push_back(CreateSpell()->_ChampName("Bard")->_MenuName("BardQ")->_MissileName("BardQMissile")->_SpellName("BardQ")->_DangerValue(3)->_MissileSpeed(1500)->_Range(950)->_Radius(60)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Bard")->_MenuName("BardR")->_SpellName("BardR")->_MissileName("BardRMissileFixedTravelTime")->_DangerValue(2)->_Delay(500)->_Slot(kSlotR)->_Range(3400)->_MissileMinSpeed(500)->_MissileSpeed(2100)->_Type(ST_Circle)->_Radius(350)->_ExtraMissileNames("BardRMissileRange1")->_ExtraMissileNames("BardRMissileRange2")->_ExtraMissileNames("BardRMissileRange3")->_ExtraMissileNames("BardRMissileRange4")->_ExtraMissileNames("BardRMissileRange5"));
#pragma endregion

#pragma region Blitzcrank
	Spells.push_back(CreateSpell()->_ChampName("Blitzcrank")->_MenuName("BlitzcrankQ")->_SpellName("RocketGrab")->_DangerValue(4)->_MissileName("RocketGrabMissile")->_MissileSpeed(1800)->_Range(1050)->_Radius(70)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Blitzcrank")->_DangerValue(2)->_MenuName("BlitzcrankR")->_SpellName("StaticField")->_AddHitbox(false)->_Slot(kSlotR)->_Type(ST_Circle)->_Radius(600));
#pragma endregion

#pragma region Brand
	Spells.push_back(CreateSpell()->_ChampName("Brand")->_MenuName("BrandQ")->_MissileName("BrandQMissile")->_SpellName("BrandQ")->_DangerValue(3)->_MissileSpeed(1600)->_Range(1100)->_Radius(60)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Brand")->_DangerValue(2)->_MenuName("BrandW")->_SpellName("BrandW")->_Delay(850)->_Slot(kSlotW)->_Type(ST_Circle)->_Range(900)->_Radius(240)->_AddHitbox(false));
#pragma endregion

#pragma region Braum
	Spells.push_back(CreateSpell()->_ChampName("Braum")->_MenuName("BraumQ")->_MissileName("BraumQMissile")->_SpellName("BraumQ")->_DangerValue(3)->_MissileSpeed(1700)->_Range(1050)->_Radius(60)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Braum")->_MenuName("BraumR")->_SpellName("BraumRWrapper")->_DangerValue(4)->_Delay(550)->_MissileName("BraumRMissile")->_Slot(kSlotR)->_Range(1200)->_MissileSpeed(1400)->_Radius(115)->_FixedRange(true)->_HasStartExplosion(true)->_IsDangerous(true)->_RadiusEx(330)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Caitlyn
	Spells.push_back(CreateSpell()->_ChampName("Caitlyn")->_MenuName("CaitlynQ")->_SpellName("CaitlynPiltoverPeacemaker")->_DangerValue(2)->_Delay(625)->_MissileName("CaitlynPiltoverPeacemaker")->_Range(1300)->_Radius(60)->_MissileSpeed(2200)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Caitlyn")->_MenuName("CaitlynQBehind")->_SpellName("CaitlynQBehind")->_DangerValue(2)->_Delay(0)->_MissileName("CaitlynPiltoverPeacemaker2")->_Range(1300)->_Radius(90)->_MissileSpeed(2200)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Caitlyn")->_MenuName("CaitlynW")->_SpellName("CaitlynTrap")->_TrapName("caitlyntrap")->_Delay(0)->_IsDangerous(true)->_Slot(kSlotW)->_Type(ST_Circle)->_Radius(75)->_DontCross(true)->_ExtraDuration(90000));
	Spells.push_back(CreateSpell()->_ChampName("Caitlyn")->_MenuName("CaitlynE")->_SpellName("CaitlynEntrapment")->_Delay(160)->_MissileName("CaitlynEntrapmentMissile")->_Slot(kSlotE)->_Range(800)->_MissileSpeed(1600)->_Radius(70)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Camille
	Spells.push_back(CreateSpell()->_ChampName("Camille")->_MenuName("CamilleE")->_MissileName("CamilleEMissile")->_SpellName("CamilleE")->_DangerValue(2)->_MissileSpeed(2500)->_Range(1100)->_Delay(250)->_Slot(kSlotE)->_Radius(80)->_Type(ST_Line)->_FixedRange(true)->_CanBeRemoved(true)->_AddHitbox(true)->_IsDangerous(false));
	Spells.push_back(CreateSpell()->_ChampName("Camille")->_MenuName("CamilleEDash2")->_MissileName("CamilleEDash2")->_SpellName("CamilleEDash2")->_DangerValue(2)->_MissileSpeed(2500)->_Range(800)->_Delay(250)->_Slot(kSlotE)->_Radius(80)->_Type(ST_Line)->_FixedRange(true)->_AddHitbox(true)->_IsDangerous(false));
#pragma endregion

#pragma region Cassiopeia
	Spells.push_back(CreateSpell()->_ChampName("Cassiopeia")->_DangerValue(2)->_MenuName("CassiopeiaQ")->_SpellName("CassiopeiaQ")->_Delay(750)->_Type(ST_Circle)->_Range(850)->_Radius(160));
	Spells.push_back(CreateSpell()->_ChampName("Cassiopeia")->_MenuName("CassiopeiaW")->_SpellName("CassiopeiaW")->_MissileOnly(true)->_DangerValue(2)->_MissileName("CassiopeiaWMissile")->_Slot(kSlotW)->_Range(850)->_MissileSpeed(3000)->_Radius(180)->_CanBeRemoved(false)->_ExtraDuration(5000)->_DontCross(true)->_Type(ST_Circle)->_AddHitbox(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Cassiopeia")->_DangerValue(5)->_MenuName("CassiopeiaR")->_SpellName("CassiopeiaR")->_Delay(500)->_Slot(kSlotR)->_Type(ST_Cone)->_Range(790)->_Radius(80)->_IsDangerous(true));
#pragma endregion

#pragma region Chogath
	Spells.push_back(CreateSpell()->_ChampName("Chogath")->_DangerValue(3)->_MenuName("ChogathQ")->_SpellName("Rupture")->_Range(950)->_Delay(1200)->_Type(ST_Circle)->_Radius(250));
	Spells.push_back(CreateSpell()->_ChampName("Chogath")->_DangerValue(2)->_MenuName("ChogathW")->_SpellName("FeralScream")->_Delay(500)->_Slot(kSlotW)->_Type(ST_Cone)->_Range(650)->_Radius(55));
#pragma endregion

#pragma region Corki
	Spells.push_back(CreateSpell()->_ChampName("Corki")->_MenuName("CorkiQ")->_SpellName("PhosphorusBomb")->_MissileName("PhosphorusBombMissile")->_DangerValue(2)->_Delay(300)->_MissileSpeed(1000)->_Range(825)->_Radius(250)->_Type(ST_Circle)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall)->_ExtraMissileNames("PhosphorusBombMissileMin"));
	Spells.push_back(CreateSpell()->_ChampName("Corki")->_MenuName("CorkiR")->_SpellName("MissileBarrageMissile")->_DangerValue(2)->_MissileName("MissileBarrageMissile")->_Slot(kSlotR)->_Range(1300)->_Delay(180)->_MissileSpeed(2000)->_Radius(40)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Corki")->_MenuName("CorkiREmpower")->_SpellName("MissileBarrageMissile2")->_DangerValue(2)->_MissileName("MissileBarrageMissile2")->_Slot(kSlotR)->_Range(1500)->_Delay(180)->_MissileSpeed(2000)->_Radius(40)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Darius
	Spells.push_back(CreateSpell()->_ChampName("Darius")->_DangerValue(3)->_MenuName("DariusQ")->_SpellName("DariusCleave")->_Delay(750)->_MissileToUnit(true)->_RadiusEx(225)->_Type(ST_Ring)->_Radius(425)->_DisabledByDefault(true)->_DontCross(true));
	Spells.push_back(CreateSpell()->_ChampName("Darius")->_DangerValue(3)->_MenuName("DariusE")->_SpellName("DariusAxeGrabCone")->_IsDangerous(true)->_Slot(kSlotE)->_Type(ST_Cone)->_Range(510)->_Radius(50));
#pragma endregion

#pragma region Diana
	Spells.push_back(CreateSpell()->_ChampName("Diana")->_DangerValue(3)->_MenuName("DianaQCircle")->_SpellName("DianaArc")->_MissileSpeed(1400)->_Type(ST_Circle)->_Range(850)->_Radius(195)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Diana")->_DangerValue(3)->_MenuName("DianaQArc")->_SpellName("DianaQArc")->_Range(850)->_MissileSpeed(1400)->_Type(ST_Arc)->_Radius(70)->_DontCross(true)->_IsDangerous(true)->_TakeClosestPath(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Diana")->_DangerValue(2)->_MenuName("DianaE")->_SpellName("DianaVortex")->_Radius(450)->_DisabledByDefault(true)->_Type(ST_Circle));
#pragma endregion

#pragma region DrMundo
	Spells.push_back(CreateSpell()->_ChampName("DrMundo")->_MenuName("DrMundoQ")->_SpellName("InfectedCleaverMissile")->_DangerValue(2)->_MissileName("InfectedCleaverMissile")->_Range(1050)->_Radius(60)->_MissileSpeed(2000)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Draven
	Spells.push_back(CreateSpell()->_ChampName("Draven")->_MenuName("DravenE")->_SpellName("DravenDoubleShot")->_DangerValue(3)->_MissileName("DravenDoubleShotMissile")->_Slot(kSlotE)->_Range(1100)->_MissileSpeed(1400)->_Radius(130)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Draven")->_MenuName("DravenR")->_SpellName("DravenRCast")->_DangerValue(5)->_MissileName("DravenR")->_Delay(525)->_MissileSpeed(2000)->_Slot(kSlotR)->_Range(25000)->_Radius(160)->_FixedRange(true)->_IsDangerous(true)->_InfrontStart(250)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall)->_ExtraSpellNames("DravenRDoublecast"));
#pragma endregion

#pragma region Ekko
	Spells.push_back(CreateSpell()->_ChampName("Ekko")->_MenuName("EkkoQ")->_MissileName("EkkoQMis")->_SpellName("EkkoQ")->_DangerValue(4)->_MissileSpeed(1650)->_Range(950)->_Radius(60)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Ekko")->_MenuName("EkkoQReturn")->_SpellName("EkkoQReturn")->_DangerValue(3)->_Delay(0)->_MissileName("EkkoQReturn")->_Range(20000)->_Radius(100)->_MissileSpeed(2300)->_MissileToUnit(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Ekko")->_MenuName("EkkoW")->_MissileName("EkkoWMis")->_SpellName("EkkoW")->_DangerValue(3)->_Delay(2250)->_Slot(kSlotW)->_Range(20000)->_Radius(375)->_CanBeRemoved(false)->_DisabledByDefault(true)->_ExtraDuration(1500)->_Type(ST_Circle)->_ExtraDelay(1050));
	Spells.push_back(CreateSpell()->_ChampName("Ekko")->_DangerValue(3)->_MenuName("EkkoR")->_Slot(kSlotR)->_SpellName("EkkoR")->_MissileSpeed(1650)->_Type(ST_Circle)->_Range(20000)->_Radius(375));
#pragma endregion

#pragma region Elise
	Spells.push_back(CreateSpell()->_ChampName("Elise")->_MenuName("EliseEHuman")->_SpellName("EliseHumanE")->_DangerValue(4)->_MissileName("EliseHumanE")->_Slot(kSlotE)->_Range(1100)->_Radius(55)->_MissileSpeed(1600)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Evelynn
	Spells.push_back(CreateSpell()->_ChampName("Evelynn")->_DangerValue(5)->_MenuName("EvelynnR")->_SpellName("EvelynnR")->_IsDangerous(true)->_Slot(kSlotR)->_Type(ST_Circle)->_Range(650)->_Radius(350));
#pragma endregion

#pragma region Ezreal
	Spells.push_back(CreateSpell()->_ChampName("Ezreal")->_MenuName("EzrealQ")->_SpellName("EzrealMysticShot")->_DangerValue(2)->_MissileName("EzrealMysticShotMissile")->_Range(1200)->_Radius(60)->_MissileSpeed(2000)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Ezreal")->_MenuName("EzrealW")->_SpellName("EzrealEssenceFlux")->_DangerValue(2)->_MissileName("EzrealEssenceFluxMissile")->_Slot(kSlotW)->_Range(1050)->_MissileSpeed(1600)->_Radius(80)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Ezreal")->_MenuName("EzrealR")->_SpellName("EzrealTrueshotBarrage")->_DangerValue(3)->_MissileName("EzrealTrueshotBarrage")->_Slot(kSlotR)->_Range(25000)->_Delay(1000)->_MissileSpeed(2000)->_Radius(160)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Fiora
	Spells.push_back(CreateSpell()->_ChampName("Fiora")->_MenuName("FioraW")->_MissileName("FioraWMissile")->_SpellName("FioraW")->_DangerValue(2)->_Delay(500)->_MissileSpeed(3200)->_Slot(kSlotW)->_Range(770)->_Radius(70)->_BehindStart(50)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Fizz
	Spells.push_back(CreateSpell()->_ChampName("Fizz")->_DangerValue(2)->_MenuName("FizzQ")->_SpellName("FizzQ")->_Slot(kSlotQ)->_Range(550)->_Delay(250)->_FixedRange(true)->_IsDash(true)->_MissileSpeed(3000)->_Radius(60)->_Type(ST_Line)->_AddHitbox(true)->_IsDangerous(true));
	Spells.push_back(CreateSpell()->_ChampName("Fizz")->_MenuName("FizzR")->_MissileName("FizzRMissile")->_SpellName("FizzR")->_DangerValue(5)->_Slot(kSlotR)->_ToggleName("Fizz_.+_R_OrbitFish")->_Range(1275)->_MissileSpeed(1300)->_Delay(250)->_Radius(110)->_ExtraDuration(2300)->_HasEndExplosion(true)->_IsDangerous(true)->_CanBeRemoved(true)->_RadiusEx(200)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Galio
	Spells.push_back(CreateSpell()->_ChampName("Galio")->_MenuName("GalioQ")->_SpellName("GalioResoluteSmite")->_DangerValue(2)->_MissileName("GalioResoluteSmite")->_Range(930)->_Radius(200)->_MissileSpeed(1300)->_Type(ST_Circle)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Galio")->_MenuName("GalioE")->_SpellName("GalioRighteousGust")->_DangerValue(2)->_MissileName("GalioRighteousGustMissile")->_Slot(kSlotE)->_Range(1200)->_MissileSpeed(1300)->_Radius(160)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Galio")->_DangerValue(5)->_MenuName("GalioR")->_SpellName("GalioIdolOfDurand")->_AddHitbox(false)->_IsDangerous(true)->_Slot(kSlotR)->_Type(ST_Circle)->_Radius(550)->_DontCross(true)->_ExtraDuration(2000));
#pragma endregion

#pragma region Gnar
	Spells.push_back(CreateSpell()->_ChampName("Gnar")->_MenuName("GnarQ")->_MissileName("GnarQMissile")->_SpellName("GnarQMissile")->_DangerValue(2)->_MissileSpeed(2500)->_Range(1125)->_Radius(55)->_FixedRange(true)->_MissileAccel(-3000)->_MissileMaxSpeed(2500)->_MissileMinSpeed(1400)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Gnar")->_MenuName("GnarQReturn")->_SpellName("GnarQMissileReturn")->_DangerValue(2)->_Delay(0)->_MissileName("GnarQMissileReturn")->_Range(3000)->_Radius(75)->_FixedRange(true)->_MissileSpeed(60)->_Invert(true)->_MissileAccel(800)->_MissileMaxSpeed(2600)->_MissileMinSpeed(60)->_DisabledByDefault(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Gnar")->_MenuName("GnarQBig")->_SpellName("GnarBigQMissile")->_DangerValue(2)->_Delay(500)->_MissileName("GnarBigQMissile")->_Range(1150)->_Radius(90)->_MissileSpeed(2100)->_FixedRange(true)->_HasEndExplosion(true)->_RadiusEx(210)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Gnar")->_DangerValue(2)->_MenuName("GnarWBig")->_SpellName("GnarBigW")->_Delay(600)->_Slot(kSlotW)->_Type(ST_Line)->_Range(600)->_Radius(110)->_FixedRange(true));
	Spells.push_back(CreateSpell()->_ChampName("Gnar")->_DangerValue(2)->_MenuName("GnarE")->_Slot(kSlotE)->_SpellName("GnarE")->_Delay(0)->_IsDash(true)->_MissileSpeed(900)->_Type(ST_Circle)->_Range(475)->_Radius(150));
	Spells.push_back(CreateSpell()->_ChampName("Gnar")->_DangerValue(2)->_MenuName("GnarEBig")->_SpellName("GnarBigE")->_Delay(0)->_MissileSpeed(800)->_Slot(kSlotE)->_Range(475)->_Radius(350)->_IsDash(true)->_Type(ST_Circle));
	Spells.push_back(CreateSpell()->_ChampName("Gnar")->_DangerValue(5)->_MenuName("GnarR")->_Slot(kSlotR)->_SpellName("GnarR")->_AddHitbox(false)->_Delay(275)->_IsDangerous(true)->_Type(ST_Circle)->_Radius(500));
#pragma endregion

#pragma region Gragas
	Spells.push_back(CreateSpell()->_ChampName("Gragas")->_MenuName("GragasQ")->_MissileName("GragasQMissile")->_SpellName("GragasQ")->_DangerValue(2)->_ToggleName("Gragas_.+_Q_(Enemy|Ally)")->_Range(850)->_Radius(280)->_MissileSpeed(1000)->_DontCross(true)->_ExtraDuration(4300)->_Type(ST_Circle)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Gragas")->_DangerValue(2)->_MenuName("GragasE")->_SpellName("GragasE")->_Delay(0)->_MissileSpeed(900)->_Slot(kSlotE)->_Range(600)->_Radius(200)->_ExtraRange(300)->_FixedRange(true)->_IsDash(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Gragas")->_MenuName("GragasR")->_MissileName("GragasRBoom")->_SpellName("GragasR")->_DangerValue(5)->_MissileSpeed(1800)->_Slot(kSlotR)->_Range(1050)->_Radius(350)->_Type(ST_Circle)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Graves
	Spells.push_back(CreateSpell()->_ChampName("Graves")->_MenuName("GravesQ")->_SpellName("GravesQLineSpell")->_DangerValue(2)->_MissileName("GravesQLineMis")->_MissileSpeed(3000)->_Range(808)->_Radius(40)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Graves")->_MenuName("GravesQReturn")->_SpellName("GravesQReturn")->_DangerValue(2)->_Delay(0)->_MissileName("GravesQReturn")->_Range(808)->_Radius(100)->_MissileSpeed(1600)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Graves")->_MenuName("GravesW")->_SpellName("GravesSmokeGrenade")->_MissileName("GravesSmokeGrenadeBoom")->_ToggleName("Graves_SmokeGrenade_.+_Team_(Green|Red)")->_MissileSpeed(1500)->_Slot(kSlotW)->_Range(950)->_Radius(225)->_Type(ST_Circle)->_ExtraDuration(4250)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Graves")->_MenuName("GravesR")->_SpellName("GravesChargeShot")->_DangerValue(5)->_MissileName("GravesChargeShotShot")->_Slot(kSlotR)->_Range(1000)->_MissileSpeed(2100)->_Radius(100)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Graves")->_MenuName("GravesRExplosion")->_SpellName("GravesRExplosion")->_DangerValue(5)->_MissileName("GravesChargeShotFxMissile2")->_Slot(kSlotR)->_Range(800)->_Delay(10)->_IsDangerous(true)->_MissileDelayed(true)->_Type(ST_Cone)->_Radius(45));
#pragma endregion

#pragma region Hecarim
	Spells.push_back(CreateSpell()->_ChampName("Hecarim")->_DangerValue(5)->_MenuName("HecarimRFear")->_SpellName("HecarimUlt")->_Delay(0)->_MissileSpeed(1100)->_Slot(kSlotR)->_Range(1000)->_Radius(240)->_IsDangerous(true)->_IsDash(true)->_Type(ST_Circle));
	Spells.push_back(CreateSpell()->_ChampName("Hecarim")->_MenuName("HecarimR")->_SpellName("HecarimRMissile")->_DangerValue(3)->_Delay(0)->_MissileName("HecarimUltMissile")->_Slot(kSlotR)->_Range(1500)->_MissileSpeed(1100)->_Radius(40)->_FixedRange(true)->_MissileOnly(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Heimerdinger
	Spells.push_back(CreateSpell()->_ChampName("Heimerdinger")->_MenuName("HeimerdingerQTurretBlast")->_MissileName("HeimerdingerTurretEnergyBlast")->_SpellName("HeimerdingerQTurretBlast")->_DangerValue(2)->_Delay(0)->_MissileSpeed(1650)->_Range(1000)->_Radius(50)->_FixedRange(true)->_MissileAccel(-1000)->_MissileMaxSpeed(1650)->_MissileMinSpeed(1200)->_MissileOnly(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Heimerdinger")->_MenuName("HeimerdingerQTurretBigBlast")->_MissileName("HeimerdingerTurretBigEnergyBlast")->_SpellName("HeimerdingerQTurretBigBlast")->_DangerValue(2)->_Delay(0)->_MissileSpeed(1650)->_Range(1000)->_Radius(75)->_FixedRange(true)->_MissileAccel(-1000)->_MissileMaxSpeed(1650)->_MissileMinSpeed(1200)->_MissileOnly(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Heimerdinger")->_MenuName("HeimerdingerW")->_SpellName("HeimerdingerW")->_MissileName("HeimerdingerWAttack2")->_DangerValue(2)->_Delay(0)->_MissileSpeed(750)->_Slot(kSlotW)->_Range(1350)->_Radius(40)->_FixedRange(true)->_MissileAccel(4000)->_MissileMaxSpeed(3000)->_MissileMinSpeed(750)->_MissileOnly(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall)->_ExtraMissileNames("HeimerdingerWAttack2Ult"));
	Spells.push_back(CreateSpell()->_ChampName("Heimerdinger")->_MenuName("HeimerdingerE")->_SpellName("HeimerdingerE")->_DangerValue(2)->_MissileName("HeimerdingerESpell")->_Slot(kSlotE)->_Range(950)->_Radius(100)->_MissileSpeed(1200)->_Type(ST_Circle)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Heimerdinger")->_MenuName("HeimerdingerEUlt")->_SpellName("HeimerdingerEUlt")->_DangerValue(2)->_MissileName("HeimerdingerESpell_ult")->_Slot(kSlotE)->_Range(1000)->_MissileSpeed(1200)->_Type(ST_Circle)->_Radius(150)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Heimerdinger")->_MenuName("HeimerdingerEUltBounce")->_MissileName("HeimerdingerESpell_ult2")->_SpellName("HeimerdingerEUltBounce")->_DangerValue(2)->_Slot(kSlotE)->_Delay(175)->_MissileSpeed(1400)->_Type(ST_Circle)->_Range(1000)->_Radius(150)->_DisabledByDefault(true)->_MissileDelayed(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall)->_ExtraMissileNames("HeimerdingerESpell_ult3"));
#pragma endregion

#pragma region Illaoi
	Spells.push_back(CreateSpell()->_ChampName("Illaoi")->_DangerValue(2)->_MenuName("IllaoiQ")->_SpellName("IllaoiQ")->_Range(850)->_Delay(750)->_FixedRange(true)->_IsDangerous(true)->_Type(ST_Line)->_Radius(100)->_BehindStart(50));
	Spells.push_back(CreateSpell()->_ChampName("Illaoi")->_MenuName("Illaoi")->_MissileName("IllaoiEMis")->_SpellName("IllaoiE")->_DangerValue(3)->_Delay(265)->_MissileSpeed(1900)->_Slot(kSlotE)->_Range(950)->_Radius(50)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Illaoi")->_DangerValue(3)->_MenuName("Illaoi")->_SpellName("IllaoiR")->_Delay(500)->_IsDangerous(true)->_Slot(kSlotR)->_Type(ST_Circle)->_Radius(450)->_AddHitbox(false));
#pragma endregion

#pragma region Irelia
	Spells.push_back(CreateSpell()->_ChampName("Irelia")->_MenuName("Irelia")->_SpellName("IreliaTranscendentBlades")->_DangerValue(2)->_MissileName("IreliaTranscendentBladesSpell")->_Slot(kSlotR)->_Range(1200)->_Delay(0)->_MissileSpeed(1600)->_Radius(120)->_FixedRange(true)->_MissileOnly(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Ivern
	Spells.push_back(CreateSpell()->_ChampName("Ivern")->_MenuName("IvernQ")->_MissileName("IvernQ")->_SpellName("IvernQ")->_DangerValue(3)->_MissileSpeed(1300)->_Range(1100)->_Radius(65)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Janna
	Spells.push_back(CreateSpell()->_ChampName("Janna")->_MenuName("JannaQ")->_SpellName("HowlingGale")->_DangerValue(2)->_Delay(0)->_MissileName("HowlingGaleSpell")->_Range(20000)->_Radius(120)->_MissileSpeed(900)->_MissileOnly(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region JarvanIV
	Spells.push_back(CreateSpell()->_ChampName("JarvanIV")->_MenuName("JarvanIVQ")->_SpellName("JarvanIVDragonStrike")->_DangerValue(3)->_Delay(425)->_FixedRange(true)->_Type(ST_Line)->_Range(770)->_Radius(70));
	Spells.push_back(CreateSpell()->_ChampName("JarvanIV")->_DangerValue(3)->_MenuName("JarvanIVQE")->_SpellName("JarvanIVQE")->_Delay(450)->_IsDangerous(true)->_MissileSpeed(2600)->_RadiusEx(210)->_Range(910)->_Radius(120)->_ExtraRange(200)->_IsDash(true));
	Spells.push_back(CreateSpell()->_ChampName("JarvanIV")->_MenuName("JarvanIVE")->_SpellName("JarvanIVDemacianStandard")->_DangerValue(2)->_Delay(450)->_Slot(kSlotE)->_Range(850)->_Radius(175)->_Type(ST_Circle));
	Spells.push_back(CreateSpell()->_ChampName("JarvanIV")->_DangerValue(3)->_MenuName("JarvanIVR")->_SpellName("JarvanIVCataclysm")->_Delay(0)->_MissileSpeed(1850)->_Slot(kSlotR)->_Range(650)->_Radius(340)->_Type(ST_Circle));
#pragma endregion

#pragma region Jayce
	Spells.push_back(CreateSpell()->_ChampName("Jayce")->_MenuName("JayceQ")->_SpellName("JayceShockBlast")->_DangerValue(2)->_Delay(230)->_MissileName("JayceShockBlastMis")->_Range(1050)->_Radius(70)->_MissileSpeed(1450)->_FixedRange(true)->_HasEndExplosion(true)->_RadiusEx(275)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Jayce")->_MenuName("JayceQAccel")->_SpellName("JayceQAccel")->_DangerValue(2)->_Delay(0)->_MissileName("JayceShockBlastWallMis")->_Range(2000)->_Radius(70)->_MissileSpeed(2350)->_HasEndExplosion(true)->_RadiusEx(275)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Jhin
	Spells.push_back(CreateSpell()->_ChampName("Jhin")->_DangerValue(3)->_MenuName("JhinW")->_Slot(kSlotW)->_SpellName("JhinW")->_Delay(750)->_Type(ST_Line)->_Range(2550)->_Radius(40)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Jhin")->_MenuName("JhinE")->_SpellName("JhinTrap")->_TrapName("jhintrap")->_Delay(0)->_ExtraDuration(120000)->_Slot(kSlotE)->_Type(ST_Circle)->_Radius(135));
	Spells.push_back(CreateSpell()->_ChampName("Jhin")->_MenuName("JhinR")->_MissileName("JhinRShotMis")->_SpellName("JhinRShot")->_DangerValue(3)->_Slot(kSlotR)->_Range(3500)->_Delay(190)->_MissileSpeed(5000)->_Radius(80)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall)->_ExtraMissileNames("JhinRShotMis4"));
#pragma endregion

#pragma region Jinx
	Spells.push_back(CreateSpell()->_ChampName("Jinx")->_MenuName("JinxW")->_MissileName("JinxWMissile")->_SpellName("JinxWMissile")->_DangerValue(3)->_Delay(600)->_MissileSpeed(3300)->_Slot(kSlotW)->_Range(1500)->_Radius(60)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Jinx")->_MenuName("JinxE")->_SpellName("JinxTrap")->_TrapName("jinxmine")->_Delay(0)->_IsDangerous(true)->_Slot(kSlotE)->_Type(ST_Circle)->_Radius(65)->_DontCross(true)->_ExtraDuration(5000));
	Spells.push_back(CreateSpell()->_ChampName("Jinx")->_MenuName("JinxR")->_MissileName("JinxR")->_SpellName("JinxR")->_DangerValue(5)->_Delay(600)->_MissileSpeed(1700)->_Slot(kSlotR)->_Range(25000)->_Radius(140)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Kalista
	Spells.push_back(CreateSpell()->_ChampName("Kalista")->_MenuName("KalistaQ")->_SpellName("KalistaMysticShot")->_DangerValue(2)->_MissileName("KalistaMysticShotMisTrue")->_Range(1200)->_Radius(40)->_MissileSpeed(2400)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Karma
	Spells.push_back(CreateSpell()->_ChampName("Karma")->_MenuName("KarmaQ")->_MissileName("KarmaQMissile")->_SpellName("KarmaQ")->_DangerValue(2)->_MissileSpeed(1700)->_Range(1050)->_Radius(60)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Karma")->_MenuName("KarmaQMantra")->_SpellName("KarmaQMantra")->_DangerValue(2)->_MissileName("KarmaQMissileMantra")->_ToggleName("Karma_.+_Q_impact_R_01")->_MissileSpeed(1700)->_Range(950)->_Radius(80)->_ExtraDuration(1650)->_FixedRange(true)->_HasEndExplosion(true)->_RadiusEx(280)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Karthus
	Spells.push_back(CreateSpell()->_ChampName("Karthus")->_MenuName("KarthusQ")->_SpellName("KarthusLayWasteA1")->_DangerValue(2)->_Range(875)->_Delay(950)->_DontCheckForDuplicates(true)->_Type(ST_Circle)->_Radius(160)->_ExtraSpellNames("KarthusLayWasteA2")->_ExtraSpellNames("KarthusLayWasteA3")->_ExtraSpellNames("KarthusLayWasteDeadA1")->_ExtraSpellNames("KarthusLayWasteDeadA2")->_ExtraSpellNames("KarthusLayWasteDeadA3"));
#pragma endregion

#pragma region Kassadin
	Spells.push_back(CreateSpell()->_ChampName("Kassadin")->_DangerValue(2)->_MenuName("KassadinE")->_SpellName("ForcePulse")->_Slot(kSlotE)->_Type(ST_Cone)->_Range(600)->_Radius(80));
	Spells.push_back(CreateSpell()->_ChampName("Kassadin")->_DangerValue(2)->_MenuName("KassadinR")->_SpellName("RiftWalk")->_Slot(kSlotR)->_Type(ST_Circle)->_Range(500)->_Radius(270));
#pragma endregion

#pragma region Kennen
	Spells.push_back(CreateSpell()->_ChampName("Kennen")->_MenuName("KennenQ")->_SpellName("KennenShurikenHurlMissile1")->_DangerValue(2)->_Delay(175)->_MissileName("KennenShurikenHurlMissile1")->_Range(1050)->_MissileSpeed(1700)->_Radius(50)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Khazix
	Spells.push_back(CreateSpell()->_ChampName("Khazix")->_MenuName("KhazixW")->_SpellName("KhazixW")->_DangerValue(2)->_MissileName("KhazixWMissile")->_MissileSpeed(1700)->_Slot(kSlotW)->_Range(1025)->_Radius(70)->_FixedRange(true)->_MultipleNumber(3)->_MultipleAngle(M_PI)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall)->_ExtraSpellNames("KhazixWLong"));
	Spells.push_back(CreateSpell()->_ChampName("Khazix")->_DangerValue(2)->_MenuName("KhazixE")->_SpellName("KhazixE")->_Delay(0)->_MissileSpeed(1250)->_Slot(kSlotE)->_Range(700)->_Radius(300)->_IsDash(true)->_Type(ST_Circle));
	Spells.push_back(CreateSpell()->_ChampName("Khazix")->_DangerValue(2)->_MenuName("KhazixEEvol")->_SpellName("KhazixELong")->_Delay(0)->_MissileSpeed(1200)->_Slot(kSlotE)->_Range(900)->_Radius(300)->_IsDash(true)->_Type(ST_Circle));
#pragma endregion

#pragma region Kled
	Spells.push_back(CreateSpell()->_ChampName("Kled")->_MenuName("KledQ")->_MissileName("KledQMissile")->_SpellName("KledQ")->_DangerValue(2)->_MissileSpeed(1600)->_Range(800)->_Radius(45)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Kled")->_MenuName("KledQDismount")->_SpellName("KledRiderQ")->_DangerValue(2)->_MissileName("KledRiderQMissile")->_MissileSpeed(3000)->_Range(700)->_Radius(40)->_FixedRange(true)->_MultipleNumber(5)->_MultipleAngle(M_PI)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Kled")->_DangerValue(2)->_MenuName("KledE")->_SpellName("KledEDash")->_Delay(0)->_FixedRange(true)->_MissileSpeed(970)->_Slot(kSlotE)->_Range(550)->_Radius(100)->_IsDangerous(true)->_IsDash(true));
#pragma endregion

#pragma region KogMaw
	Spells.push_back(CreateSpell()->_ChampName("KogMaw")->_MenuName("KogMawQ")->_MissileName("KogMawQ")->_SpellName("KogMawQ")->_DangerValue(2)->_MissileSpeed(1650)->_Range(1200)->_Radius(70)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("KogMaw")->_MenuName("KogMawE")->_SpellName("KogMawVoidOozeMissile")->_DangerValue(2)->_MissileName("KogMawVoidOozeMissile")->_Slot(kSlotE)->_Range(1300)->_MissileSpeed(1400)->_Radius(120)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("KogMaw")->_DangerValue(2)->_MenuName("KogMawR")->_SpellName("KogMawLivingArtillery")->_Delay(1150)->_Slot(kSlotR)->_Type(ST_Circle)->_Range(1200)->_Radius(240)->_DontCheckForDuplicates(true));
#pragma endregion

#pragma region Leblanc
	Spells.push_back(CreateSpell()->_ChampName("Leblanc")->_DangerValue(2)->_MenuName("LeblancW")->_SpellName("LeblancW")->_Delay(0)->_MissileSpeed(1600)->_Slot(kSlotW)->_Range(600)->_Radius(220)->_IsDash(true)->_Type(ST_Circle));
	Spells.push_back(CreateSpell()->_ChampName("Leblanc")->_MenuName("LeblancE")->_SpellName("LeblancE")->_DangerValue(3)->_MissileName("LeblancEMissile")->_MissileSpeed(1600)->_Slot(kSlotE)->_Range(950)->_Radius(70)->_FixedRange(true)->_IsDangerous(true)->_AddHitbox(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall)->_ExtraMissileNames("LeblancRE"));
	Spells.push_back(CreateSpell()->_ChampName("Leblanc")->_MenuName("LeblancER")->_SpellName("LeblancEM")->_DangerValue(3)->_MissileName("LeblancEM")->_MissileSpeed(1600)->_Slot(kSlotR)->_Range(950)->_Radius(70)->_FixedRange(true)->_IsDangerous(true)->_AddHitbox(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall)->_ExtraMissileNames("LeblancRE"));
#pragma endregion

#pragma region LeeSin
	Spells.push_back(CreateSpell()->_ChampName("LeeSin")->_MenuName("LeeSinQ")->_SpellName("BlindMonkQOne")->_DangerValue(3)->_MissileName("BlindMonkQOne")->_MissileSpeed(1800)->_Range(1100)->_Radius(60)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("LeeSin")->_DangerValue(2)->_MenuName("LeeSinE")->_SpellName("BlindMonkEOne")->_AddHitbox(false)->_Slot(kSlotE)->_Type(ST_Circle)->_Radius(430));
#pragma endregion

#pragma region Leona
	Spells.push_back(CreateSpell()->_ChampName("Leona")->_MenuName("LeonaE")->_SpellName("LeonaZenithBlade")->_DangerValue(3)->_MissileName("LeonaZenithBladeMissile")->_Slot(kSlotE)->_Range(900)->_MissileSpeed(2000)->_Radius(70)->_FixedRange(true)->_IsDangerous(true)->_TakeClosestPath(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Leona")->_DangerValue(5)->_MenuName("LeonaR")->_SpellName("LeonaSolarFlare")->_Delay(930)->_Slot(kSlotR)->_Type(ST_Circle)->_Range(1200)->_Radius(300)->_IsDangerous(true));
#pragma endregion

#pragma region Lissandra
	Spells.push_back(CreateSpell()->_ChampName("Lissandra")->_MenuName("LissandraQ")->_SpellName("LissandraQMissile")->_DangerValue(2)->_MissileName("LissandraQMissile")->_MissileSpeed(2200)->_Range(700)->_Radius(75)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Lissandra")->_MenuName("LissandraQExplosion")->_SpellName("LissandraQShards")->_DangerValue(2)->_Delay(0)->_MissileName("LissandraQShards")->_Range(1650)->_Radius(90)->_MissileSpeed(2200)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Lissandra")->_DangerValue(3)->_MenuName("LissandraW")->_SpellName("LissandraW")->_AddHitbox(false)->_Delay(10)->_Slot(kSlotW)->_Type(ST_Circle)->_Radius(450));
	Spells.push_back(CreateSpell()->_ChampName("Lissandra")->_MenuName("LissandraE")->_SpellName("LissandraEMissile")->_DangerValue(2)->_MissileName("LissandraEMissile")->_Slot(kSlotE)->_Range(1025)->_Radius(125)->_FixedRange(true)->_MissileSpeed(850)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Lucian
	Spells.push_back(CreateSpell()->_ChampName("Lucian")->_DangerValue(2)->_MenuName("LucianQ")->_SpellName("LucianQ")->_Range(900)->_Delay(350)->_FixedRange(true)->_Type(ST_Line)->_Radius(65));
	Spells.push_back(CreateSpell()->_ChampName("Lucian")->_MenuName("LucianW")->_MissileName("LucianWMissile")->_SpellName("LucianW")->_DangerValue(2)->_Delay(325)->_MissileSpeed(1600)->_Slot(kSlotW)->_Range(900)->_Radius(55)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Lucian")->_MenuName("LucianR")->_SpellName("LucianR")->_MissileName("LucianRMissileOffhand")->_DangerValue(2)->_Delay(0)->_MissileSpeed(2800)->_Slot(kSlotR)->_Range(1200)->_Radius(110)->_FixedRange(true)->_MissileOnly(true)->_DisabledByDefault(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall)->_ExtraMissileNames("LucianRMissile"));
#pragma endregion

#pragma region Lulu
	Spells.push_back(CreateSpell()->_ChampName("Lulu")->_MenuName("LuluQ")->_MissileName("LuluQMissile")->_SpellName("LuluQ")->_DangerValue(2)->_Range(950)->_Radius(60)->_MissileSpeed(1450)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall)->_ExtraMissileNames("LuluQMissileTwo"));
#pragma endregion

#pragma region Lux
	Spells.push_back(CreateSpell()->_ChampName("Lux")->_MenuName("LuxQ")->_SpellName("LuxLightBinding")->_DangerValue(3)->_MissileName("LuxLightBindingMis")->_Range(1300)->_Radius(70)->_MissileSpeed(1200)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Lux")->_MenuName("LuxE")->_SpellName("LuxLightStrikeKugel")->_DangerValue(2)->_MissileName("LuxLightStrikeKugel")->_ToggleName("Lux_.+_E_tar_aoe_(green|red)")->_MissileSpeed(1300)->_Slot(kSlotE)->_Type(ST_Circle)->_Range(1100)->_Radius(330)->_ExtraDuration(5100)->_DisabledByDefault(true)->_DontCross(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Lux")->_DangerValue(5)->_MenuName("LuxR")->_SpellName("LuxMaliceCannon")->_Delay(1000)->_Slot(kSlotR)->_Type(ST_Line)->_Range(3300)->_Radius(150)->_FixedRange(true)->_IsDangerous(true));
#pragma endregion

#pragma region Malphite
	Spells.push_back(CreateSpell()->_ChampName("Malphite")->_DangerValue(2)->_MenuName("MalphiteE")->_SpellName("Landslide")->_Slot(kSlotE)->_Type(ST_Circle)->_Radius(400));
	Spells.push_back(CreateSpell()->_ChampName("Malphite")->_DangerValue(5)->_MenuName("MalphiteR")->_SpellName("UFSlash")->_Delay(0)->_MissileSpeed(1600)->_Slot(kSlotR)->_Range(1000)->_Radius(270)->_IsDangerous(true)->_Type(ST_Circle));
#pragma endregion

#pragma region Malzahar
	Spells.push_back(CreateSpell()->_ChampName("Malzahar")->_MenuName("MalzaharQ")->_SpellName("MalzaharQ")->_DangerValue(2)->_Delay(520)->_MissileName("MalzaharQMissile")->_Range(750)->_Radius(85)->_FixedRange(true)->_MissileSpeed(1600));
#pragma endregion

#pragma region Maokai
	Spells.push_back(CreateSpell()->_ChampName("Maokai")->_MenuName("MaokaiQ")->_SpellName("MaokaiTrunkLine")->_DangerValue(3)->_Delay(400)->_MissileName("MaokaiTrunkLineMissile")->_Range(650)->_Radius(110)->_FixedRange(true)->_HasStartExplosion(true)->_MissileSpeed(1800)->_RadiusEx(360));
#pragma endregion

#pragma region Mordekaiser
	Spells.push_back(CreateSpell()->_ChampName("Mordekaiser")->_MenuName("MordekaiserE")->_SpellName("MordekaiserSyphonOfDestruction")->_DangerValue(2)->_Slot(kSlotE)->_Type(ST_Cone)->_Range(640)->_Radius(50));
#pragma endregion

#pragma region Morgana
	Spells.push_back(CreateSpell()->_ChampName("Morgana")->_MenuName("MorganaQ")->_SpellName("DarkBindingMissile")->_DangerValue(3)->_MissileName("DarkBindingMissile")->_Range(1300)->_Radius(70)->_MissileSpeed(1200)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Nami
	Spells.push_back(CreateSpell()->_ChampName("Nami")->_MenuName("NamiQ")->_MissileName("NamiQMissile")->_SpellName("NamiQ")->_DangerValue(3)->_MissileSpeed(2500)->_Type(ST_Circle)->_Range(850)->_Radius(200)->_ExtraDelay(700)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Nami")->_MenuName("NamiR")->_MissileName("NamiRMissile")->_SpellName("NamiRMissile")->_DangerValue(2)->_Delay(500)->_MissileSpeed(850)->_Slot(kSlotR)->_Range(2750)->_Radius(250)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Nautilus
	Spells.push_back(CreateSpell()->_ChampName("Nautilus")->_MenuName("NautilusQ")->_SpellName("NautilusAnchorDragMissile")->_DangerValue(3)->_MissileName("NautilusAnchorDragMissile")->_Range(1150)->_Radius(90)->_MissileSpeed(2000)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Nidalee
	Spells.push_back(CreateSpell()->_ChampName("Nidalee")->_MenuName("NidaleeQ")->_SpellName("JavelinToss")->_DangerValue(3)->_MissileName("JavelinToss")->_MissileSpeed(1300)->_Range(1500)->_Radius(40)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Nocturne
	Spells.push_back(CreateSpell()->_ChampName("Nocturne")->_MenuName("NocturneQ")->_SpellName("NocturneDuskbringer")->_DangerValue(2)->_MissileName("NocturneDuskbringer")->_Range(1200)->_Radius(60)->_MissileSpeed(1400)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Olaf
	Spells.push_back(CreateSpell()->_ChampName("Olaf")->_MenuName("OlafQ")->_SpellName("OlafAxeThrowCast")->_DangerValue(2)->_MissileName("OlafAxeThrow")->_MissileSpeed(1600)->_Range(1000)->_Radius(90)->_ExtraRange(150)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Orianna
	Spells.push_back(CreateSpell()->_ChampName("Orianna")->_MenuName("OriannaQ")->_SpellName("OrianaIzunaCommand")->_DangerValue(2)->_Delay(0)->_MissileName("OrianaIzuna")->_Range(2000)->_Radius(80)->_MissileSpeed(1200)->_HasEndExplosion(true)->_RadiusEx(230)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Orianna")->_MenuName("OriannaW")->_SpellName("OrianaDissonanceCommand")->_DangerValue(2)->_Delay(10)->_Slot(kSlotW)->_Type(ST_Circle)->_Radius(255));
	Spells.push_back(CreateSpell()->_ChampName("Orianna")->_DangerValue(5)->_MenuName("OriannaR")->_SpellName("OrianaDetonateCommand")->_Delay(500)->_IsDangerous(true)->_Slot(kSlotR)->_Type(ST_Circle)->_Radius(410));
#pragma endregion

#pragma region Pantheon
	Spells.push_back(CreateSpell()->_ChampName("Pantheon")->_DangerValue(2)->_MenuName("PantheonE")->_SpellName("PantheonE")->_Delay(400)->_Slot(kSlotE)->_Type(ST_Cone)->_Range(640)->_Radius(70)->_ExtraDuration(750));
#pragma endregion

#pragma region Poppy
	Spells.push_back(CreateSpell()->_ChampName("Poppy")->_DangerValue(2)->_MenuName("PoppyQ")->_SpellName("PoppyQ")->_Range(430)->_Delay(500)->_ExtraDuration(900)->_FixedRange(true)->_Type(ST_Line)->_Radius(100));
	Spells.push_back(CreateSpell()->_ChampName("Poppy")->_DangerValue(4)->_MenuName("PoppyRInstant")->_SpellName("PoppyRSpellInstant")->_Delay(350)->_Slot(kSlotR)->_Type(ST_Line)->_Range(450)->_Radius(100)->_FixedRange(true)->_IsDangerous(true));
	Spells.push_back(CreateSpell()->_ChampName("Poppy")->_MenuName("PoppyRCharge")->_SpellName("PoppyRSpell")->_DangerValue(3)->_Delay(350)->_MissileName("PoppyRMissile")->_Slot(kSlotR)->_Range(1200)->_MissileSpeed(1600)->_Radius(100)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Quinn
	Spells.push_back(CreateSpell()->_ChampName("Quinn")->_MenuName("QuinnQ")->_MissileName("QuinnQ")->_SpellName("QuinnQ")->_Slot(kSlotQ)->_Type(ST_Line)->_DangerValue(2)->_Delay(313)->_MissileSpeed(1550)->_Range(1050)->_Radius(60)->_AddHitbox(true)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region RekSai
	Spells.push_back(CreateSpell()->_ChampName("RekSai")->_MenuName("RekSaiQ")->_SpellName("RekSaiQBurrowed")->_DangerValue(3)->_Delay(125)->_MissileName("RekSaiQBurrowedMis")->_Range(1500)->_Radius(65)->_MissileSpeed(1950)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("RekSai")->_DangerValue(3)->_MenuName("RekSaiW")->_SpellName("RekSaiW2")->_Delay(10)->_IsDangerous(true)->_Slot(kSlotW)->_Type(ST_Circle)->_Radius(200));
#pragma endregion

#pragma region Renekton
	Spells.push_back(CreateSpell()->_ChampName("Renekton")->_MenuName("RenektonE")->_SpellName("RenektonSliceAndDice")->_DangerValue(2)->_Slot(kSlotE)->_Range(450)->_Delay(0)->_FixedRange(true)->_IsDash(true)->_MissileSpeed(1100)->_Radius(100)->_ExtraSpellNames("RenektonDice"));
#pragma endregion

#pragma region Rengar
	Spells.push_back(CreateSpell()->_ChampName("Rengar")->_MenuName("RengarE")->_MissileName("RengarEMis")->_SpellName("RengarE")->_DangerValue(3)->_MissileSpeed(1500)->_Slot(kSlotE)->_Range(1000)->_Radius(70)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Rengar")->_MenuName("RengarEEmp")->_SpellName("RengarEEmp")->_DangerValue(3)->_MissileName("RengarEEmpMis")->_Slot(kSlotE)->_Range(1000)->_Radius(70)->_MissileSpeed(1500)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Riven
	Spells.push_back(CreateSpell()->_ChampName("Riven")->_DangerValue(3)->_Delay(350)->_MenuName("RivenQ3")->_SpellName("RivenQ3")->_AddHitbox(false)->_IsDangerous(true)->_IsDash(true)->_Type(ST_Circle)->_Radius(260));
	Spells.push_back(CreateSpell()->_ChampName("Riven")->_DangerValue(3)->_MenuName("RivenW")->_SpellName("RivenMartyr")->_Delay(10)->_IsDangerous(true)->_Slot(kSlotW)->_Type(ST_Circle)->_Radius(250));
	Spells.push_back(CreateSpell()->_ChampName("Riven")->_MenuName("RivenR")->_SpellName("RivenIzunaBlade")->_MissileName("RivenWindslashMissileCenter")->_DangerValue(5)->_MissileSpeed(1600)->_Slot(kSlotR)->_Range(1075)->_Radius(100)->_FixedRange(true)->_IsDangerous(true)->_AddHitbox(false)->_MultipleAngle(M_PI)->_MultipleNumber(3)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall)->_ExtraMissileNames("RivenWindslashMissileLeft")->_ExtraMissileNames("RivenWindslashMissileRight"));
#pragma endregion

#pragma region Rumble
	Spells.push_back(CreateSpell()->_ChampName("Rumble")->_MenuName("RumbleE")->_SpellName("RumbleGrenade")->_MissileName("RumbleGrenadeMissile")->_DangerValue(2)->_MissileSpeed(2000)->_Slot(kSlotE)->_Range(950)->_Radius(60)->_DontCheckForDuplicates(true)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall)->_ExtraMissileNames("RumbleGrenadeMissileDangerZone"));
	Spells.push_back(CreateSpell()->_ChampName("Rumble")->_MenuName("RumbleR")->_SpellName("RumbleCarpetBomb")->_DangerValue(4)->_MissileName("RumbleCarpetBombMissile")->_Slot(kSlotR)->_Range(20000)->_Delay(400)->_MissileDelayed(true)->_MissileOnly(true)->_MissileSpeed(1600)->_Radius(150)->_AddHitbox(false)->_CanBeRemoved(false));
#pragma endregion

#pragma region Ryze
	Spells.push_back(CreateSpell()->_ChampName("Ryze")->_MenuName("RyzeQ")->_MissileName("RyzeQ")->_SpellName("RyzeQ")->_DangerValue(2)->_MissileSpeed(1700)->_Range(1000)->_Radius(55)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Sejuani
	Spells.push_back(CreateSpell()->_ChampName("Sejuani")->_DangerValue(3)->_MenuName("SejuaniQ")->_SpellName("SejuaniArcticAssault")->_Delay(0)->_MissileSpeed(1000)->_Range(620)->_Radius(75)->_IsDangerous(true)->_IsDash(true)->_ExtraRange(200)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Sejuani")->_MenuName("SejuaniR")->_SpellName("SejuaniGlacialPrisonCast")->_DangerValue(3)->_MissileName("SejuaniGlacialPrison")->_Slot(kSlotR)->_Range(1100)->_MissileSpeed(1600)->_Radius(110)->_FixedRange(true)->_HasEndExplosion(true)->_IsDangerous(true)->_RadiusEx(500)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Shen
	Spells.push_back(CreateSpell()->_ChampName("Shen")->_DangerValue(3)->_MenuName("ShenE")->_Slot(kSlotE)->_SpellName("ShenE")->_Delay(0)->_IsDangerous(true)->_IsDash(true)->_MissileSpeed(1300)->_Range(600)->_Radius(50)->_ExtraRange(150));
#pragma endregion

#pragma region Shyvana
	Spells.push_back(CreateSpell()->_ChampName("Shyvana")->_MenuName("ShyvanaE")->_SpellName("ShyvanaFireball")->_DangerValue(2)->_MissileName("ShyvanaFireballMissile")->_Slot(kSlotE)->_Range(950)->_MissileSpeed(1575)->_Radius(60)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Shyvana")->_MenuName("ShyvanaEDragon")->_SpellName("ShyvanaFireballDragon2")->_MissileName("ShyvanaFireballDragonMissile")->_DangerValue(3)->_Delay(350)->_MissileSpeed(1575)->_Slot(kSlotE)->_Range(780)->_Radius(60)->_HasEndExplosion(true)->_InfrontStart(200)->_RadiusEx(350)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall)->_ExtraMissileNames("ShyvanaFireballDragonMissileBig")->_ExtraMissileNames("ShyvanaFireballDragonMissileMax"));
	Spells.push_back(CreateSpell()->_ChampName("Shyvana")->_DangerValue(3)->_MenuName("ShyvanaR")->_SpellName("ShyvanaTransformCast")->_Delay(300)->_MissileSpeed(1100)->_Slot(kSlotR)->_Range(950)->_Radius(150)->_ExtraRange(200)->_IsDangerous(true)->_IsDash(true));
#pragma endregion

#pragma region Sion
	Spells.push_back(CreateSpell()->_ChampName("Sion")->_MenuName("SionE")->_MissileName("SionEMissile")->_SpellName("SionE")->_DangerValue(3)->_MissileSpeed(1800)->_Slot(kSlotE)->_Range(800)->_Radius(80)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Sion")->_DangerValue(3)->_MenuName("SionEMinion")->_SpellName("SionEMinion")->_Delay(0)->_IsDangerous(true)->_MissileSpeed(2100)->_Slot(kSlotE)->_Range(1400)->_Radius(60));
	Spells.push_back(CreateSpell()->_ChampName("Sion")->_DangerValue(3)->_MenuName("SionR")->_Slot(kSlotR)->_SpellName("SionR")->_Delay(0)->_MissileSpeed(500)->_Range(500)->_Radius(100)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Sivir
	Spells.push_back(CreateSpell()->_ChampName("Sivir")->_MenuName("SivirQ")->_MissileName("SivirQMissile")->_SpellName("SivirQ")->_DangerValue(2)->_MissileSpeed(1350)->_Range(1250)->_Radius(90)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Sivir")->_MenuName("SivirQReturn")->_SpellName("SivirQReturn")->_DangerValue(2)->_Delay(0)->_MissileName("SivirQMissileReturn")->_Range(20000)->_Radius(100)->_MissileSpeed(1350)->_MissileToUnit(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Skarner
	Spells.push_back(CreateSpell()->_ChampName("Skarner")->_MenuName("SkarnerE")->_SpellName("SkarnerFracture")->_DangerValue(2)->_MissileName("SkarnerFractureMissile")->_Slot(kSlotE)->_Range(1000)->_MissileSpeed(1500)->_Radius(70)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Sona
	Spells.push_back(CreateSpell()->_ChampName("Sona")->_MenuName("SonaR")->_MissileName("SonaR")->_SpellName("SonaR")->_DangerValue(5)->_MissileSpeed(2400)->_Slot(kSlotR)->_Range(1000)->_Radius(140)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Soraka
	Spells.push_back(CreateSpell()->_ChampName("Soraka")->_MenuName("SorakaQ")->_MissileName("SorakaQMissile")->_SpellName("SorakaQ")->_DangerValue(2)->_MissileSpeed(1100)->_Type(ST_Circle)->_Range(800)->_Radius(230)->_CollisionObjects(kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Soraka")->_DangerValue(3)->_MenuName("SorakaE")->_SpellName("SorakaE")->_Delay(1770)->_Slot(kSlotE)->_Type(ST_Circle)->_Range(920)->_Radius(250));
#pragma endregion

#pragma region Swain
	Spells.push_back(CreateSpell()->_ChampName("Swain")->_DangerValue(3)->_MenuName("SwainW")->_SpellName("SwainShadowGrasp")->_Delay(1100)->_Slot(kSlotW)->_Type(ST_Circle)->_Range(900)->_Radius(240)->_IsDangerous(true));
#pragma endregion

#pragma region Syndra
	Spells.push_back(CreateSpell()->_ChampName("Syndra")->_MenuName("SyndraQ")->_MissileName("SyndraQSpell")->_SpellName("SyndraQ")->_DangerValue(2)->_Delay(650)->_Type(ST_Circle)->_Range(825)->_Radius(180)->_MissileDelayed(true));
	Spells.push_back(CreateSpell()->_ChampName("Syndra")->_DangerValue(2)->_MenuName("SyndraW")->_SpellName("SyndraWCast")->_MissileSpeed(1450)->_Slot(kSlotW)->_Type(ST_Circle)->_Range(950)->_Radius(210)->_Delay(250));
	Spells.push_back(CreateSpell()->_ChampName("Syndra")->_DangerValue(2)->_MenuName("SyndraE")->_SpellName("SyndraE")->_MissileSpeed(2000)->_Slot(kSlotE)->_Range(950)->_Radius(100)->_Type(ST_MissileCone)->_DontCross(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Syndra")->_DangerValue(2)->_MenuName("SyndraEMax")->_SpellName("SyndraE5")->_MissileSpeed(2000)->_Slot(kSlotE)->_Range(950)->_Radius(100)->_Type(ST_MissileCone)->_DontCross(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Syndra")->_DangerValue(2)->_MenuName("SyndraEQ")->_SpellName("SyndraEQ")->_Delay(0)->_MissileSpeed(2000)->_Slot(kSlotE)->_Range(1300)->_Radius(55)->_DontCheckForDuplicates(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region TahmKench
	Spells.push_back(CreateSpell()->_ChampName("TahmKench")->_MenuName("TahmKenchQ")->_SpellName("TahmKenchQ")->_DangerValue(3)->_MissileName("TahmKenchQMissile")->_MissileSpeed(2800)->_Range(850)->_Radius(70)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Taliyah
	Spells.push_back(CreateSpell()->_ChampName("Taliyah")->_MenuName("TaliyahQ")->_MissileName("TaliyahQMis")->_SpellName("TaliyahQ")->_DangerValue(2)->_MissileSpeed(3600)->_Range(1000)->_Radius(100)->_FixedRange(true)->_MissileAccel(-5000)->_DisabledByDefault(true)->_MissileMaxSpeed(3600)->_MissileMinSpeed(1500)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Taliyah")->_DangerValue(2)->_MenuName("TaliyahW")->_SpellName("TaliyahWVC")->_Delay(800)->_Slot(kSlotW)->_Type(ST_Circle)->_Range(900)->_Radius(200)->_IsDangerous(true));
#pragma endregion

#pragma region Talon
	Spells.push_back(CreateSpell()->_ChampName("Talon")->_MenuName("TalonW")->_MissileName("TalonWMissileOne")->_SpellName("TalonW")->_DangerValue(2)->_MissileSpeed(2500)->_Slot(kSlotW)->_Range(850)->_Radius(75)->_FixedRange(true)->_IsDangerous(true)->_MultipleAngle(M_PI)->_MultipleNumber(3)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Talon")->_MenuName("TalonWReturn")->_SpellName("TalonWReturn")->_DangerValue(2)->_Delay(0)->_MissileName("TalonWMissileTwo")->_Slot(kSlotW)->_Range(20000)->_MissileSpeed(3000)->_Radius(75)->_IsDangerous(true)->_MissileToUnit(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Talon")->_MenuName("TalonR")->_MissileName("TalonRMisOne")->_SpellName("TalonR")->_DangerValue(2)->_Delay(0)->_MissileSpeed(2400)->_Slot(kSlotR)->_Range(550)->_Radius(140)->_FixedRange(true)->_IsDangerous(true)->_MissileOnly(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Talon")->_MenuName("TalonRReturn")->_SpellName("TalonRReturn")->_DangerValue(2)->_Delay(0)->_MissileName("TalonRMisTwo")->_Slot(kSlotR)->_Range(20000)->_MissileSpeed(4000)->_Radius(140)->_IsDangerous(true)->_MissileToUnit(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Taric
	Spells.push_back(CreateSpell()->_ChampName("Taric")->_DangerValue(3)->_MenuName("TaricE")->_SpellName("TaricE")->_Delay(1000)->_Slot(kSlotE)->_Type(ST_Line)->_Range(650)->_Radius(100)->_FixedRange(true)->_IsDangerous(true)->_MissileFromUnit(true));
#pragma endregion

#pragma region Thresh
	Spells.push_back(CreateSpell()->_ChampName("Thresh")->_MenuName("ThreshQ")->_MissileName("ThreshQMissile")->_SpellName("ThreshQ")->_DangerValue(3)->_Delay(500)->_MissileSpeed(1900)->_Range(1100)->_Radius(70)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Thresh")->_MenuName("ThreshE")->_MissileName("ThreshEMissile1")->_SpellName("ThreshE")->_DangerValue(3)->_Delay(0)->_MissileSpeed(2000)->_Slot(kSlotE)->_Range(1075)->_Radius(110)->_FixedRange(true)->_IsDangerous(true)->_MissileOnly(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Tristana
	Spells.push_back(CreateSpell()->_ChampName("Tristana")->_DangerValue(2)->_MenuName("TristanaW")->_SpellName("TristanaW")->_Delay(300)->_MissileSpeed(1100)->_Slot(kSlotW)->_Range(900)->_Radius(270)->_IsDash(true)->_Type(ST_Circle));
#pragma endregion

#pragma region Trundle
	Spells.push_back(CreateSpell()->_ChampName("Trundle")->_DangerValue(2)->_MenuName("TrundleE")->_SpellName("TrundleCircle")->_Slot(kSlotE)->_Type(ST_Circle)->_Range(1000)->_Radius(125));
#pragma endregion

#pragma region Tryndamere
	Spells.push_back(CreateSpell()->_ChampName("Tryndamere")->_DangerValue(2)->_MenuName("TryndamereE")->_SpellName("TryndamereE")->_Delay(0)->_IsDash(true)->_MissileSpeed(900)->_Slot(kSlotE)->_Range(650)->_Radius(160));
#pragma endregion

#pragma region TwistedFate
	Spells.push_back(CreateSpell()->_ChampName("TwistedFate")->_MenuName("TwistedFateQ")->_SpellName("WildCards")->_DangerValue(2)->_MissileName("SealFateMissile")->_MissileSpeed(1000)->_Range(1450)->_Radius(40)->_FixedRange(true)->_MultipleNumber(3)->_MultipleAngle(M_PI)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Twitch
	Spells.push_back(CreateSpell()->_ChampName("Twitch")->_MenuName("TwitchW")->_SpellName("TwitchVenomCask")->_MissileName("TwitchVenomCaskMissile")->_ToggleName("twitch_w_indicator_(green|red)_team")->_DangerValue(2)->_MissileSpeed(1400)->_Slot(kSlotW)->_Range(950)->_Radius(285)->_Type(ST_Circle)->_ExtraDuration(2850)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Twitch")->_MenuName("TwitchR")->_SpellName("TwitchSprayAndPrayAttack")->_DangerValue(2)->_MissileName("TwitchSprayAndPrayAttack")->_Slot(kSlotR)->_Range(1100)->_Delay(0)->_MissileSpeed(4000)->_Radius(60)->_FixedRange(true)->_MissileOnly(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Urgot
	Spells.push_back(CreateSpell()->_ChampName("Urgot")->_MenuName("UrgotQ")->_SpellName("UrgotHeatseekingMissile")->_DangerValue(2)->_Delay(150)->_MissileName("UrgotHeatseekingLineMissile")->_Range(1000)->_MissileSpeed(1600)->_Radius(60)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Urgot")->_MenuName("UrgotE")->_SpellName("UrgotPlasmaGrenade")->_DangerValue(2)->_MissileName("UrgotPlasmaGrenadeBoom")->_Slot(kSlotE)->_Range(900)->_MissileSpeed(1500)->_Type(ST_Circle)->_Radius(250)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Varus
	Spells.push_back(CreateSpell()->_ChampName("Varus")->_MenuName("VarusQ")->_MissileName("VarusQMissile")->_SpellName("VarusQ")->_DangerValue(2)->_Type(ST_Line)->_Delay(250)->_Slot(kSlotQ)->_MissileSpeed(1850)->_Range(1625)->_Radius(70)->_AddHitbox(true)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Varus")->_MenuName("VarusE")->_MissileName("VarusEMissile")->_SpellName("VarusE")->_DangerValue(2)->_MissileSpeed(1500)->_Slot(kSlotE)->_Range(925)->_Radius(260)->_Type(ST_Circle)->_ExtraDelay(550)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Varus")->_MenuName("VarusR")->_MissileName("VarusRMissile")->_SpellName("VarusR")->_DangerValue(3)->_MissileSpeed(1950)->_Slot(kSlotR)->_Range(1250)->_Radius(120)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Veigar
	Spells.push_back(CreateSpell()->_ChampName("Veigar")->_MenuName("VeigarQ")->_SpellName("VeigarBalefulStrike")->_DangerValue(2)->_MissileName("VeigarBalefulStrikeMis")->_Range(950)->_Radius(70)->_MissileSpeed(2200)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Veigar")->_DangerValue(2)->_MenuName("VeigarW")->_SpellName("VeigarDarkMatter")->_Delay(1250)->_Slot(kSlotW)->_Type(ST_Circle)->_Range(900)->_Radius(225));
	Spells.push_back(CreateSpell()->_ChampName("Veigar")->_DangerValue(3)->_MenuName("VeigarE")->_SpellName("VeigarEventHorizon")->_Delay(800)->_RadiusEx(300)->_Slot(kSlotE)->_Type(ST_Ring)->_Range(700)->_Radius(390)->_DontAddExtraDuration(true)->_DontCross(true)->_ExtraDuration(3000)->_IsDangerous(true));
#pragma endregion

#pragma region Velkoz
	Spells.push_back(CreateSpell()->_ChampName("Velkoz")->_MenuName("VelkozQ")->_MissileName("VelkozQMissile")->_SpellName("VelkozQ")->_DangerValue(2)->_MissileSpeed(1300)->_Range(1100)->_Radius(50)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Velkoz")->_MenuName("VelkozQSplit")->_SpellName("VelkozQSplit")->_DangerValue(2)->_Delay(0)->_MissileName("VelkozQMissileSplit")->_Range(1100)->_Radius(45)->_MissileSpeed(2100)->_FixedRange(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Velkoz")->_MenuName("VelkozW")->_MissileName("VelkozWMissile")->_SpellName("VelkozW")->_DangerValue(2)->_MissileSpeed(1700)->_Slot(kSlotW)->_Range(1200)->_Radius(88)->_BehindStart(100)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Velkoz")->_MenuName("VelkozE")->_MissileName("VelkozEMissile")->_SpellName("VelkozE")->_DangerValue(2)->_MissileSpeed(1500)->_Slot(kSlotE)->_Range(800)->_Radius(225)->_AddHitbox(false)->_ExtraDelay(550)->_Type(ST_Circle));
#pragma endregion

#pragma region Vi
	Spells.push_back(CreateSpell()->_ChampName("Vi")->_DangerValue(3)->_MenuName("ViQ")->_MissileName("ViQMissile")->_SpellName("ViQ")->_Delay(0)->_MissileSpeed(1500)->_Range(750)->_Radius(90)->_IsDangerous(true)->_MissileOnly(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Viktor
	Spells.push_back(CreateSpell()->_ChampName("Viktor")->_DangerValue(3)->_MenuName("ViktorW")->_SpellName("ViktorGravitonField")->_Delay(1500)->_Slot(kSlotW)->_Type(ST_Circle)->_Range(700)->_Radius(300));
	Spells.push_back(CreateSpell()->_ChampName("Viktor")->_MenuName("ViktorE")->_SpellName("ViktorDeathRay")->_MissileName("ViktorDeathRayMissile")->_DangerValue(2)->_Delay(0)->_MissileSpeed(1050)->_Slot(kSlotE)->_Range(710)->_Radius(80)->_FixedRange(true)->_MissileOnly(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall)->_ExtraMissileNames("ViktorEAugMissile"));
	Spells.push_back(CreateSpell()->_ChampName("Viktor")->_MenuName("ViktorEExplosion")->_SpellName("ViktorEExplosion")->_DangerValue(2)->_Delay(1000)->_MissileSpeed(1500)->_Slot(kSlotE)->_Range(710)->_Radius(80)->_FixedRange(true));
#pragma endregion

#pragma region Vladimir
	Spells.push_back(CreateSpell()->_ChampName("Vladimir")->_MenuName("VladimirE")->_SpellName("VladimirE")->_DangerValue(2)->_Delay(0)->_MissileName("VladimirEMissile")->_Slot(kSlotE)->_Range(550)->_MissileSpeed(4000)->_Radius(60)->_FixedRange(true)->_MissileOnly(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Vladimir")->_DangerValue(3)->_MenuName("VladimirR")->_SpellName("VladimirHemoplague")->_Delay(10)->_Slot(kSlotR)->_Type(ST_Circle)->_Range(700)->_Radius(375));
#pragma endregion

#pragma region Warwick
	Spells.push_back(CreateSpell()->_ChampName("Warwick")->_MenuName("WarwickR")->_SpellName("WarwickR")->_DangerValue(5)->_MissileName("WarwickRMissile")->_Slot(kSlotR)->_Range(1300)->_Radius(90)->_MissileSpeed(2175)->_FixedRange(true)->_IsDangerous(true)->_AddHitbox(true)->_Type(ST_Line)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Wukong
	Spells.push_back(CreateSpell()->_ChampName("MonkeyKing")->_MenuName("MonkeyKingR")->_SpellName("MonkeyKingSpinToWin")->_DangerValue(3)->_Delay(50)->_Slot(kSlotR)->_Type(ST_Circle)->_Radius(320)->_DisabledByDefault(true)->_DontCross(true)->_MissileToUnit(true));
#pragma endregion

#pragma region Xerath
	Spells.push_back(CreateSpell()->_ChampName("Xerath")->_DangerValue(2)->_MenuName("XerathQ")->_SpellName("XerathArcanopulse2")->_Delay(530)->_Type(ST_Line)->_Range(20000)->_Radius(100));
	Spells.push_back(CreateSpell()->_ChampName("Xerath")->_DangerValue(2)->_MenuName("XerathW")->_SpellName("XerathArcaneBarrage2")->_Delay(780)->_Slot(kSlotW)->_Type(ST_Circle)->_Range(1100)->_Radius(260));
	Spells.push_back(CreateSpell()->_ChampName("Xerath")->_MenuName("XerathE")->_SpellName("XerathMageSpear")->_DangerValue(2)->_MissileName("XerathMageSpearMissile")->_Slot(kSlotE)->_Range(1100)->_MissileSpeed(1400)->_Radius(60)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Xerath")->_MenuName("XerathR")->_SpellName("XerathRMissileWrapper")->_DangerValue(3)->_Delay(650)->_MissileName("XerathLocusPulse")->_Slot(kSlotR)->_Range(20000)->_IsDangerous(true)->_MissileDelayed(true)->_Type(ST_Circle)->_Radius(200)->_CanBeRemoved(false));
#pragma endregion

#pragma region Yasuo
	Spells.push_back(CreateSpell()->_ChampName("Yasuo")->_MenuName("YasuoQ")->_SpellName("YasuoQ")->_DangerValue(2)->_Delay(400)->_Range(520)->_Radius(55)->_FixedRange(true)->_Invert(true)->_IsDangerous(true)->_Type(ST_Line)->_ExtraSpellNames("YasuoQ2"));
	Spells.push_back(CreateSpell()->_ChampName("Yasuo")->_MenuName("YasuoQ3")->_MissileName("YasuoQ3Mis")->_SpellName("YasuoQ3")->_DangerValue(3)->_Delay(300)->_MissileSpeed(1200)->_Range(1100)->_Radius(90)->_FixedRange(true)->_Invert(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Yorick
	Spells.push_back(CreateSpell()->_ChampName("Yorick")->_DangerValue(2)->_MenuName("YorickW")->_SpellName("YorickW")->_Delay(750)->_Slot(kSlotW)->_Type(ST_Circle)->_Range(600)->_Radius(250)->_DisabledByDefault(true));
	Spells.push_back(CreateSpell()->_ChampName("Yorick")->_DangerValue(2)->_MenuName("YorickE")->_SpellName("YorickE")->_Delay(50)->_Slot(kSlotE)->_Type(ST_Line)->_Range(550)->_Radius(120)->_FixedRange(true));
#pragma endregion

#pragma region Zac
	Spells.push_back(CreateSpell()->_ChampName("Zac")->_DangerValue(2)->_MenuName("ZacQ")->_SpellName("ZacQ")->_Range(550)->_Delay(500)->_FixedRange(true)->_Type(ST_Line)->_Radius(120));
	Spells.push_back(CreateSpell()->_ChampName("Zac")->_DangerValue(3)->_MenuName("ZacE")->_Slot(kSlotE)->_SpellName("ZacE2")->_Delay(0)->_MissileSpeed(1350)->_Type(ST_Circle)->_Range(1800)->_Radius(250)->_IsDangerous(true)->_IsDash(true));
#pragma endregion

#pragma region Zed
	Spells.push_back(CreateSpell()->_ChampName("Zed")->_MenuName("ZedQ")->_MissileName("ZedQMissile")->_SpellName("ZedQ")->_DangerValue(2)->_MissileSpeed(1700)->_Range(925)->_Radius(50)->_FixedRange(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Zed")->_DangerValue(2)->_MenuName("ZedE")->_Slot(kSlotE)->_SpellName("ZedE")->_Delay(10)->_Radius(290));
#pragma endregion

#pragma region Ziggs
	Spells.push_back(CreateSpell()->_ChampName("Ziggs")->_MenuName("ZiggsQ")->_MissileName("ZiggsQSpell")->_SpellName("ZiggsQ")->_DangerValue(2)->_MissileSpeed(1700)->_Range(850)->_Radius(125)->_Type(ST_Circle)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Ziggs")->_MenuName("ZiggsQBounce")->_SpellName("ZiggsQBounce")->_DangerValue(2)->_MissileName("ZiggsQSpell2")->_Delay(400)->_MissileSpeed(1600)->_Range(1100)->_Radius(125)->_Type(ST_Circle)->_MissileDelayed(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall)->_ExtraMissileNames("ZiggsQSpell3"));
	Spells.push_back(CreateSpell()->_ChampName("Ziggs")->_MenuName("ZiggsW")->_MissileName("ZiggsW")->_SpellName("ZiggsW")->_DangerValue(2)->_MissileSpeed(1750)->_Slot(kSlotW)->_Range(1000)->_Radius(275)->_Type(ST_Circle)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Ziggs")->_MenuName("ZiggsE")->_MissileName("ZiggsE2")->_SpellName("ZiggsE")->_DangerValue(2)->_MissileSpeed(1550)->_Slot(kSlotE)->_Range(900)->_Radius(235)->_Type(ST_Circle)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Ziggs")->_MenuName("ZiggsR")->_MissileName("ZiggsRBoom")->_SpellName("ZiggsR")->_DangerValue(5)->_Delay(400)->_MissileSpeed(1500)->_AddHitbox(true)->_Slot(kSlotR)->_Range(5300)->_Radius(500)->_ExtraDelay(1100)->_Type(ST_Circle));
#pragma endregion

#pragma region Zilean
	Spells.push_back(CreateSpell()->_ChampName("Zilean")->_MenuName("ZileanQ")->_MissileName("ZileanQMissile")->_SpellName("ZileanQ")->_DangerValue(2)->_ToggleName("Zilean_.+_Q_TimeBombGround(Green|Red)")->_Range(900)->_Radius(150)->_Delay(250)->_MissileSpeed(2000)->_Type(ST_Circle)->_ExtraDuration(3100)->_DontCross(true)->_ExtraDelay(450)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
#pragma endregion

#pragma region Zyra
	Spells.push_back(CreateSpell()->_ChampName("Zyra")->_DangerValue(2)->_MenuName("ZyraQ")->_SpellName("ZyraQ")->_Range(800)->_Delay(850)->_Perpendicular(true)->_RadiusEx(450)->_Type(ST_Line)->_Radius(140));
	Spells.push_back(CreateSpell()->_ChampName("Zyra")->_MenuName("ZyraE")->_MissileName("ZyraE")->_SpellName("ZyraE")->_DangerValue(3)->_MissileSpeed(1150)->_Slot(kSlotE)->_Range(1150)->_Radius(70)->_FixedRange(true)->_IsDangerous(true)->_CollisionObjects(kCollidesWithHeroes | kCollidesWithYasuoWall));
	Spells.push_back(CreateSpell()->_ChampName("Zyra")->_DangerValue(5)->_MenuName("ZyraR")->_Slot(kSlotR)->_SpellName("ZyraR")->_Delay(2200)->_Type(ST_Circle)->_Range(700)->_Radius(520)->_MissileSpeed(INT_MAX));
#pragma endregion

}