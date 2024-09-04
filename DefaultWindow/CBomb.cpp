#include "stdafx.h"
#include "CBomb.h"
#include "CAbstractFactory.h"
#include "CScrollMgr.h"
#include "CCollisionMgr.h"
#include "CTileMgr.h"
#include "CExplosion.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CFire.h"
#include "CSoundMgr.h"

CBomb::CBomb() : m_iExplosionCount(0), m_bFire(false), m_bBouncy(false), m_bMine(false), m_iMineCount(MINEWAIT), m_bFIreSpray(false), m_bHighExp(false), m_bMineActive(false), m_bJump(false)
{
}

CBomb::~CBomb()
{
}

void CBomb::Initialize()
{
    m_tInfo = { WINCX / 2.f, WINCY / 2.f, 22.f, 22.f };
    m_fSpeed = 5.f;

    if (m_bBouncy)
        m_fSpeed = 20.f;

    m_vecSpec[int(OBJSPEC::ATTACK)] = 0;				m_vecSpecLimit[int(OBJSPEC::ATTACK)] = 0;
    m_vecSpec[int(OBJSPEC::HEALTH)] = 50;				m_vecSpecLimit[int(OBJSPEC::HEALTH)] = 50;

    m_bPlayer = false;
    m_ObjId = OBJID::MISSILE;
}

void CBomb::Update()
{
    if (!CTileMgr::Get_Instance()->IsGround(this))
        m_vVelocity.fY += m_fGravity * 0.05f;
    else
    {
        if (!m_bBouncy)
        {
            m_vVelocity.fY = 0;
            m_vVelocity.fX = 0;
        }
    }

    m_tInfo.fX += m_vVelocity.fX * m_fSpeed;
    m_tInfo.fY += m_vVelocity.fY * m_fSpeed;

    if (!m_bMine)
        --m_iExplosionCount;
}

OBJRESULT CBomb::Late_Update()
{
    INFO tTemp = { 0,0,0,0 };
    tTemp = CCollisionMgr::Get_Instance()->Collision_MapBorder(this);
    tTemp = CTileMgr::Get_Instance()->Coillsion_Tile(this);

    if (m_bMine)
    {
        m_tInfo = tTemp;

        if (m_iMineCount != MINEWAIT)
            --m_iMineCount;

        if (0 >= m_iMineCount)
        {
            m_iMineCount = MINEWAIT;
        }
            
        if ((m_bHighExp) && (m_bJump))
            if (0 <= m_vVelocity.fY)
            {
                m_bJump = false;
                CObjMgr::Get_Instance()->Add_Object(OBJID::EXPLOSION, CAbstractFactory<CExplosion>().Create(m_tInfo.fX, m_tInfo.fY));
                CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Size(100, 100);
                CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Player(m_bPlayer);
                dynamic_cast<CExplosion*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION))->Set_FireBomb();
                dynamic_cast<CExplosion*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION))->Set_DestroyTile(false);

                m_vecSpec[int(OBJSPEC::HEALTH)] -= 7;
            }

        Mine_Explosion();

        if (0 >= m_vecSpec[int(OBJSPEC::HEALTH)])
            m_bDead = true;

        if (m_bDead)
            return OBJRESULT::DEAD;

        __super::Update_Rect();

        return OBJRESULT::NONE;
    }

    if ((tTemp.fX != m_tInfo.fX) || (tTemp.fY != m_tInfo.fY))
        if (m_bChainExp)
        {
            CObjMgr::Get_Instance()->Add_Object(OBJID::EXPLOSION, CAbstractFactory<CExplosion>().Create(m_tInfo.fX, m_tInfo.fY));
            CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Size(100, 100);
            CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Player(m_bPlayer);
            if (m_bFire)
                dynamic_cast<CExplosion*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION))->Set_FireBomb();
        }

    if (tTemp.fX != m_tInfo.fX)
    {
        if (!m_bBouncy)
            m_vVelocity.fX = 0;
        else 
            m_vVelocity.fX *= -0.5f;
    }

    if ((tTemp.fY != m_tInfo.fY) && (m_bBouncy))
    {
        m_vVelocity.fY *= -0.5f;
    }

    m_tInfo = tTemp;

    if (0 >= m_iExplosionCount)
    {
        CObjMgr::Get_Instance()->Add_Object(OBJID::EXPLOSION, CAbstractFactory<CExplosion>().Create(m_tInfo.fX, m_tInfo.fY));
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Size(100, 100);
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Player(m_bPlayer);
        if (m_bFire)
            dynamic_cast<CExplosion*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION))->Set_FireBomb();

        Dead();
    }

    

    if (m_bDead)
        return OBJRESULT::DEAD;

    __super::Update_Rect();

    return OBJRESULT::NONE;
}

