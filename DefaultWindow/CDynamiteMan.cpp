#include "stdafx.h"
#include "CDynamiteMan.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CStats.h"
#include "CTileMgr.h"
#include "CCollisionMgr.h"
#include "CNumber.h"
#include "CMelee.h"
#include "CMissile.h"
#include "CExplosion.h"
#include "CTileMgr.h"
#include "CBomb.h"
#include "CSceneMgr.h"
#include "CInGame.h"
#include "CBmpMgr.h"
#include "CSoundMgr.h"

void CDynamiteMan::Initialize()
{
	m_tInfo = { WINCX / 2.f, WINCY / 2.f, 210.f, 96.f };
	m_fSpeed = 1.f;
	m_fJumpPower = 10.f;

	m_vecSpec[int(OBJSPEC::HEALTH)] = 1000;				m_vecSpecLimit[int(OBJSPEC::HEALTH)] = 1000;
	m_vecSpec[int(OBJSPEC::ATTACK)] = 10;				m_vecSpecLimit[int(OBJSPEC::ATTACK)] = 10;

	m_bPlayer = false;
	m_ObjId = OBJID::BOSS;

	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CStats>().Create_Stats(STATS::HEALTH_BOSS, this));
}

void CDynamiteMan::Update()
{
	++m_iActionNum;
	if (1000 == m_iActionNum)
		m_iActionNum = 0;

	m_tBeforeInfo.fX = m_tInfo.fX;
	m_tBeforeInfo.fY = m_tInfo.fY;

	if (0.5f > float(m_vecSpec[int(OBJSPEC::HEALTH)]) / float(m_vecSpecLimit[int(OBJSPEC::HEALTH)]))
		if (1 == m_iPhase)
		{
			Set_Phase(2);
			CSceneMgr::Get_Instance()->Change_InGame_BackGround(BACKGROUND::DYNAMITE);
			CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"※주의※ 보스가 더 강력한 공격을 사용합니다."));
		}
			

	if ((0 >= float(m_vecSpec[int(OBJSPEC::HEALTH)]))&& (3 != m_iPhase))
	{
 		Dead_Scene();
		Set_Last_Phase();
	}
		
	if (ENDPHASE == m_iPhase)
		return;

	if (0 != m_iPhase)
	{
		Move_to_Player();
	}

	m_tInfo.fX += m_vVelocity.fX;
	m_tInfo.fY += m_vVelocity.fY;
}

OBJRESULT CDynamiteMan::Late_Update()
{

	__super::Update_Rect();

	if (1000 > abs(CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX - m_tInfo.fX))
	{
		if (!m_bActive)
		{
			CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_BGM);
			CSoundMgr::Get_Instance()->PlayBGM(L"Boss_Mod.wav", 0.3f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"※주의※ 보스가 깨어났습니다."));
		}

		Is_Active_Boss(true);

		if (0 == m_iPhase) 
			Set_Phase(1);
	}
	else
	{
		if (m_bActive)
		{
			CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_BGM);
			CSoundMgr::Get_Instance()->PlayBGM(L"Peace_Mode.wav", 0.3f);
		}

		Is_Active_Boss(false), Set_Phase(0);
		
	}
		

	if (1 == m_iPhase)
	{
		Slow_Explosion_Missile();
		Fire_Bomb();
	}

	if (2 == m_iPhase)
	{
		float fDistance = abs(CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX - m_tInfo.fX);

		if (50 < fDistance)
		{
			Dirt_Prison();
		}
			

		Fast_Explosion_Missile();
		Bomb_Rain();
		Spray_Fast_Explosion_Missile();
	}
	
	if (m_bDead)
	{
		CSceneMgr::Get_Instance()->Change_InGame_BackGround(BACKGROUND::NORMAL);
		CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_BGM);
		CSoundMgr::Get_Instance()->PlayBGM(L"Peace_Mode.wav", 1.f);
		return OBJRESULT::DEAD;
	}

	return OBJRESULT();
}

void CDynamiteMan::Render(HDC hDC)
{

	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();


	HDC hMoon = CBmpMgr::Get_Instance()->Find_Img(L"Eilen_Boss");

	GdiTransparentBlt(hDC,
		m_tRect.left + ScrollX,
		m_tRect.top + ScrollY,
		m_tInfo.fCX,
		m_tInfo.fCY,
		hMoon,
		0,
		96* int((GetTickCount()/100) % 4),
		210,
		96,
		RGB(255, 255, 255));
}

void CDynamiteMan::Release()
{
}

