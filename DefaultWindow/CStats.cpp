#include "stdafx.h"
#include "CStats.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CBoss.h"
#include "Player.h"

void CStats::Initialize()
{
	m_ObjId = OBJID::UI;

	if (m_Type == STATS::HEALTH_PLAYER)
		m_tInfo = { 400, 500, 256, 38 };

	if (m_Type == STATS::FLYGAGE)
		m_tInfo = { 400, 530, 257, 30 };

	if (m_Type == STATS::HEALTH_BOSS)
		m_tInfo = { 150, 200, 257, 30 };

}

void CStats::Update()
{
	
}

OBJRESULT CStats::Late_Update()
{
	if (m_pLinkedObj->Get_Dead())
		return OBJRESULT::DEAD;

	if (m_Type == STATS::HEALTH_MON)
		m_tInfo = { m_pLinkedObj->Get_Info().fX, m_pLinkedObj->Get_Info().fY - m_pLinkedObj->Get_Info().fCY, 36, 12 };

	__super::Update_Rect();
	return OBJRESULT();
}

void CStats::Render(HDC hDC)
{
	if (m_Type == STATS::HEALTH_PLAYER)
		Player_Hp_Bar(hDC);

	if (m_Type == STATS::HEALTH_MON)
		Mon_Hp_bar(hDC);

	if ((m_Type == STATS::HEALTH_BOSS) && (dynamic_cast<CBoss*>(m_pLinkedObj)->Get_Active()))
		Boss_Hp_bar(hDC);

	if ((m_Type == STATS::FLYGAGE)&&(m_pLinkedObj->Get_Special(OBJSPECIAL::SUPERJUMP)))
		Fly_Gage(hDC);
}

void CStats::Collision_Rect_Obj(CObj* opponent)
{
}

void CStats::Collision_Eillpse_Obj(CObj* opponent)
{
}

void CStats::Player_Hp_Bar(HDC hDC)
{
	int iHp = m_pLinkedObj->Get_Spec(OBJSPEC::HEALTH);

	// 哭率 场
	HDC hLeft = CBmpMgr::Get_Instance()->Find_Img(L"Heart_Left");
	HDC hLeft_Fill = CBmpMgr::Get_Instance()->Find_Img(L"Heart_Fill");
	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top+4, 28, 30, hLeft, 0, 0, 28, 30, RGB(255, 255, 255));

	float fStartY = 0.f;
	if (iHp >= 100 * 1) { fStartY = 0.f; }
	else { fStartY = 22.f - 22.f * (iHp * 0.01f); }
	GdiTransparentBlt(hDC, m_tRect.left + 4, m_tRect.top + 4 +4 + fStartY, 22, 22 - fStartY, hLeft_Fill, 0, fStartY, 22, 22 - fStartY, RGB(255, 255, 255));

	// 吝居 * 8俺
	for (int i = 0; i < 8; ++i)
	{
		HDC hMiddle = CBmpMgr::Get_Instance()->Find_Img(L"Heart_Middle");

		GdiTransparentBlt(hDC, m_tRect.left + 28 + (24 * i), m_tRect.top + 4, 24, 30, hMiddle, 0, 0, 24, 30, RGB(255, 255, 255));

		float fStartY = 0.f;
		if (iHp >= 100 * (i+2)) { fStartY = 0.f; }
		else { fStartY = 22.f - 22.f * ((iHp - (100 * (i + 1))) * 0.01f); }
		GdiTransparentBlt(hDC, m_tRect.left + 28 + (24 * i), m_tRect.top + 4 + 4 + fStartY, 22, 22 - fStartY, hLeft_Fill, 0, fStartY, 22, 22 - fStartY, RGB(255, 255, 255));
	}

	// 坷弗率 场
	HDC hRight = CBmpMgr::Get_Instance()->Find_Img(L"Heart_Right_Fancy");
	GdiTransparentBlt(hDC, m_tRect.left + 20 + (24 * 8), m_tRect.top, 36, 38, hRight, 0, 0, 36, 38, RGB(255, 255, 255));

	fStartY = 0.f;
	if (iHp >= 1000) { fStartY = 0.f; }
	else { fStartY = 22.f - 22.f * ((iHp - (100 * (9))) * 0.01f); }
	GdiTransparentBlt(hDC, m_tRect.left + 28 + (24 * 8), m_tRect.top + 4 + 4 + fStartY, 22, 22- fStartY, hLeft_Fill, 0, fStartY, 22, 22 - fStartY, RGB(255, 255, 255));
}

