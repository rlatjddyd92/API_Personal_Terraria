#include "stdafx.h"
#include "CMissile.h"
#include "CTileMgr.h"
#include "CCollisionMgr.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CExplosion.h"
#include "CBmpMgr.h"
#include "CFire.h"

CMissile::CMissile(): m_bArrow(false), m_bExplosion(false), m_bDrop(false), m_bFire(false), m_fExplosionSize(0), m_iFlyCount(0), m_bMagic(false), m_bEnergy(false)
{
}

CMissile::~CMissile()
{
}

void CMissile::Initialize()
{
    m_tInfo = { WINCX / 2.f, WINCY / 2.f, 15.f, 15.f };
    m_fSpeed = 10.f;

    m_vecSpec[int(OBJSPEC::ATTACK)] = 50;				m_vecSpecLimit[int(OBJSPEC::ATTACK)] = 50;

    m_bPlayer = false;
    m_ObjId = OBJID::MISSILE;
    m_bFire = false;
}

void CMissile::Update()
{
    ++m_iFlyCount;

    if (m_bArrow)
    {
        if (!CTileMgr::Get_Instance()->IsGround(this))
            m_vVelocity.fY += m_fGravity * 0.01f;
        else
            m_vVelocity.fY = 0;
    }

    m_tInfo.fX += m_vVelocity.fX * m_fSpeed;
    m_tInfo.fY += m_vVelocity.fY * m_fSpeed;

    INFO tTemp = { 0,0,0,0 };
    tTemp = CCollisionMgr::Get_Instance()->Collision_MapBorder(this);
    tTemp = CTileMgr::Get_Instance()->Coillsion_Tile(this);

    if ((tTemp.fX != m_tInfo.fX) || (tTemp.fY != m_tInfo.fY))
        Special_Dead();

    if (CTileMgr::Get_Instance()->IsGround(this))
        Special_Dead();

    if (1000 <= m_iFlyCount)
        Special_Dead();
}

OBJRESULT CMissile::Late_Update()
{
    __super::Update_Rect();

    if (m_bDead)
        return OBJRESULT::DEAD;

	return OBJRESULT::NONE;
}

void CMissile::Render(HDC hDC)
{
    float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC hMissile = nullptr;

    if (m_bMagic)
    {
        int iSelect = m_iFlyCount % 6;

        hMissile = CBmpMgr::Get_Instance()->Find_Img(L"Magic_Missile");

        GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, m_tInfo.fCX, m_tInfo.fCY,
            hMissile,
            0, 12 * iSelect, 12, 12,
            RGB(255, 255, 255));
    }
    else if (m_bEnergy)
    {
        int iSelect = m_iFlyCount % 3;

        hMissile = CBmpMgr::Get_Instance()->Find_Img(L"Energy_Missile");

        GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, m_tInfo.fCX, m_tInfo.fCY,
            hMissile,
            0, 38 * iSelect, 38, 38,
            RGB(255, 255, 255));
    }
    else
    {
        hMissile = CBmpMgr::Get_Instance()->Find_Img(L"Dounut_Bullet");

        GdiTransparentBlt(hDC, m_tRect.left + ScrollX, m_tRect.top + ScrollY, m_tInfo.fCX, m_tInfo.fCY,
            hMissile,
            0, 0, 16, 16,
            RGB(255, 255, 255));
    }
    
}

void CMissile::Release()
{
}

void CMissile::Collision_Rect_Obj(CObj* opponent)
{
    if (m_bDrop)
        return;

    if (opponent->Get_Player() != m_bPlayer)
    {
        Special_Dead();
    }
}

void CMissile::Collision_Eillpse_Obj(CObj* opponent)
{
    Special_Dead();
}

void CMissile::Special_Dead()
{
    m_bDead = true;

    if (m_bExplosion)
    {
        CObjMgr::Get_Instance()->Add_Object(OBJID::EXPLOSION, CAbstractFactory<CExplosion>().Create(m_tInfo.fX, m_tInfo.fY));
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Size(m_fExplosionSize, m_fExplosionSize);
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Player(m_bPlayer);
        if (!m_bFire)
            dynamic_cast<CExplosion*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION))->Set_FireBomb(false);
    }

    if (m_bFire)
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
    }
}
