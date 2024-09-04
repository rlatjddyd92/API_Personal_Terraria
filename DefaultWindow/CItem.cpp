#include "stdafx.h"
#include "CItem.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CCollisionMgr.h"
#include "CTileMgr.h"
#include "CInvenMgr.h"
#include "CMessage.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"

void CItem::Initialize()
{
	m_tInfo = { WINCX / 2.f, WINCY / 2.f, INVENC - 5, INVENC - 5 };
	if (ITEMTYPE::TILE == m_Type)
	{
		m_tInfo.fCX = 16;
		m_tInfo.fCY = 16;
		m_bCanStack = true;
	}

	m_AdjustFuniture();

	m_hItemSkin = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);

	if (ITEMTYPE::TILE == m_Type)
		m_iCount = 10;

	if (ITEMTYPE::FUNITURE == m_Type)
	{
		if (m_pItemKey == L"Door")
		{
			m_FType = FUNITURETYPE::DOOR;
		}
		else if (m_pItemKey == L"Furnace")
		{
			m_FType = FUNITURETYPE::FURNACE;
		}
		else if (m_pItemKey == L"WorkDesk")
		{
			m_FType = FUNITURETYPE::WORKDESK;
		}
		else if (m_pItemKey == L"WorkStation")
		{
			m_FType = FUNITURETYPE::WORKSTATION;
		}
	}

	if (ITEMTYPE::SUPERJUMP == m_Type)
	{
		m_tInfo.fX = 33;
		m_tInfo.fY = 27;
		m_hItemSkin = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);
		m_bEquip = true;
	}

	if (ITEMTYPE::POTION == m_Type)
	{
		m_tInfo.fX = 34;
		m_tInfo.fY = 34;
		m_hItemSkin = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);
		m_bEquip = true;
	}



	CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_ToolTip(m_pItemKey, m_tInfo));
	m_pMessage = CObjMgr::Get_Instance()->Get_LastObj(OBJID::MESSAGE);
}

void CItem::Update()
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (ITEMMODE::PICKUP == m_Mode)
	{
		m_tInfo.fX = pt.x;
		m_tInfo.fY = pt.y;
	}
	else if (ITEMMODE::GROUND == m_Mode)
	{
		m_tInfo.fX += m_vVelocity.fX;
		m_tInfo.fY += m_vVelocity.fY;
	}

	if (0 < m_iGround_Delay)
		--m_iGround_Delay;

	m_pMessage->Set_Pos(m_tInfo.fX, m_tInfo.fY);
}

OBJRESULT CItem::Late_Update()
{
	if (m_bDead)
		return OBJRESULT::DEAD;

	if (0 == m_iCount)
		return OBJRESULT::DEAD;

	if (ITEMMODE::GROUND == m_Mode)
	{
		m_tInfo = CCollisionMgr::Get_Instance()->Collision_MapBorder(this);
		m_tInfo = CTileMgr::Get_Instance()->Coillsion_Tile(this);

		if (!CTileMgr::Get_Instance()->IsGround(this))
			m_vVelocity.fY += m_fGravity;
		else
		{
			m_vVelocity.fY = 0;
			m_vVelocity.fX = 0;
		}
	}

	

	__super::Update_Rect();

	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
		if (m_pMessage)
			dynamic_cast<CMessage*>(m_pMessage)->Set_ItemOpen();

	return OBJRESULT::NONE;
}

