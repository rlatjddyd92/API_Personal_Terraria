#include "stdafx.h"
#include "CAchievment.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CInven.h"
#include "CInvenMgr.h"
#include "CSceneMgr.h"

void CAchievment::Initialize()
{
	

	switch (m_Achievement)
	{
	case ACHIEVMENT::MAKE_FUNANCE:
		m_tInfo = { 600,50,200,32 };
		m_tGoal = L"화로를 제작하세요.";
		CObjMgr::Get_Instance()->Add_Object(OBJID::INVEN, CAbstractFactory<CInven>().Create(m_tInfo.fX - 50, m_tInfo.fY));
		CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>().Create_Item(L"Gun", ITEMTYPE::GUN, false));
		dynamic_cast<CInven*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::INVEN))->Put_in_Item(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));
		break;
	case ACHIEVMENT::MAKE_WORKSTATION:
		m_tInfo = { 600,100,200,32 };
		m_tGoal = L"대형작업대를 제작하세요.";
		CObjMgr::Get_Instance()->Add_Object(OBJID::INVEN, CAbstractFactory<CInven>().Create(m_tInfo.fX - 50, m_tInfo.fY));
		CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>().Create_Item(L"Heal", ITEMTYPE::POTION, false));
		dynamic_cast<CInven*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::INVEN))->Put_in_Item(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));
		break;
	case ACHIEVMENT::SURVIVE:
		m_tInfo = { 600,150,200,32 };
		m_tGoal = L"생존의 밤에서 살아남으세요.";
		CObjMgr::Get_Instance()->Add_Object(OBJID::INVEN, CAbstractFactory<CInven>().Create(m_tInfo.fX - 50, m_tInfo.fY));
		CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>().Create_Item(L"SuperJump", ITEMTYPE::SUPERJUMP, false));
		dynamic_cast<CInven*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::INVEN))->Put_in_Item(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));
		break;
	case ACHIEVMENT::END:
		break;
	default:
		break;
	}



}

void CAchievment::Update()
{


}

OBJRESULT CAchievment::Late_Update()
{
	return OBJRESULT();
}

void CAchievment::Render(HDC hDC)
{
}

void CAchievment::Release()
{
}

void CAchievment::Collision_Rect_Obj(CObj* opponent)
{
}

void CAchievment::Collision_Eillpse_Obj(CObj* opponent)
{
}

void CAchievment::Check_Sucssess()
{
	switch (m_Achievement)
	{
	case ACHIEVMENT::MAKE_FUNANCE:
		if (CInvenMgr::Get_Instance()->Get_Item_by_Key(L"Furnace"))
			m_bSucssess = true;
		break;
	case ACHIEVMENT::MAKE_WORKSTATION:
		if (CInvenMgr::Get_Instance()->Get_Item_by_Key(L"WorkStation"))
			m_bSucssess = true;
		break;
	case ACHIEVMENT::SURVIVE:
		if ((BACKGROUND::SURVIVAL == m_NowBack) && (BACKGROUND::NORMAL == CSceneMgr::Get_Instance()->Get_Ingame_BackGround()))
			m_bSucssess = true;
		else
			m_NowBack = CSceneMgr::Get_Instance()->Get_Ingame_BackGround();
		break;
	case ACHIEVMENT::END:
		break;
	default:
		break;
	}
}

void CAchievment::Check_Click()
{
}
