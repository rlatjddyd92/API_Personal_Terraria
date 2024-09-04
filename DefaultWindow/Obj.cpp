#include "stdafx.h"
#include "Obj.h"
#include "CTileMgr.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"

CObj::CObj() : 
	m_fSpeed(0.f), 
	m_vVelocity({ 0,0 }), 
	m_fGravity(0.8f), 
	m_fJumpPower(10.0f), 
	m_bPushed(false), 
	m_nowAction(ACTION::IDLE), 
	m_iActionNum(0), 
	m_iAttackNum(0), 
	m_bDead(false), 
	m_ObjId(OBJID::END),
	m_bPlayer(false),
	m_iSoundCount(0)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));

	m_vecSpec.resize(int(OBJSPEC::END));
	m_vecSpecLimit.resize(int(OBJSPEC::END));
	m_vecSpecial.resize(int(OBJSPECIAL::END));
}

CObj::~CObj()
{
}

void CObj::Update_Rect()
{
	m_tRect.left	= LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top		= LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right	= LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom	= LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

void CObj::Rotate_Image()
{
	float	fDiagonal = sqrt((m_tInfo.fCX / 2.f) * (m_tInfo.fCX / 2.f) + (m_tInfo.fCY / 2.f) * (m_tInfo.fCY / 2.f));

	Vector2D vTemp = m_vVelocity;

	vTemp = vTemp.Normalliztion(vTemp);

	float m_fAngle = acos(vTemp.fX);


	
	// аб ╩С╢э
	m_tPoint[0].x = LONG((m_tInfo.fCX / 2.f) + fDiagonal * cos((m_fAngle + 135.f)));
	m_tPoint[0].y = LONG((m_tInfo.fCY / 2.f) - fDiagonal * sin((m_fAngle + 135.f)));

	// ©Л ╩С╢э
	m_tPoint[1].x = LONG((m_tInfo.fCX / 2.f) + fDiagonal * cos((m_fAngle + 45.f)));
	m_tPoint[1].y = LONG((m_tInfo.fCY / 2.f) - fDiagonal * sin((m_fAngle + 45.f)));

	// аб го╢э
	m_tPoint[2].x = LONG((m_tInfo.fCX / 2.f) + fDiagonal * cos((m_fAngle + 225.f)));
	m_tPoint[2].y = LONG((m_tInfo.fCY / 2.f) - fDiagonal * sin((m_fAngle + 225.f)));
}
