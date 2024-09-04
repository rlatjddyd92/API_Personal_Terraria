#include "stdafx.h"
#include "CEdit.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CImageButton.h"
#include "CKeyMgr.h"
#include "CTileMgr.h"
#include "CBmpMgr.h"
#include "CMouse.h"

void CEdit::Initialize()
{
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CImageButton>().Create(600, 25));
	dynamic_cast<CImageButton*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::UI))->Set_Function(BUTTONFUNCTION::SAVE);
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CImageButton>().Create(650, 25));
	dynamic_cast<CImageButton*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::UI))->Set_Function(BUTTONFUNCTION::LOAD);
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CImageButton>().Create(700, 25));
	dynamic_cast<CImageButton*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::UI))->Set_Function(BUTTONFUNCTION::MAINMENU);
	CScrollMgr::Get_Instance()->Reset_Scroll();
	CObjMgr::Get_Instance()->Add_Object(OBJID::MOUSE, CAbstractFactory<CMouse>().Create());
}

void CEdit::Update()
{
	CObjMgr::Get_Instance()->Update(OBJID::UI);
	CObjMgr::Get_Instance()->Update(OBJID::MOUSE);
	Key_Input();
}

void CEdit::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update(OBJID::UI);
	CTileMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update(OBJID::MOUSE);
}

void CEdit::Render(HDC hDC)
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"InGameMem");
	HDC hBackDC = CBmpMgr::Get_Instance()->Find_Img(L"InGame");
	GdiTransparentBlt(hMemDC, 0, 0, WINCX, WINCY, hBackDC, -(fScrollX/50.f), -(fScrollY/10.f), WINCX, WINCY, RGB(255, 255, 255));
	CTileMgr::Get_Instance()->Render(hMemDC);
	CObjMgr::Get_Instance()->Render(hMemDC, OBJID::UI);
	ShowTileIndex(hMemDC);
	ShowNowType(hMemDC);
	ShowBrush(hMemDC);
	CObjMgr::Get_Instance()->Render(hMemDC, OBJID::MOUSE);
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CEdit::Release()
{
	
}

void CEdit::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Down('Q'))
	{
		m_NowType = TILETYPE(int(m_NowType) + 1);

		if (TILETYPE::BEDROCK == m_NowType)
			m_NowType = TILETYPE::EMPTY;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('E'))
	{
		++m_iBrush;

		if (10 < m_iBrush)
			m_iBrush = 1;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RBUTTON))
	{
		POINT	pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		float fStartX = pt.x - (float(m_iBrush - 1) * float(TILECX));
		float fStartY = pt.y - (float(m_iBrush - 1) * float(TILECY));
		float fEndX = pt.x + (float(m_iBrush - 1) * float(TILECX));
		float fEndY = pt.y + (float(m_iBrush - 1) * float(TILECY));

		for (int i = 0; i < 1 + (m_iBrush-1)*2;++i)
			for (int j = 0; j < 1+(m_iBrush-1)*2;++j)
			{
				float fX = fStartX + (i * TILECX);
				float fY = fStartY + (j * TILECY);

				POINT pTemp = { fX ,fY };

				CTileMgr::Get_Instance()->MakeTile(m_NowType, pTemp);
			}
		
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
		CScrollMgr::Get_Instance()->Set_ScrollX(16);

	if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
		CScrollMgr::Get_Instance()->Set_ScrollY(16);

	if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
		CScrollMgr::Get_Instance()->Set_ScrollY(-16);

	if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
		CScrollMgr::Get_Instance()->Set_ScrollX(-16);
}

void CEdit::ShowTileIndex(HDC hDC)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	float iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	int iRow = int(pt.y - iScrollY) / int(TILECY);
	int iCol = int(pt.x - iScrollX) / int(TILECX);

	SetBkMode(hDC, OPAQUE);
	SetTextAlign(hDC, TA_TOP);

	TCHAR	szCnt[32] = L"";
	swprintf_s(szCnt, L"<Row [%d] : Col [%d]>", iRow, iCol);
	TextOut(hDC, 0, 0, szCnt, lstrlen(szCnt));
}

void CEdit::ShowNowType(HDC hDC)
{
	TCHAR	szType[32] = L"";

	SetBkMode(hDC, OPAQUE);
	SetTextAlign(hDC, TA_TOP);

	switch (m_NowType)
	{
	case TILETYPE::EMPTY:
		swprintf_s(szType, L"EMPTY(Erase)");
		break;
	case TILETYPE::DIRT:
		swprintf_s(szType, L"DIRT");
		break;
	case TILETYPE::ROCK:
		swprintf_s(szType, L"ROCK");
		break;
	case TILETYPE::COPPER:
		swprintf_s(szType, L"COPPER");
		break;
	case TILETYPE::IORN:
		swprintf_s(szType, L"IORN");
		break;
	case TILETYPE::SILVER:
		swprintf_s(szType, L"SILVER");
		break;
	case TILETYPE::SAND:
		swprintf_s(szType, L"SAND");
		break;
	case TILETYPE::GOLD:
		swprintf_s(szType, L"GOLD");
		break;
	case TILETYPE::WOODEN:
		swprintf_s(szType, L"WOODEN");
		break;
	case TILETYPE::TREE:
		swprintf_s(szType, L"TREE");
		break;
	case TILETYPE::COAL:
		swprintf_s(szType, L"COAL");
		break;
	case TILETYPE::END:
		swprintf_s(szType, L"END");
		break;
	default:
		break;
	}

	
	TextOut(hDC, 0, 50, szType, lstrlen(szType));
}

void CEdit::ShowBrush(HDC hDC)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	int iStartX = (int(pt.x) / TILECX) * TILECX;
	int iStartY = (int(pt.y) / TILECY) * TILECY;
	int iEndX = (int(pt.x) / TILECX) * TILECX + TILECX;
	int iEndY = (int(pt.y) / TILECY) * TILECY + TILECY;

	iStartX -= iStartX % TILECX;
	iStartY -= iStartY % TILECY;
	iEndX -= iEndX % TILECX;
	iEndY -= iEndY % TILECY;

	iStartX -= (m_iBrush - 1) * TILECX;
	iStartY -= (m_iBrush - 1) * TILECY;
	iEndX += (m_iBrush - 1) * TILECX;
	iEndY += (m_iBrush - 1) * TILECY;

	HPEN hNewPen = CreatePen(PS_SOLID, 1, RGB(255, 153, 0));
	HPEN hOldPen = (HPEN)SelectObject(hDC, hNewPen);

	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);

	Rectangle(hDC, iStartX, iStartY, iEndX, iEndY);

	SelectObject(hDC, hOldPen);
	DeleteObject(hNewPen);

	SelectObject(hDC, oldBrush);
	DeleteObject(myBrush);
}