void CStats::Mon_Hp_bar(HDC hDC)
{
	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	int iHp = m_pLinkedObj->Get_Spec(OBJSPEC::HEALTH);
	float fAdjustX = 29.f - (float (iHp) / float(m_pLinkedObj->Get_SpecLimit(OBJSPEC::HEALTH))) * 29.f;

	HDC hHPbar = CBmpMgr::Get_Instance()->Find_Img(L"HealthBar1");
	GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, 36, 12, hHPbar, 0, 0, 36, 12, RGB(255, 255, 255));

	HPEN MyPen = CreatePen(PS_NULL, 5, RGB(0, 0, 255));
	HPEN OldPen = (HPEN)SelectObject(hDC, MyPen);

	HBRUSH MyBrush = CreateSolidBrush(RGB(50, 205, 50));
	HBRUSH OldBrush = (HBRUSH)SelectObject(hDC, MyBrush);

	Rectangle(hDC, m_tRect.left + 4 + ScrollX, m_tRect.top + 4 + ScrollY, m_tRect.right - 3 + ScrollX - fAdjustX, m_tRect.bottom - 3 + ScrollY);

	SelectObject(hDC, OldPen);
	DeleteObject(MyPen);

	SelectObject(hDC, OldBrush);
	DeleteObject(MyBrush);
}

void CStats::Boss_Hp_bar(HDC hDC)
{
	int iHp = m_pLinkedObj->Get_Spec(OBJSPEC::HEALTH);
	int iHpLimit = m_pLinkedObj->Get_SpecLimit(OBJSPEC::HEALTH);
	float fFillLength = 230.f * (float(iHp) / float(iHpLimit));

	HDC hHpBar = CBmpMgr::Get_Instance()->Find_Img(L"UI_BossBar");
	HDC hHpBack = CBmpMgr::Get_Instance()->Find_Img(L"UI_BossBar_Back");
	HDC hHpFill = CBmpMgr::Get_Instance()->Find_Img(L"UI_BossBar_Fill");

	GdiTransparentBlt(hDC, m_tRect.left + 16, m_tRect.top + 10, 230, 11, hHpBack, 0, 0, 456, 22, RGB(255, 255, 255));
	GdiTransparentBlt(hDC, m_tRect.left + 16, m_tRect.top + 10, fFillLength, 11, hHpFill, 0, 0, 2, 11, RGB(255, 255, 255));
	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, 257, 30, hHpBar, 0, 0, 515, 60, RGB(255, 255, 255));
}

void CStats::Fly_Gage(HDC hDC)
{
	int iHp = m_pLinkedObj->Get_Spec(OBJSPEC::FUEL);
	int iHpLimit = m_pLinkedObj->Get_SpecLimit(OBJSPEC::FUEL);
	float fFillLength = 230.f * (float(iHp) / float(iHpLimit));

	HDC hHpBar = CBmpMgr::Get_Instance()->Find_Img(L"UI_BossBar");
	HDC hHpBack = CBmpMgr::Get_Instance()->Find_Img(L"UI_BossBar_Back");
	HDC hHpFill = CBmpMgr::Get_Instance()->Find_Img(L"UI_Fuel_Fill");

	GdiTransparentBlt(hDC, m_tRect.left + 16, m_tRect.top + 10, 230, 11, hHpBack, 0, 0, 456, 22, RGB(255, 255, 255));
	GdiTransparentBlt(hDC, m_tRect.left + 16, m_tRect.top + 10, fFillLength, 11, hHpFill, 0, 0, 2, 11, RGB(255, 255, 255));
	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, 257, 30, hHpBar, 0, 0, 515, 60, RGB(255, 255, 255));
}
