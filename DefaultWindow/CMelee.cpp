#include "stdafx.h"
#include "CMelee.h"
#include "CScrollMgr.h"

void CMelee::Initialize()
{
	m_tInfo = { WINCX / 2.f, WINCY / 2.f, 48.f, 40.f };
	m_vecSpec[int(OBJSPEC::ATTACK)] = 10;				m_vecSpecLimit[int(OBJSPEC::ATTACK)] = 10;
	m_vecSpecial[int(OBJSPECIAL::PUSH)] = true;

	m_ObjId = OBJID::MELEE;
}

void CMelee::Update()
{

}

OBJRESULT CMelee::Late_Update()
{
	__super::Update_Rect();

	if (!m_bDead)
		return OBJRESULT::NONE;
	else
		return OBJRESULT::DEAD;
}

void CMelee::Render(HDC hDC)
{
	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
}

void CMelee::Release()
{
}

void CMelee::Collision_Rect_Obj(CObj* opponent)
{
	Dead();
}

void CMelee::Collision_Eillpse_Obj(CObj* opponent)
{
	Dead();
}