void CItem::Render(HDC hDC)
{
	if (ITEMMODE::GROUND == m_Mode)
	{
		float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
		float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

		if (ITEMTYPE::TILE == m_Type)
		{
			HDC hTile = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);
			if (hTile)
				GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, TILECX, TILECY, hTile, 18 * 11, 18 * 3, 16, 16, RGB(255, 255, 255));
		}
		else if (ITEMTYPE::THROW == m_Type)
		{
			HDC hTile = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);

			if (m_bFIreSpray)
				GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, 16, 8, hTile, 0, 0, 16, 8, RGB(255, 255, 255));
			else if (m_bHighExp)
				GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, 16, 8, hTile, 0, 8, 16, 8, RGB(255, 255, 255));
			else
				GdiTransparentBlt(hDC, m_tInfo.fX - 11 + ScrollX, m_tInfo.fY - 15 + ScrollY, 22, 30, hTile, 0, 0, 22, 30, RGB(255, 255, 255));
		}
		else if (ITEMTYPE::FUNITURE == m_Type)
		{
			HDC hFuni = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);

			if (m_pItemKey == L"Door")
			{
				GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, m_tInfo.fCX, m_tInfo.fCY, hFuni, 0, 0, 18, 32, RGB(255, 255, 255));
			}
			else if (m_pItemKey == L"Furnace")
			{
				GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, m_tInfo.fCX, m_tInfo.fCY, hFuni, 0, 0, 48, 48, RGB(255, 255, 255));
			}
			else if (m_pItemKey == L"WorkDesk")
			{
				GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, m_tInfo.fCX, m_tInfo.fCY, hFuni, 0, 0, 32, 32, RGB(255, 255, 255));
			}
			else if (m_pItemKey == L"WorkStation")
			{
				GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, m_tInfo.fCX, m_tInfo.fCY, hFuni, 0, 0, 48, 48, RGB(255, 255, 255));
			}
		}
		else
		{
			HDC hItem = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);

			if ((ITEMTYPE::FIRETHROWER == m_Type) || (ITEMTYPE::GUN == m_Type))
				GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, m_tInfo.fCX, m_tInfo.fCY, hItem, 0, 0, 56, 56, RGB(255, 255, 255));
			else if ((ITEMTYPE::MELEE == m_Type) || (ITEMTYPE::PICK == m_Type))
				GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, m_tInfo.fCX, m_tInfo.fCY, hItem, 0, 0, 56, 56, RGB(255, 255, 255));
			else if (ITEMTYPE::SUPERJUMP == m_Type)
				GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, m_tInfo.fCX, m_tInfo.fCY, hItem, 0, 0, 22, 18, RGB(255, 255, 255));
			else if (ITEMTYPE::POTION == m_Type)
				GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, m_tInfo.fCX, m_tInfo.fCY, hItem, 0, 0, 34, 34, RGB(255, 255, 255));
			else
				GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, m_tInfo.fCX, m_tInfo.fCY, hItem, 0, 0, 34, 34, RGB(255, 255, 255));
		}
			

		//Show_Initial(hDC);
		if (m_bCanStack)
			Show_Count(hDC);
	}
	else if (ITEMMODE::PICKUP == m_Mode)
	{
		
		if (ITEMTYPE::TILE == m_Type)
		{
			HDC hTile = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);
			if (hTile)
				GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, TILECX, TILECY, hTile, 18 * 11, 18 * 3, 16, 16, RGB(255, 255, 255));
		}
		else if (ITEMTYPE::THROW == m_Type)
		{
			HDC hTile = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);

			if (m_bFIreSpray)
				GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, 16, 8, hTile, 0, 0, 16, 8, RGB(255, 255, 255));
			else if (m_bHighExp)
				GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, 16, 8, hTile, 0, 8, 16, 8, RGB(255, 255, 255));
			else 
				GdiTransparentBlt(hDC, m_tInfo.fX - 11, m_tInfo.fY - 15, 22, 30, hTile, 0, 0, 22, 30, RGB(255, 255, 255));
		}
		else if (ITEMTYPE::FUNITURE == m_Type)
		{
			HDC hFuni = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);

			if (m_pItemKey == L"Door")
			{
				GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hFuni, 0, 0, 18, 32, RGB(255, 255, 255));
			}
			else if (m_pItemKey == L"Furnace")
			{
				GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hFuni, 0, 0, 48, 48, RGB(255, 255, 255));
			}
			else if (m_pItemKey == L"WorkDesk")
			{
				GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hFuni, 0, 0, 32, 32, RGB(255, 255, 255));
			}
			else if (m_pItemKey == L"WorkStation")
			{
				GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hFuni, 0, 0, 48, 48, RGB(255, 255, 255));
			}
		}
		else
		{
			HDC hItem = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);

			if ((ITEMTYPE::FIRETHROWER == m_Type) || (ITEMTYPE::GUN == m_Type))
				GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hItem, 0, 0, 56, 56, RGB(255, 255, 255));
			else if ((ITEMTYPE::MELEE == m_Type) || (ITEMTYPE::PICK == m_Type))
				GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hItem, 0, 0, 56, 56, RGB(255, 255, 255));
			else if (ITEMTYPE::SUPERJUMP == m_Type)
				GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hItem, 0, 0, 22, 18, RGB(255, 255, 255));
			else if (ITEMTYPE::POTION == m_Type)
				GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hItem, 0, 0, 34, 34, RGB(255, 255, 255));
			else
				GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hItem, 0, 0, 34, 34, RGB(255, 255, 255));
		}

		//Show_Initial(hDC);

		if (m_bCanStack)
			Show_Count(hDC);
	}

	
}

void CItem::Release()
{
	ReleaseDC(g_hWnd, m_hItemSkin);
}

void CItem::Collision_Rect_Obj(CObj* opponent)
{
	if ((ITEMMODE::GROUND != m_Mode) || (0 != m_iGround_Delay))
		return;

	if (OBJID::PLAYER == opponent->Get_OBJID())
	{
		bool bResult = CInvenMgr::Get_Instance()->Put_In_Inven_Direct(0, this);

		if (bResult)
			m_Mode = ITEMMODE::INVEN;
	}
}

void CItem::Collision_Eillpse_Obj(CObj* opponent)
{
	if (ITEMMODE::GROUND != m_Mode)
		return;
}

