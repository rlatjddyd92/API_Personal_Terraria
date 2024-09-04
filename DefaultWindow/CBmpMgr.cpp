#include "stdafx.h"
#include "CBmpMgr.h"

CBmpMgr* CBmpMgr::m_pInstance = nullptr;

CBmpMgr::CBmpMgr()
{
}

CBmpMgr::~CBmpMgr()
{
	Release();
}



void CBmpMgr::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Plg.bmp", L"Plg");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/InGameMem.bmp", L"InGameMem");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/InGame.bmp", L"InGame");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Terraria.bmp", L"Terraria");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Logo3.bmp", L"Logo3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Dirt.bmp", L"Dirt");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Copper.bmp", L"Copper");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Gold.bmp", L"Gold");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Iron.bmp", L"Iron");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Rock.bmp", L"Rock");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Sand.bmp", L"Sand");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Silver.bmp", L"Silver");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Grass.bmp", L"Grass");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/BedRock.bmp", L"BedRock");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Prison.bmp", L"Prison");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Tree_2.bmp", L"Tree_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Wooden.bmp", L"Wooden");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Coal.bmp", L"Coal");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Mine1.bmp", L"Mine1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Mine2.bmp", L"Mine2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Tree_Tops.bmp", L"Tree_Tops");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Player_Idle_L.bmp", L"Player_Idle_L");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Player_Idle_R.bmp", L"Player_Idle_R");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Player_Using_L.bmp", L"Player_Using_L");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Player_Using_R.bmp", L"Player_Using_R");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Player_Walk_L.bmp", L"Player_Walk_L");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Player_Walk_R.bmp", L"Player_Walk_R");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Inventory_Back.bmp", L"Inventory_Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Inventory_Back2.bmp", L"Inventory_Back2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Crack.bmp", L"Crack");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Heart_Left.bmp", L"Heart_Left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Heart_Middle.bmp", L"Heart_Middle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Heart_Right_Fancy.bmp", L"Heart_Right_Fancy");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Heart_Fill.bmp", L"Heart_Fill");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/HealthBar1.bmp", L"HealthBar1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_BossBar.bmp", L"UI_BossBar");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_BossBar_Back.bmp", L"UI_BossBar_Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_BossBar_Fill.bmp", L"UI_BossBar_Fill");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Time.bmp", L"Time");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Dynamaite_2Phase.bmp", L"Dynamaite_2Phase");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/Survival.bmp", L"Survival");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Bomb.bmp", L"Bomb");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Missile/Nuke.bmp", L"Nuke");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Missile/Nuke1.bmp", L"Nuke1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Missile/Exp1.bmp", L"Exp1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Missile/Slow_Missile.bmp", L"Slow_Missile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Missile/Blue_Light.bmp", L"Blue_Light");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Missile/Slow_Missile1.bmp", L"Slow_Missile1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Missile/Exp2.bmp", L"Exp2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Missile/Magic_Missile.bmp", L"Magic_Missile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Missile/Dounut_Bullet.bmp", L"Dounut_Bullet");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Missile/Energy_Missile.bmp", L"Energy_Missile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Missile/Energy_Bounce.bmp", L"Energy_Bounce");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Moon_Lord.bmp", L"Moon_Lord");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_Fuel_Fill.bmp", L"UI_Fuel_Fill");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Mine.bmp", L"Mine");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Flame.bmp", L"Flame");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Flame_Small.bmp", L"Flame_Small");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Air.bmp", L"Air");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/AIr_Exp.bmp", L"AIr_Exp");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Air_Range.bmp", L"Air_Range");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Air_Range_Exp.bmp", L"Air_Range_Exp");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Ground.bmp", L"Ground");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Ground_Exp.bmp", L"Ground_Exp");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Ground_Range.bmp", L"Ground_Range");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Ground_Range_Exp.bmp", L"Ground_Range_Exp");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Door.bmp", L"Door");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Furnace.bmp", L"Furnace");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/WorkDesk.bmp", L"WorkDesk");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/WorkStation.bmp", L"WorkStation");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Axe.bmp", L"Axe");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Bow.bmp", L"Bow");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/CopperBar.bmp", L"CopperBar");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/FireThrower.bmp", L"FireThrower");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/GoldBar.bmp", L"GoldBar");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Gun.bmp", L"Gun");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/IronBar.bmp", L"IronBar");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Pick.bmp", L"Pick");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/SilverBar.bmp", L"SilverBar");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Melee.bmp", L"Melee");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Heal.bmp", L"Heal");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/SuperJump.bmp", L"SuperJump");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Eilen_Boss.bmp", L"Eilen_Boss");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/WorkStation.bmp", L"WorkStation");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Text_Back.bmp", L"Text_Back");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/FireThrower_1.bmp", L"FireThrower_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/FireThrower_2.bmp", L"FireThrower_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/FireThrower_3.bmp", L"FireThrower_3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/FireThrower_4.bmp", L"FireThrower_4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/FireThrower_5.bmp", L"FireThrower_5");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/FireThrower_6.bmp", L"FireThrower_6");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/FireThrower_7.bmp", L"FireThrower_7");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/FireThrower_8.bmp", L"FireThrower_8");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Gun_Using_1.bmp", L"Gun_Using_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Gun_Using_2.bmp", L"Gun_Using_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Gun_Using_3.bmp", L"Gun_Using_3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Gun_Using_4.bmp", L"Gun_Using_4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Gun_Using_5.bmp", L"Gun_Using_5");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Gun_Using_6.bmp", L"Gun_Using_6");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Gun_Using_7.bmp", L"Gun_Using_7");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Gun_Using_8.bmp", L"Gun_Using_8");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Axe_1.bmp", L"Axe_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Pick_1.bmp", L"Pick_1");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Axe_2.bmp", L"Axe_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Pick_2.bmp", L"Pick_2");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Axe_1_L.bmp", L"Axe_1_L");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Pick_1_L.bmp", L"Pick_1_L");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Axe_2_L.bmp", L"Axe_2_L");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Pick_2_L.bmp", L"Pick_2_L");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player,Mon/Eilen_Boss.bmp", L"Eilen_Boss");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Buff/Heal_Icon.bmp", L"Heal_Icon");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Mouse.bmp", L"Mouse");
}

void CBmpMgr::Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagFinder(pImgKey));

	if (iter == m_mapBit.end())
	{
		CMyBmp* pBmp = new CMyBmp;
		pBmp->Load_Bmp(pFilePath);

		m_mapBit.insert({ pImgKey, pBmp });
	}

}

HDC CBmpMgr::Find_Img(const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagFinder(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_MemDC();
}

void CBmpMgr::Release()
{
	for_each(m_mapBit.begin(), m_mapBit.end(), [](auto& Pair)
		{
			if (Pair.second)
			{
				Safe_Delete(Pair.second);
				Pair.second = nullptr;
			}
		});

	m_mapBit.clear();

}