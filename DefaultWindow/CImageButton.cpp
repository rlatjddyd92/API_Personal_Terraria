#include "stdafx.h"
#include "CImageButton.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CTileMgr.h"

CImageButton::CImageButton() : m_Function(BUTTONFUNCTION::END), m_bCursor(false)
{
}

CImageButton::~CImageButton()
{
	Release();
}

void CImageButton::Initialize()
{
	m_tInfo.fCX = 50;
	m_tInfo.fCY = 50;
}

void CImageButton::Update()
{
	__super::Update_Rect();
}

OBJRESULT CImageButton::Late_Update()
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
			case BUTTONFUNCTION::SAVE:
				CTileMgr::Get_Instance()->Save_Map();
				break;
			case BUTTONFUNCTION::LOAD:
				CTileMgr::Get_Instance()->Load_Map();
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

void CImageButton::Render(HDC hDC)
{
	if (!m_bCursor)
	{
		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	}
	else
	{
		HPEN hNewPen = CreatePen(PS_SOLID, 1, RGB(255, 153, 0));
		HPEN hOldPen = (HPEN)SelectObject(hDC, hNewPen);

		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

		SelectObject(hDC, hOldPen);
		DeleteObject(hNewPen);
	}

	TCHAR	szCnt[32] = L"";

	switch (m_Function)
	{
	case BUTTONFUNCTION::SAVE:
		swprintf_s(szCnt, L"SAVE");
		break;
	case BUTTONFUNCTION::LOAD:
		swprintf_s(szCnt, L"LOAD");
		break;
	case BUTTONFUNCTION::EXIT:
		swprintf_s(szCnt, L"EXIT");
		break;
	case BUTTONFUNCTION::MAINMENU:
		swprintf_s(szCnt, L"BACK");
		break;
	case BUTTONFUNCTION::END:
		break;
	default:
		break;
	}

	SetBkMode(hDC, TRANSPARENT);
	SetTextAlign(hDC, TA_CENTER);
	TextOut(hDC, m_tInfo.fX, m_tInfo.fY - 8, szCnt, lstrlen(szCnt));
}

void CImageButton::Release()
{
}

void CImageButton::Collision_Rect_Obj(CObj* opponent)
{
}

void CImageButton::Collision_Eillpse_Obj(CObj* opponent)
{
}

void CImageButton::Set_Function(BUTTONFUNCTION input)
{
	m_Function = input;
}