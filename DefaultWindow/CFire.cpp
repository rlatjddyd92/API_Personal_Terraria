#include "stdafx.h"
#include "CFire.h"
#include "CScrollMgr.h"
#include "CTileMgr.h"
#include "CCollisionMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"

void CFire::Initialize()
{
	m_tInfo = { 0.f, 0.f, 1,1 };
	m_tFireInfo = { 0.f, 0.f, 10.f, 10.f };

	m_fSpeed = 0.5f + float(rand()%500) / 100.f;

	m_fJumpPower = 10.f;

	m_vecSpec[int(OBJSPEC::HEALTH)] = 1000;				m_vecSpecLimit[int(OBJSPEC::HEALTH)] = 1000;
	m_vecSpec[int(OBJSPEC::ATTACK)] = 10;				m_vecSpecLimit[int(OBJSPEC::ATTACK)] = 10;

	m_ObjId = OBJID::FIRE;
}

void CFire::Update()
{
	m_vecSpec[int(OBJSPEC::HEALTH)] -= 5;
	++m_iImage;

	if (15 <= m_iImage)
		m_iImage = 0;

	if (m_bFixed)
	{
		m_tInfo.fX = m_pLink->Get_Info().fX + m_fAdjustX;
		m_tInfo.fY = m_pLink->Get_Info().fY + m_fAdjustY;

		return;
	}

	if (m_bBurnTree)
	{
		m_bBurnTree = true;

		if (!CTileMgr::Get_Instance()->IsGround(this))
			Dead();

		//m_vecSpec[int(OBJSPEC::HEALTH)] += 1.8;

		return;
	}

	if (!CTileMgr::Get_Instance()->IsGround(this))
		m_vVelocity.fY += m_fGravity * 0.01f;
	else if (0 < m_vVelocity.fY)
	{

		m_vVelocity.fX = 0;
		m_vVelocity.fY = 0;

	}

	m_tInfo.fX += m_vVelocity.fX * m_fSpeed;
	m_tInfo.fY += m_vVelocity.fY * m_fSpeed;

	if (0 < m_vVelocity.fY)
		{
			m_tInfo = CTileMgr::Get_Instance()->Coillsion_Tile(this);
		}
	
	if ((0 > m_tInfo.fX - m_tInfo.fCX * 0.5f) || (MAPCOL * TILECX < m_tInfo.fX + m_tInfo.fCX * 0.5f))
	{
		m_tInfo.fX = 10;
		m_tInfo.fY = 10;
		Dead();
	}

	if ((0 > m_tInfo.fY - m_tInfo.fCY * 0.5f) || (MAPROW * TILECY < m_tInfo.fY + m_tInfo.fCY * 0.5f))
	{
		m_tInfo.fX = 10;
		m_tInfo.fY = 10;
		Dead();
	}
}

OBJRESULT CFire::Late_Update()
{
	__super::Update_Rect();

	m_tFireInfo.fX = m_tInfo.fX - m_tFireInfo.fCX * 0.5f;
	m_tFireInfo.fY = m_tInfo.fY;

	if (0 >= m_vecSpec[int(OBJSPEC::HEALTH)])
	{
		if (m_bBurnTree)
		{
			int iRow = m_tInfo.fY / TILECY;
			int iCol = m_tInfo.fX / TILECX;

			//CTileMgr::Get_Instance()->Burn_Tree(iRow, iCol-1);
			CTileMgr::Get_Instance()->Burn_Tree(iRow, iCol);
			//CTileMgr::Get_Instance()->Burn_Tree(iRow, iCol+1);
			m_bDead = true;
		}
		else
			m_bDead = true;
	}
		

	if (true == m_bDead)
		return OBJRESULT::DEAD;

	if (m_bFixed)
	{
		if (m_pLink->Get_Dead())
			return OBJRESULT::DEAD;
	}

	return OBJRESULT();
}

void CFire::Render(HDC hDC)
{
	HDC hFire = CBmpMgr::Get_Instance()->Find_Img(L"Flame_Small");

	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	
	GdiTransparentBlt(hDC,
		m_tFireInfo.fX - m_tFireInfo.fCX*0.5f + ScrollX,
		m_tFireInfo.fY - m_tFireInfo.fCY*0.5f + ScrollY,
		m_tFireInfo.fCX,
		m_tFireInfo.fCY,
		hFire,
		(m_iImage / 5) * (10),
		0,
		10,
		10,
		RGB(255, 255, 255));
}

void CFire::Release()
{
	m_pLink = nullptr;
}

void CFire::Collision_Rect_Obj(CObj* opponent)
{
	if (m_bFixed) return;

	if (m_bPlayer == opponent->Get_Player())
		return;

	m_bFixed = true;
	m_pLink = opponent;

	m_fAdjustX = m_tInfo.fX - opponent->Get_Info().fX;
	m_fAdjustY = m_tInfo.fY - opponent->Get_Info().fY;

	if (0 > m_fAdjustX) { m_fAdjustX += rand() % int(opponent->Get_Info().fCX * 0.5f); }
	if (0 < m_fAdjustX) { m_fAdjustX -= rand() % int(opponent->Get_Info().fCX * 0.5f); }
	if (0 > m_fAdjustY) { m_fAdjustY += rand() % int(opponent->Get_Info().fCY * 0.5f); }
	if (0 < m_fAdjustY) { m_fAdjustY -= rand() % int(opponent->Get_Info().fCY * 0.5f); }

}

void CFire::Collision_Eillpse_Obj(CObj* opponent)
{
}

void CFire::Burn_Tree()
{
	m_bBurnTree = true;

	/*
	if (0 == rand()%30)
	{
		for (int i = 0; i < rand() % 2; ++i)
		{
			float fX = (rand() % 100 * 0.005f);
			if (0 == rand() % 2) { fX *= -1; }
			float fY = (rand() % 100 * 0.005f);
			fY *= -1;

			CObjMgr::Get_Instance()->Add_Object(OBJID::FIRE, CAbstractFactory<CFire>().Create(m_tInfo.fX, m_tInfo.fY));
			CObjMgr::Get_Instance()->Get_LastObj(OBJID::FIRE)->Set_Vecter2D(fX, fY);
			CObjMgr::Get_Instance()->Get_LastObj(OBJID::FIRE)->Set_Player(m_bPlayer);
		}
	}
	*/
}
