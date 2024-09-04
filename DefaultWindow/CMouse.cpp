#include "stdafx.h"
#include "CMouse.h"
#include "CBmpMgr.h"

void CMouse::Initialize()
{
	m_tInfo.fCX = 18.f;
	m_tInfo.fCY = 18.f;
}

void CMouse::Update()
{

}

OBJRESULT CMouse::Late_Update()
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	m_tInfo.fX = pt.x;
	m_tInfo.fY = pt.y;

	__super::Update_Rect();

	return OBJRESULT();
}

void CMouse::Render(HDC hDC)
{
	HDC hMouse = CBmpMgr::Get_Instance()->Find_Img(L"Mouse");

	GdiTransparentBlt(hDC,
		m_tInfo.fX,
		m_tInfo.fY,
		18,
		18,
		hMouse,
		0,
		0,
		18,
		18,
		RGB(0, 0, 0));
}

void CMouse::Release()
{
}

void CMouse::Collision_Rect_Obj(CObj* opponent)
{
}

void CMouse::Collision_Eillpse_Obj(CObj* opponent)
{
}