void CItem::Render_Inven(HDC hDC)
{
	if (ITEMTYPE::TILE == m_Type)
	{
		HDC hTile = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);
		if (hTile)
			GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, TILECX, TILECY, hTile, 18 * 11, 18 * 3, 16, 16, RGB(255, 255, 255));
	}
	else if (ITEMTYPE::THROW == m_Type)
	{
		HDC hTile = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);

		if (m_bFIreSpray)
			GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, 16, 8, hTile, 0, 0, 16, 8, RGB(255, 255, 255));
		else if (m_bHighExp)
			GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, 16, 8, hTile, 0, 8, 16, 8, RGB(255, 255, 255));
		else
			GdiTransparentBlt(hDC, m_tInfo.fX - 11, m_tInfo.fY - 15, 22, 30, hTile, 0, 0, 22, 30, RGB(255, 255, 255));
	}
	else if (ITEMTYPE::FUNITURE == m_Type)
	{
		HDC hFuni = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);

		if (m_pItemKey == L"Door")
		{
			GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hFuni, 0, 0, 18, 32, RGB(255, 255, 255));
		}
		else if (m_pItemKey == L"Furnace")
		{
			GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hFuni, 0, 0, 48, 48, RGB(255, 255, 255));
		}
		else if (m_pItemKey == L"WorkDesk")
		{
			GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hFuni, 0, 0, 32, 32, RGB(255, 255, 255));
		}
		else if (m_pItemKey == L"WorkStation")
		{
			GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hFuni, 0, 0, 48, 48, RGB(255, 255, 255));
		}
	}
	else
	{
		HDC hItem = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);

		if ((ITEMTYPE::FIRETHROWER == m_Type) || (ITEMTYPE::GUN == m_Type))
			GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hItem, 0, 0, 56, 56, RGB(255, 255, 255));
		else if ((ITEMTYPE::MELEE == m_Type) || (ITEMTYPE::PICK == m_Type))
			GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hItem, 0, 0, 56, 56, RGB(255, 255, 255));
		else if (ITEMTYPE::SUPERJUMP == m_Type)
			GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hItem, 0, 0, 22, 18, RGB(255, 255, 255));
		else if (ITEMTYPE::POTION == m_Type)
			GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hItem, 0, 0, 34, 34, RGB(255, 255, 255));
		else
			GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hItem, 0, 0, 34, 34, RGB(255, 255, 255));
	}

	//Show_Initial(hDC);

	if (m_bCanStack)
		Show_Count(hDC);

	
}

void CItem::Show_Initial(HDC hDC)
{
	TCHAR	szCnt[32] = L"";

	switch (m_Type)
	{
	case ITEMTYPE::MELEE:
		swprintf_s(szCnt, L"MEL");
		break;
	case ITEMTYPE::GUN:
		swprintf_s(szCnt, L"GUN");
		break;
	case ITEMTYPE::BOW:
		swprintf_s(szCnt, L"BOW");
		break;
	case ITEMTYPE::PICK:
		swprintf_s(szCnt, L"PICK");
		break;
	case ITEMTYPE::SUPERJUMP:
		swprintf_s(szCnt, L"SJUMP");
		break;
	case ITEMTYPE::FIRETHROWER:
		swprintf_s(szCnt, L"FIRE");
		break;
	case ITEMTYPE::AXE:
		swprintf_s(szCnt, L"AXE");
		break;
	default:
		break;
	}

	swprintf_s(szCnt, szCnt);

	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (ITEMMODE::GROUND != m_Mode)
		TextOut(hDC, m_tInfo.fX - 5, m_tInfo.fY, szCnt, lstrlen(szCnt));
	else
		TextOut(hDC, m_tInfo.fX - 5 + ScrollX, m_tInfo.fY + ScrollY, szCnt, lstrlen(szCnt));
}

void CItem::Show_Count(HDC hDC)
{
	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	SetTextColor(hDC, RGB(0, 255, 0));
	TCHAR	szCnt[32] = L"";
	swprintf_s(szCnt, L"%d", m_iCount);

	if (ITEMMODE::GROUND != m_Mode)
		TextOut(hDC, m_tInfo.fX + 5, m_tInfo.fY, szCnt, lstrlen(szCnt));
	else
		TextOut(hDC, m_tInfo.fX + 5 + ScrollX, m_tInfo.fY + ScrollY, szCnt, lstrlen(szCnt));
	SetTextColor(hDC, RGB(0, 0, 0));
}

void CItem::m_AdjustFuniture()
{
	if (ITEMTYPE::FUNITURE == m_Type)
	{
		m_bCanStack = false;

		if (m_pItemKey == L"Door")
		{
			m_FType = FUNITURETYPE::DOOR;

			m_tInfo.fCX = 18 * 0.7f;
			m_tInfo.fCY = 32 * 0.7f;
		}
		else if (m_pItemKey == L"Furnace")
		{
			m_FType = FUNITURETYPE::FURNACE;

			m_tInfo.fCX = 48 * 0.7f;
			m_tInfo.fCY = 48 * 0.7f;
		}
		else if (m_pItemKey == L"WorkDesk")
		{
			m_FType = FUNITURETYPE::FURNACE;

			m_tInfo.fCX = 32 * 0.7f;
			m_tInfo.fCY = 32 * 0.7f;
		}
		else if (m_pItemKey == L"WorkStation")
		{
			m_FType = FUNITURETYPE::FURNACE;

			m_tInfo.fCX = 48 * 0.7f;
			m_tInfo.fCY = 48 * 0.7f;
		}

		return;
	}
}