void CDynamiteMan::Collision_Rect_Obj(CObj* opponent)
{
	if ((opponent->Get_Player()) && (OBJID::MELEE == opponent->Get_OBJID()))
	{
		Add_Spec(OBJSPEC::HEALTH, -(opponent->Get_Spec(OBJSPEC::ATTACK)));
		CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CNumber>().Create_Number(m_tInfo.fX, m_tInfo.fY - 10, -(opponent->Get_Spec(OBJSPEC::ATTACK))));
	}

	if ((opponent->Get_Player()) && (OBJID::MISSILE == opponent->Get_OBJID()))
	{
		Add_Spec(OBJSPEC::HEALTH, -(opponent->Get_Spec(OBJSPEC::ATTACK)));
		CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CNumber>().Create_Number(m_tInfo.fX, m_tInfo.fY - 10, -(opponent->Get_Spec(OBJSPEC::ATTACK))));
	}

	if (OBJID::FIRE == opponent->Get_OBJID())
	{
		if (0 != rand() % 100)
			return;

		Add_Spec(OBJSPEC::HEALTH, -(opponent->Get_Spec(OBJSPEC::ATTACK)));
		CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CNumber>().Create_Number(m_tInfo.fX, m_tInfo.fY - 10, -(opponent->Get_Spec(OBJSPEC::ATTACK))));
	}
}

void CDynamiteMan::Collision_Eillpse_Obj(CObj* opponent)
{
	if (OBJID::EXPLOSION == opponent->Get_OBJID())
		if (dynamic_cast<CExplosion*>(opponent)->Get_Nuke())
			if (opponent->Get_Info().fCX >= NUKE_CX - 6)
				m_bDead = true;

	if ((opponent->Get_Player()) && (OBJID::EXPLOSION == opponent->Get_OBJID()))
	{
		Add_Spec(OBJSPEC::HEALTH, -(opponent->Get_Spec(OBJSPEC::ATTACK)));
 	}
}

void CDynamiteMan::Jump()
{
	m_vVelocity.fY -= m_fJumpPower;

	if (0 == m_vVelocity.fX)
	{
		if (0==rand() % 2)
			m_vVelocity.fX = -m_fSpeed;
		else 
			m_vVelocity.fX = m_fSpeed;
	}
}

void CDynamiteMan::Move_to_Player()
{
	float fDistance = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX - m_tInfo.fX;

	float fPlayerY = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY;

	float fLimit = 0;

	if (1 == m_iPhase)
		fLimit = 100;
	else if (2 == m_iPhase)
		fLimit = 100;

	if (fLimit < abs(fDistance))
	{
		if (0 > fDistance)
			m_vVelocity.fX = -m_fSpeed;
		else
			m_vVelocity.fX = m_fSpeed;
	}
	else
	{
		if (CTileMgr::Get_Instance()->IsGround(this))
			m_vVelocity.fX = 0;
	}

	if (fPlayerY - 75 < m_tInfo.fY)
		m_vVelocity.fY = -m_fSpeed;
	else if (fPlayerY - 65 > m_tInfo.fY)
		m_vVelocity.fY = m_fSpeed;

}

void CDynamiteMan::Slow_Explosion_Missile()
{
	if (0 != m_iActionNum % 400)
		return;

	INFO tPlayer = CObjMgr::Get_Instance()->Get_Player()->Get_Info();

	Vector2D vTemp = { 0,0 };

	vTemp.fX = tPlayer.fX - m_tInfo.fX;
	vTemp.fY = tPlayer.fY - m_tInfo.fY;

	vTemp = vTemp.Normalliztion(vTemp);

	CObjMgr::Get_Instance()->Add_Object(OBJID::MISSILE, CAbstractFactory<CMissile>().Create(m_tInfo.fX, m_tInfo.fY));
	CObj* pTemp = CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE);
	dynamic_cast<CMissile*>(pTemp)->Set_Explosion(true, 100);
	pTemp->Set_Player(false);
	pTemp->Set_Speed(5.f);
	pTemp->Set_Vecter2D(vTemp.fX, vTemp.fY);
}

void CDynamiteMan::Dirt_Prison()
{
	if (0 != m_iActionNum % 500)
		return;

	INFO tPlayer = CObjMgr::Get_Instance()->Get_Player()->Get_Info();

	CObjMgr::Get_Instance()->Add_Object(OBJID::EXPLOSION, CAbstractFactory<CExplosion>().Create(tPlayer.fX, tPlayer.fY));
	CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Size(250,250);
	CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Player(m_bPlayer);
	dynamic_cast<CExplosion*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION))->Set_TileMake();

	CObjMgr::Get_Instance()->Add_Object(OBJID::EXPLOSION, CAbstractFactory<CExplosion>().Create(tPlayer.fX, tPlayer.fY));
	CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Size(180, 180);
	CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Player(m_bPlayer);
}

