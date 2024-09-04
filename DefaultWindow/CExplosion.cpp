#include "stdafx.h"
#include "CExplosion.h"
#include "CScrollMgr.h"
#include "CTileMgr.h"
#include "CBmpMgr.h"
#include "CFire.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CSoundMgr.h"

void CExplosion::Initialize()
{
    m_tInfo = { WINCX / 2.f, WINCY / 2.f, 20.f, 20.f };
    m_fSpeed = 10.f;

    m_vecSpec[int(OBJSPEC::ATTACK)] = 50;				m_vecSpecLimit[int(OBJSPEC::ATTACK)] = 10;

    m_bPlayer = false;
    m_ObjId = OBJID::EXPLOSION;
    m_bTileMake = false;
    m_FireBomb = false;

    m_ExpSoundNum = CSoundMgr::Get_Instance()->Set_ExpChannel();
}

void CExplosion::Update()
{
    if (m_bNuke)
    {
        m_tInfo.fCX += 3;
        m_tInfo.fCY += 3;

        if (m_tInfo.fCX >= NUKE_CX)
        {
            if (!m_bTileMake)
                CTileMgr::Get_Instance()->Explosion_Tile(this, false);
            else 
                CTileMgr::Get_Instance()->Explosion_Tile(this, true);
            m_bDead = true;
        }

        return;
    }

    if (!m_bTileMake)
        CTileMgr::Get_Instance()->Explosion_Tile(this, false);
    else if (m_bDestroyTile)
        CTileMgr::Get_Instance()->Explosion_Tile(this, true);

    if (!m_bTileMake)
    {
        int iMulty = 0;

        if (m_FireBomb)
            iMulty = 2;

        for (int i = 0; i < rand() % 10 * iMulty; ++i)
        {
            float fX = (rand() % 100 * 0.01f);
            if (0 == rand() % 2) { fX *= -1; }
            float fY = (rand() % 100 * 0.01f);
            if (0 == rand() % 2) { fY *= -1; }

            CObjMgr::Get_Instance()->Add_Object(OBJID::FIRE, CAbstractFactory<CFire>().Create(m_tInfo.fX, m_tInfo.fY));
            CObjMgr::Get_Instance()->Get_LastObj(OBJID::FIRE)->Set_Vecter2D(fX, fY);
            CObjMgr::Get_Instance()->Get_LastObj(OBJID::FIRE)->Set_Player(m_bPlayer);
            CObjMgr::Get_Instance()->Get_LastObj(OBJID::FIRE)->Set_Speed(10);
        }
    }
        

}

OBJRESULT CExplosion::Late_Update()
{
    __super::Update_Rect();

    if (m_bDead)
        return OBJRESULT::DEAD;

    CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_EXP_1);

    int iTemp = rand() % 3;

    if (iTemp == 0)
        CSoundMgr::Get_Instance()->PlaySound(L"Bomb_Big.wav", m_ExpSoundNum, 3.f);
    else if (iTemp == 1)
        CSoundMgr::Get_Instance()->PlaySound(L"Bomb_Middle.wav", m_ExpSoundNum, 3.f);
    else 
        CSoundMgr::Get_Instance()->PlaySound(L"Bomb_Small.wav", m_ExpSoundNum, 3.f);

    if (m_bNuke)
    {
        if (m_tInfo.fCX == NUKE_CX)
            m_bDead = true;
        else 
            return OBJRESULT::NONE;
    }

    if (4 == m_ImageNum)
        m_bDead = true;

    return OBJRESULT::NONE;
}

void CExplosion::Render(HDC hDC)
{
    float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    if (m_bNuke)
    {
        HDC hNuke = CBmpMgr::Get_Instance()->Find_Img(L"Nuke");
        GdiTransparentBlt(hDC,
            m_tRect.left + ScrollX,
            m_tRect.top + ScrollY,
            m_tInfo.fCX,
            m_tInfo.fCY,
            hNuke,
            0,
            0,
            408,
            408,
            RGB(255, 255, 255));
    }
    else
    {
        HDC hExp = CBmpMgr::Get_Instance()->Find_Img(L"Exp2");
        GdiTransparentBlt(hDC,
            m_tRect.left + ScrollX - m_tInfo.fCX*0.5,
            m_tRect.top + ScrollY - m_tInfo.fCY*0.5,
            m_tInfo.fCX * 2,
            m_tInfo.fCY * 2,
            hExp,
            0,
            m_ImageNum * 52,
            52,
            52,
            RGB(255, 255, 255));
        
    }

    ++m_ImageNum;


    m_vecSpec[int(OBJSPEC::ATTACK)] = 0;
}

void CExplosion::Release()
{
}

void CExplosion::Collision_Rect_Obj(CObj* opponent)
{
    
}

void CExplosion::Collision_Eillpse_Obj(CObj* opponent)
{
    
}
