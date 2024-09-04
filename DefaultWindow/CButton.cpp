#include "stdafx.h"
#include "CButton.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"

CButton::CButton() : m_Function(BUTTONFUNCTION::END), m_bCursor(false)
{
}

CButton::~CButton()
{
	Release();
}

void CButton::Initialize()
{
	m_tInfo.fCX = 200;
	m_tInfo.fCY = 50;
}

void CButton::Update()
{
	__super::Update_Rect();
}

OBJRESULT CButton::Late_Update()
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		m_bCursor = true;

		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			switch (m_Function)
			{
			case BUTTONFUNCTION::INGAME:
				CSceneMgr::Get_Instance()->Set_NowScene(SCENETYPE::INGAME);
				break;
			case BUTTONFUNCTION::EDIT:
				CSceneMgr::Get_Instance()->Set_NowScene(SCENETYPE::EDITER);
				break;
			case BUTTONFUNCTION::EXIT:
				CSceneMgr::Get_Instance()->Set_NowScene(SCENETYPE::EXIT);
				break;
			case BUTTONFUNCTION::MAINMENU:
				CSceneMgr::Get_Instance()->Set_NowScene(SCENETYPE::MAINMENU);
				break;
			case BUTTONFUNCTION::END:
				break;
			default:
				break;
			}
			
			return OBJRESULT::ACTIVE;
		}
	}
	else
		m_bCursor = false;

	return OBJRESULT::NONE;
}

void CButton::Render(HDC hDC)
{
	if (m_bCursor)
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

	TCHAR	szCnt[32] = L"";

	switch (m_Function)
	{
	case BUTTONFUNCTION::INGAME:
		//swprintf_s(szCnt, L"GAME START");
		swprintf_s(szCnt, L"게임 시작");
		break;
	case BUTTONFUNCTION::EDIT:
		//swprintf_s(szCnt, L"MAP EDITOR");
		swprintf_s(szCnt, L"맵 에디터");
		break;
	case BUTTONFUNCTION::EXIT:
		//swprintf_s(szCnt, L"EXIT");
		swprintf_s(szCnt, L"게임 종료");
		break;
	case BUTTONFUNCTION::MAINMENU:
		swprintf_s(szCnt, L"MAIN MENU");
		break;
	case BUTTONFUNCTION::END:
		break;
	default:
		break;
	}

	SetBkMode(hDC, TRANSPARENT);
	SetTextAlign(hDC, TA_CENTER);
	TextOut(hDC, m_tInfo.fX, m_tInfo.fY-8, szCnt, lstrlen(szCnt));
}

void CButton::Release()
{

}

void CButton::Collision_Rect_Obj(CObj* opponent)
{
}

void CButton::Collision_Eillpse_Obj(CObj* opponent)
{
}

void CButton::Set_Function(BUTTONFUNCTION input)
{
	m_Function = input;
}
