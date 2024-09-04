#include "stdafx.h"
#include "CNumber.h"
#include "CScrollMgr.h"

void CNumber::Initialize()
{
	m_vVelocity.fY = -3.f;
	m_vVelocity.fX = 1.f;

	m_fStartX = m_tInfo.fX;
	m_fStartY = m_tInfo.fY;

	m_tInfo.fCX = 20;
	m_tInfo.fCY = 20;
}

void CNumber::Update()
{
	m_tInfo.fY += m_vVelocity.fY;
	m_tInfo.fX += m_vVelocity.fX;

	if (5 < abs(m_fStartX - m_tInfo.fX))
		m_vVelocity.fX *= -1;

	if (100 < abs(m_fStartY - m_tInfo.fY))
		m_bDead = true;
}

OBJRESULT CNumber::Late_Update()
{
	__super::Update_Rect();

	if (m_bDead)
		return OBJRESULT::DEAD;

	return OBJRESULT();
}

void CNumber::Render(HDC hDC)
{
	TCHAR	szCnt[32] = L"";
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (0 < m_fNumber)
	{
		SetTextColor(hDC, RGB(0, 100, 0));
		swprintf_s(szCnt, L"%d", int(m_fNumber));
	}
	else if (0 > m_fNumber)
	{
		SetTextColor(hDC, RGB(255, 0, 0));
		swprintf_s(szCnt, L"%d", int(m_fNumber));
	}
	else
		return;
	
	TextOut(hDC, m_tRect.left + fScrollX, m_tRect.top + fScrollY, szCnt, lstrlen(szCnt));

	SetTextColor(hDC, RGB(0, 0, 0));
}

void CNumber::Release()
{
}

void CNumber::Collision_Rect_Obj(CObj* opponent)
{
}

void CNumber::Collision_Eillpse_Obj(CObj* opponent)
{
}
