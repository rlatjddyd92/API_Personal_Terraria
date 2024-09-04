#include "stdafx.h"
#include "CInven.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "CFunitureMgr.h"

void CInven::Initialize() 
{
	m_tInfo = { 0,0,INVENC,INVENC };

	if (m_InvenType == INVENTYPE::FUNITURE_INPUT)
	{
		m_tInfo.fCX = INVENC * 0.8f;
		m_tInfo.fCY = INVENC * 0.8f;
	}
}

void CInven::Update()
{
	
}

OBJRESULT CInven::Late_Update()
{
	__super::Update_Rect();

	if ((m_pContainItem) && (0 >= dynamic_cast<CItem*>(m_pContainItem)->Get_Count()))
		Reset_Item_Obj();

	return OBJRESULT::NONE;
}

void CInven::Render(HDC hDC)
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hItem = nullptr;
		
	if (!m_bEquipment)
		hItem = CBmpMgr::Get_Instance()->Find_Img(L"Inventory_Back");
	else 
		hItem = CBmpMgr::Get_Instance()->Find_Img(L"Inventory_Back2");

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		m_tInfo.fCX,
		m_tInfo.fCY,
		hItem,
		0,
		0,
		52,
		52,
		RGB(255, 255, 255));

	if (m_pContainItem)
		dynamic_cast<CItem*>(m_pContainItem)->Render_Inven(hDC);

	if (-1 != m_iQuickNum)
	{
		TCHAR	szCnt[32] = L"";
		SetTextColor(hDC, RGB(255, 255, 0));
		swprintf_s(szCnt, L"%d", (m_iQuickNum + 1) % 10);
		TextOut(hDC, m_tRect.left + 5, m_tRect.top, szCnt, lstrlen(szCnt));
		SetTextColor(hDC, RGB(0, 0, 0));
	}

	if (true == m_bSelected)
	{
		HPEN hNewPen = CreatePen(PS_SOLID, 1, RGB(255, 153, 0));
		HPEN hOldPen = (HPEN)SelectObject(hDC, hNewPen);

		HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);

		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

		SelectObject(hDC, hOldPen);
		DeleteObject(hNewPen);

		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);
	}
}

void CInven::Collision_Rect_Obj(CObj* opponent)
{
}

void CInven::Collision_Eillpse_Obj(CObj* opponent)
{
}