void CDynamiteMan::Fast_Explosion_Missile()
{
	if ((0 != m_iActionNum % 550) && (0 == m_iMachine_Gun))
		return;

	if (0 == m_iMachine_Gun)
		m_iMachine_Gun = 10;

	--m_iMachine_Gun;

	if (1 != m_iMachine_Gun % 2)
		return;

	INFO tPlayer = CObjMgr::Get_Instance()->Get_Player()->Get_Info();

	Vector2D vTemp = { 0,0 };

	vTemp.fX = tPlayer.fX - m_tInfo.fX;
	vTemp.fY = tPlayer.fY - m_tInfo.fY;

	vTemp = vTemp.Normalliztion(vTemp);

	CObjMgr::Get_Instance()->Add_Object(OBJID::MISSILE, CAbstractFactory<CMissile>().Create(m_tInfo.fX, m_tInfo.fY));
	CObj* pTemp = CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE);
	dynamic_cast<CMissile*>(pTemp)->Set_Explosion(true, 50);
	pTemp->Set_Player(false);
	pTemp->Set_Speed(20.f);
	pTemp->Set_Vecter2D(vTemp.fX, vTemp.fY);
}

void CDynamiteMan::Bomb_Rain()
{
	if (0 != m_iActionNum % 800)
		return;

	for (int i = -30; i < 30; ++i)
	{
		Vector2D vTemp = { float (i) /5.f,-20};

		vTemp = vTemp.Normalliztion(vTemp);

		CObjMgr::Get_Instance()->Add_Object(OBJID::MISSILE, CAbstractFactory<CMissile>().Create(m_tInfo.fX, m_tInfo.fY));
		CObj* pTemp = CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE);
		dynamic_cast<CMissile*>(pTemp)->Set_Arrow(true);
		dynamic_cast<CMissile*>(pTemp)->Set_Drop(true);
		dynamic_cast<CMissile*>(pTemp)->Set_Explosion(true, 10);
		pTemp->Set_Player(false);
		pTemp->Set_Speed(10.f);
		pTemp->Set_Size(10, 10);
		pTemp->Set_Vecter2D(vTemp.fX, vTemp.fY);
	}
}

void CDynamiteMan::Fire_Bomb()
{
	if (0 != m_iActionNum % 800)
		return;

	INFO tPlayer = CObjMgr::Get_Instance()->Get_Player()->Get_Info();

	Vector2D vTemp = { 0,0 };

	vTemp.fX = tPlayer.fX - m_tInfo.fX;
	vTemp.fY = tPlayer.fY - m_tInfo.fY;

	vTemp = vTemp.Normalliztion(vTemp);

	CObjMgr::Get_Instance()->Add_Object(OBJID::MISSILE, CAbstractFactory<CBomb>().Create(m_tInfo.fX, m_tInfo.fY));
	CObj* pTemp = CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE);
	dynamic_cast<CBomb*>(pTemp)->Set_Bouncy();
	dynamic_cast<CBomb*>(pTemp)->Set_Fire();
	dynamic_cast<CBomb*>(pTemp)->Set_Chain();
	dynamic_cast<CBomb*>(pTemp)->Set_ExplosionCount(200);
	pTemp->Set_Player(false);
	pTemp->Set_Vecter2D(vTemp.fX, vTemp.fY);
}

void CDynamiteMan::Spray_Fast_Explosion_Missile()
{
	if ((0 != m_iActionNum % 900) && (0 == m_iAttackNum))
		return;

	if (0 == m_iAttackNum)
		m_iAttackNum = 100;

	--m_iAttackNum;

	if (0 == m_iAttackNum % 5)
	{
		float fX = 1;
		float fY = 1;

		while(1)
		{
			if ((fX == 0) && (fY == 0))
				return;

			CObjMgr::Get_Instance()->Add_Object(OBJID::MISSILE, CAbstractFactory<CMissile>().Create(m_tInfo.fX, m_tInfo.fY));
			CObj* pTemp = CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE);
			dynamic_cast<CMissile*>(pTemp)->Set_Explosion(true, 20);
			pTemp->Set_Player(false);
			pTemp->Set_Speed(5.f);
			pTemp->Set_Vecter2D(fX, fY);

			if (1 == fY)
				fY = -1;
			else if (-1 == fY)
				fY = 0;
			else
			{
				fY = 1;

				if (1 == fX)
					fX = -1;
				else if (-1 == fX)
					fX = 0;
				else
					fX = 1;
			}
		}
	}
}

void CDynamiteMan::Dead_Scene()
{
	if (ENDPHASE == m_iPhase)
		return;

	CObjMgr::Get_Instance()->Add_Object(OBJID::EXPLOSION, CAbstractFactory<CExplosion>().Create(m_tInfo.fX, m_tInfo.fY));
	CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Size(10, 10);
	CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Player(m_bPlayer);
	dynamic_cast<CExplosion*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION))->Set_NuKe();

	CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"※주의※ 보스가 모든 것을 소멸시키려 합니다."));
}

void CDynamiteMan::Drop_Item()
{
}