void CBomb::Render(HDC hDC)
{
    float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC hBomb = nullptr;

    if (m_bBouncy)
    {
        hBomb = CBmpMgr::Get_Instance()->Find_Img(L"Bomb");

        GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top - 8 + ScrollY, m_tInfo.fCX, m_tInfo.fCY + 8,
            hBomb,
            0, 0, 22, 30,
            RGB(255, 255, 255));
    }
    else if (m_bMine)
    {
        if (m_bFIreSpray)
        {
            hBomb = CBmpMgr::Get_Instance()->Find_Img(L"Mine1");
            GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY + 5, m_tInfo.fCX, m_tInfo.fCY,
                hBomb,
                0, 0, 16, 8,
                RGB(255, 255, 255));
        }
        else if (m_bHighExp)
        {
            hBomb = CBmpMgr::Get_Instance()->Find_Img(L"Mine2");
            GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY + 5, m_tInfo.fCX, m_tInfo.fCY,
                hBomb,
                0, 8, 16, 8,
                RGB(255, 255, 255));
        }
    }
    else
    {
        hBomb = CBmpMgr::Get_Instance()->Find_Img(L"Bomb");

        GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top - 8 + ScrollY, m_tInfo.fCX, m_tInfo.fCY + 8,
            hBomb,
            0, 0, 22, 30,
            RGB(255, 255, 255));
    }


    

    //Ellipse(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, m_tRect.right + ScrollX, m_tRect.bottom + ScrollY);
}

void CBomb::Release()
{
}

void CBomb::Collision_Rect_Obj(CObj* opponent)
{
}

void CBomb::Collision_Eillpse_Obj(CObj* opponent)
{
}

void CBomb::Mine_Explosion()
{
    if (!m_bMine)
        return; 

    if (m_bFIreSpray)
    {
        if ((!m_bMineActive) && (m_iMineCount == MINEWAIT))
        {
            list<CObj*> Templist = CObjMgr::Get_Instance()->Get_List(OBJID::MONSTER);

            for (auto& iter : Templist)
            {
                if (dynamic_cast<CMonster*>(iter)->Get_Air())
                    continue;

                if (TILECX * 5 > abs(iter->Get_Info().fX - m_tInfo.fX))
                {
                    m_bMineActive = true;
                    break;
                }
            }
        }

        if (m_bMineActive)
        {
             for (int i = 0; i < 10; ++i)
             {
                 CObjMgr::Get_Instance()->Add_Object(OBJID::FIRE, CAbstractFactory<CFire>::Create(m_tInfo.fX, m_tInfo.fY));
                 CObj* pTemp = CObjMgr::Get_Instance()->Get_LastObj(OBJID::FIRE);
                 pTemp->Set_Player(m_bPlayer);
                 pTemp->Set_Speed(10);

                 float fX = (rand() % 100 * 0.005f);
                 if (0 == rand() % 2) { fX *= -1; }
                 float fY = (rand() % 100 * 0.005f);
                 fY *= -1;

                pTemp->Set_Vecter2D(fX, fY);
             }

             m_bMineActive = false;
             --m_iMineCount;

             CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_EXP_1);
             CSoundMgr::Get_Instance()->PlaySound(L"Mine_Fire.wav", CHANNELID::SOUND_EXP_1, 3.f);
             m_vecSpec[int(OBJSPEC::HEALTH)] -= 7;
        }
    }

    if (m_bHighExp)
    {
        if ((!m_bMineActive) && (m_iMineCount == MINEWAIT))
        {
            list<CObj*> Templist = CObjMgr::Get_Instance()->Get_List(OBJID::MONSTER);

            for (auto& iter : Templist)
            {
                if (dynamic_cast<CMonster*>(iter)->Get_Air())
                    continue;

                if (TILECX * 5 > abs(iter->Get_Info().fX - m_tInfo.fX))
                {
                    m_bMineActive = true;
                    break;
                }
            }
        }

        if (m_bMineActive)
        {
            m_vVelocity.fY -= 1.1f;
            m_tInfo.fY += m_vVelocity.fY;
            m_bJump = true;
            m_bMineActive = false;
            --m_iMineCount;
        }
    }
}
