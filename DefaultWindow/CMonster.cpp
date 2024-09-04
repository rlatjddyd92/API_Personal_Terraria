#include "stdafx.h"
#include "CMonster.h"
#include "CMelee.h"
#include "CTileMgr.h"
#include "CCollisionMgr.h"
#include "CScrollMgr.h"
#include "CStats.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CNumber.h"
#include "CExplosion.h"
#include "CBmpMgr.h"
#include "CMissile.h"
#include "CSoundMgr.h"

void CMonster::Initialize()
{
    m_tInfo = { WINCX / 2.f, WINCY / 2.f, 27.f, 50.f };
    m_fSpeed = 2.f;
    m_fJumpPower = 15.f;
    m_iActionNum = 0; 

    m_vecSpec[int(OBJSPEC::HEALTH)] = 300;				m_vecSpecLimit[int(OBJSPEC::HEALTH)] = 300;
    m_vecSpec[int(OBJSPEC::ATTACK)] = 10;				m_vecSpecLimit[int(OBJSPEC::ATTACK)] = 10;

    m_bPlayer = false;
    m_ObjId = OBJID::MONSTER;

    CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CStats>().Create_Stats(STATS::HEALTH_MON, this));

    // 리소스 적용 
    if ((!m_bAir) && (m_bRange) && (m_bExp)) // 지상 - 원거리 - 폭발 
        m_hImage = CBmpMgr::Get_Instance()->Find_Img(L"Ground_Range_Exp"); // 마법사
    else if ((!m_bAir) && (m_bRange) && (!m_bExp)) // 지상 - 원거리 - 일반 
        m_hImage = CBmpMgr::Get_Instance()->Find_Img(L"Ground_Range"); // 화살
    else if ((!m_bAir) && (!m_bRange) && (m_bExp)) // 지상 - 근거리 - 폭발 
        m_hImage = CBmpMgr::Get_Instance()->Find_Img(L"Ground_Exp"); // 폭발토끼
    else if ((!m_bAir) && (!m_bRange) && (!m_bExp)) // 지상 - 근거리 - 일반 
        m_hImage = CBmpMgr::Get_Instance()->Find_Img(L"Ground"); // 뼈다귀맨
    else if ((m_bAir) && (m_bRange) && (m_bExp)) // 공중 - 원거리 - 폭발 
        m_hImage = CBmpMgr::Get_Instance()->Find_Img(L"Air_Range_Exp"); // 화난 구름
    else if ((m_bAir) && (m_bRange) && (!m_bExp)) // 공중 - 원거리 - 일반 
        m_hImage = CBmpMgr::Get_Instance()->Find_Img(L"Air_Range"); // 붉은 도넛
    else if ((m_bAir) && (!m_bRange) && (m_bExp)) // 공중 - 근거리 - 폭발 
        m_hImage = CBmpMgr::Get_Instance()->Find_Img(L"AIr_Exp"); // 폭발 해골
    else if ((m_bAir) && (!m_bRange) && (!m_bExp)) // 공중 - 근거리 - 일반
        m_hImage = CBmpMgr::Get_Instance()->Find_Img(L"Air"); // 유령

    // 크기 조정 
    if ((!m_bAir) && (m_bRange) && (m_bExp)) // 지상 - 원거리 - 폭발 
    {
        m_tInfo.fCX = 26.f;
        m_tInfo.fCY = 48.f;
        m_tImage = { m_tInfo.fX, m_tInfo.fY,  33.f,50.f };
        m_fNext = 54.f;
    }
    else if ((!m_bAir) && (m_bRange) && (!m_bExp)) // 지상 - 원거리 - 일반 
    {
        m_tInfo.fCX = 48.f;
        m_tInfo.fCY = 50.f;
        m_tImage = { m_tInfo.fX, m_tInfo.fY,  48.f,50.f };
        m_fNext = 56.f;
    }
    else if ((!m_bAir) && (!m_bRange) && (m_bExp)) // 지상 - 근거리 - 폭발 
    {
        m_tInfo.fCX = 28.f;
        m_tInfo.fCY = 28.f;
        m_tImage = { m_tInfo.fX, m_tInfo.fY,  28.f,28.f };
        m_fNext = 28.f;
    }
    else if ((!m_bAir) && (!m_bRange) && (!m_bExp)) // 지상 - 근거리 - 일반 
    {
        m_tInfo.fCX = 26.f;
        m_tInfo.fCY = 46.f;
        m_tImage = { m_tInfo.fX, m_tInfo.fY,  86.f,46.f };
        m_fNext = 68.f;
    }
    else if ((m_bAir) && (m_bRange) && (m_bExp)) // 공중 - 원거리 - 폭발 
    {
        m_tInfo.fCX = 48.f;
        m_tInfo.fCY = 42.f;
        m_tImage = { m_tInfo.fX, m_tInfo.fY,  48.f,42.f };
        m_fNext = 42.f;
    }
    else if ((m_bAir) && (m_bRange) && (!m_bExp)) // 공중 - 원거리 - 일반 
    {
        m_tInfo.fCX = 34.f;
        m_tInfo.fCY = 34.f;
        m_tImage = { m_tInfo.fX, m_tInfo.fY,  34.f,34.f };
        m_fNext = 36.f;
    }
    else if ((m_bAir) && (!m_bRange) && (m_bExp)) // 공중 - 근거리 - 폭발 
    {
        m_tInfo.fCX = 28.f;
        m_tInfo.fCY = 34.f;
        m_tImage = { m_tInfo.fX, m_tInfo.fY,  28.f,34.f };
        m_fNext = 34.f;
    }
    else if ((m_bAir) && (!m_bRange) && (!m_bExp)) // 공중 - 근거리 - 일반
    {
        m_tInfo.fCX = 40.f;
        m_tInfo.fCY = 56.f;
        m_tImage = { m_tInfo.fX, m_tInfo.fY,  40.f,56.f };
        m_fNext = 56.f;
    }

    m_fAttackYAdjust = rand() % 50;
    m_fAttackXAdjust = rand() % 50;

    m_ChaId = CSoundMgr::Get_Instance()->Set_MonChannel();
}

void CMonster::Update()
{
    if (m_bAir)
    {
        m_fAttackY = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY - 150 - m_fAttackYAdjust;
    }

    if (m_bAttack)
    {
        ++m_iActionNum;

        if (200 < m_iActionNum)
        {
            m_iActionNum = 0;
            m_bAttack = false;
        }
    }
    ++m_ImageNum;

    if (-1 == m_tBeforeInfo.fX) // 등장 사운드 
    {
        if ((!m_bAir) && (!m_bRange) && (!m_bExp)) // 지상 - 근거리 - 일반 (좀비)
        {
            CSoundMgr::Get_Instance()->StopSound(m_ChaId);
            CSoundMgr::Get_Instance()->PlaySound(L"Zombie_Emerge.wav", m_ChaId, 5.f);
        }
        else if ((m_bAir) && (!m_bRange) && (m_bExp)) // 공중 - 근거리 - 폭발 (해골)
        {
            CSoundMgr::Get_Instance()->StopSound(m_ChaId);
            CSoundMgr::Get_Instance()->PlaySound(L"Skull_Emerge.wav", m_ChaId, 5.f);
        }
    }

    m_tBeforeInfo.fX = m_tInfo.fX;
    m_tBeforeInfo.fY = m_tInfo.fY;

    float fDistance = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX - m_tInfo.fX;

    if (2000 > fDistance) m_bActive = true;
    else m_bActive = false;

    if (!m_bAir)
    {
        if (!CTileMgr::Get_Instance()->IsGround(this))
            m_vVelocity.fY += m_fGravity;
        else
        {
            m_vVelocity.fY = 0;
            //m_bPushed = false;

            if (5 >= abs(m_vVelocity.fX))
                m_vVelocity.fX = 0;
            else if (0 < m_vVelocity.fX)
                m_vVelocity.fX -= m_fSpeed;
            else if (0 > m_vVelocity.fX)
                m_vVelocity.fX += m_fSpeed;
        }
    }
    else if (m_bPushed)
    {

        if (5 >= abs(m_vVelocity.fX))
        {
            m_vVelocity.fX = 0;
            m_bPushed = false;
        }  
        else if (0 < m_vVelocity.fX)
            m_vVelocity.fX -= m_fSpeed;
        else if (0 > m_vVelocity.fX)
            m_vVelocity.fX += m_fSpeed;
    }

    if (m_bActive)
        Move_to_Player();

    if (m_bPushed)
    {
        if (0 < m_vVelocity.fX)
            m_vVelocity.fX -= 0.1;
        else if (0 > m_vVelocity.fX)
            m_vVelocity.fX += 0.1;
    }

    if (!m_bAir)
    {
        if (CTileMgr::Get_Instance()->Front_Tlie(this))
        {
            if (CTileMgr::Get_Instance()->IsGround(this))
                m_vVelocity.fY -= m_fJumpPower;
        }
    }
}

OBJRESULT CMonster::Late_Update()
{
    m_tInfo.fX += m_vVelocity.fX;
    m_tInfo.fY += m_vVelocity.fY;
    m_tImage.fX = m_tInfo.fX;
    m_tImage.fY = m_tInfo.fY;

    INFO tTemp = { 0,0,0,0 };
    tTemp = CCollisionMgr::Get_Instance()->Collision_MapBorder(this);
    tTemp = CTileMgr::Get_Instance()->Coillsion_Tile(this);


    if ((tTemp.fX != m_tInfo.fX) || (tTemp.fY != m_tInfo.fY))
    {
        m_bPushed = false;
    }

    if (tTemp.fX < 0)
        Dead();

    m_tInfo = tTemp;

    if (0 > m_vVelocity.fX)
        m_bRight = false;
    else if (0 < m_vVelocity.fX)
        m_bRight = true;

    if (m_vecSpec[int(OBJSPEC::HEALTH)] == 0)
    {
        if (m_bExp)
            Attack_Exp();

        Dead();
    }

    
        

    if (m_bDead)
    {
        if (-1 == m_tBeforeInfo.fX) // 사망 사운드 
        {
            if ((!m_bAir) && (m_bRange) && (!m_bExp)) // 지상 - 원거리 - 일반 (궁수)
            {
                CSoundMgr::Get_Instance()->StopSound(m_ChaId);
                CSoundMgr::Get_Instance()->PlaySound(L"Archer_Die.wav", m_ChaId, 5.f);
            }
            else if ((!m_bAir) && (!m_bRange) && (!m_bExp)) // 지상 - 근거리 - 일반 (좀비)
            {
                CSoundMgr::Get_Instance()->StopSound(m_ChaId);
                CSoundMgr::Get_Instance()->PlaySound(L"Zombie_Die.wav", m_ChaId, 5.f);
            }
            else if ((m_bAir) && (m_bRange) && (!m_bExp)) // 공중 - 원거리 - 일반 (도넛)
            {
                CSoundMgr::Get_Instance()->StopSound(m_ChaId);
                CSoundMgr::Get_Instance()->PlaySound(L"Bomb_Small.wav", CHANNELID::SOUND_EXP_1, 4.f);
            }
            else if ((m_bAir) && (!m_bRange) && (!m_bExp)) // 공중 - 근거리 - 일반 (유령)
            {
                CSoundMgr::Get_Instance()->StopSound(m_ChaId);
                CSoundMgr::Get_Instance()->PlaySound(L"Ghost_Die.wav", m_ChaId, 5.f);
            }
        }

        return OBJRESULT::DEAD;
    }
        

    __super::Update_Rect();
    Make_Image_Rect();

    return OBJRESULT::NONE;
}

void CMonster::Render(HDC hDC)
{
    float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    int iSelectNum = 0;

    if ((!m_bAir) && (m_bRange) && (m_bExp)) // 지상 - 원거리 - 폭발 
    {
        if ((m_bAttack)&&(m_iActionNum <= 50))
            iSelectNum = 2;
        else
            iSelectNum = (m_ImageNum / 10) % 2;
    }
    else if ((!m_bAir) && (m_bRange) && (!m_bExp)) // 지상 - 원거리 - 일반 
    {
        if ((m_bAttack) && (m_iActionNum <= 50))
            iSelectNum = 0;
        else
            iSelectNum = 1 + (m_ImageNum / 10) % 5;
    }
    else if ((!m_bAir) && (!m_bRange) && (m_bExp)) // 지상 - 근거리 - 폭발 
    {
        iSelectNum = (m_ImageNum / 10) % 7;

        GdiTransparentBlt(hDC, m_tImageRect.left + ScrollX, m_tImageRect.top + ScrollY, m_tImage.fCX, m_tImage.fCY, m_hImage, m_bRight * m_tImage.fCX, iSelectNum * m_fNext, m_tImage.fCX, m_tImage.fCY, RGB(255,255,255));
        return;
    }
    else if ((!m_bAir) && (!m_bRange) && (!m_bExp)) // 지상 - 근거리 - 일반 
    {
        if ((m_bAttack) && (m_iActionNum <= 50))
            iSelectNum = 3 + (m_iActionNum / 10) % 4;
        else
            iSelectNum = (m_ImageNum / 10) % 3;
    }
    else if ((m_bAir) && (m_bRange) && (m_bExp)) // 공중 - 원거리 - 폭발 
    {
        iSelectNum = (m_ImageNum / 10) % 4;
        m_bRight = false;
    }
    else if ((m_bAir) && (m_bRange) && (!m_bExp)) // 공중 - 원거리 - 일반 
    {
        iSelectNum = (m_ImageNum / 10) % 4;
        m_bRight = false;
    }
    else if ((m_bAir) && (!m_bRange) && (m_bExp)) // 공중 - 근거리 - 폭발 
    {
        iSelectNum = (m_ImageNum / 10) % 3;
        m_bRight = false;
    }
    else if ((m_bAir) && (!m_bRange) && (!m_bExp)) // 공중 - 근거리 - 일반
    {
        iSelectNum = (m_ImageNum / 10) % 4;
    }

    GdiTransparentBlt(hDC, m_tImageRect.left + ScrollX, m_tImageRect.top + ScrollY, m_tImage.fCX, m_tImage.fCY, m_hImage, m_bRight * m_tImage.fCX, iSelectNum * m_fNext, m_tImage.fCX, m_tImage.fCY, RGB(255, 255, 255));
}

void CMonster::Release()
{
  
}

void CMonster::Collision_Rect_Obj(CObj* opponent)
{
    if ((opponent->Get_Player()) && (OBJID::MELEE == opponent->Get_OBJID()))
    {
        Add_Spec(OBJSPEC::HEALTH, -(opponent->Get_Spec(OBJSPEC::ATTACK)));
        CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CNumber>().Create_Number(m_tInfo.fX, m_tInfo.fY - 10, -(opponent->Get_Spec(OBJSPEC::ATTACK))));

        if (DIRECTION::EAST == dynamic_cast<CMelee*>(opponent)->Get_AttackDirection())
            m_vVelocity.fX += 10;
        else 
            m_vVelocity.fX -= 10;

        m_vVelocity.fY -= 5;

        m_bPushed = true;
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

void CMonster::Collision_Eillpse_Obj(CObj* opponent)
{
    if (OBJID::EXPLOSION == opponent->Get_OBJID())
        if (dynamic_cast<CExplosion*>(opponent)->Get_Nuke())
            if (opponent->Get_Info().fCX >= NUKE_CX - 6)
                m_bDead = true;

    if (OBJID::EXPLOSION == opponent->Get_OBJID())
    {
        Add_Spec(OBJSPEC::HEALTH, -(opponent->Get_Spec(OBJSPEC::ATTACK)));

        Vector2D vTemp = { 0,0 };

        m_tInfo.fY -= 20;

        vTemp.fX = m_tInfo.fX - opponent->Get_Info().fX;
        vTemp.fY = m_tInfo.fY - opponent->Get_Info().fY;

        vTemp = vTemp.Normalliztion(vTemp);

        m_vVelocity = vTemp * m_fSpeed * 5;

        m_bPushed = true;
    }
}

void CMonster::Move_to_Player()
{
    if (m_bPushed)
        return;

    float fDistance = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX - m_tInfo.fX;
    float fDistanceY = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY - m_tInfo.fY;

    float fPlayerJump = CObjMgr::Get_Instance()->Get_Player()->Get_Vector2D().fY;

    float fLimit = 0.f;

    if (m_bRange) 
        fLimit = 300.f;
    else if ((m_bExp) && (!m_bRange)) 
        fLimit = 20.f; 
    else 
        fLimit = 20.f;

    if ((m_bAir) && (m_bRange))
        fLimit += m_fAttackXAdjust;

    if (abs(fDistance) > fLimit)
    {
        if (fDistance > 0)
            m_vVelocity.fX = +m_fSpeed;
        else if (fDistance < 0)
            m_vVelocity.fX = -m_fSpeed;
        else
        {
            m_vVelocity.fX = 0;
        }
    }
    else
    {
        m_vVelocity.fX = 0;

        if (!m_bAttack)
        {
            if (-1 == m_tBeforeInfo.fX) // 공격 사운드 
            {
                if ((!m_bAir) && (m_bRange) && (m_bExp)) // 지상 - 원거리 - 폭발 (마법사)
                {
                    CSoundMgr::Get_Instance()->StopSound(m_ChaId);
                    CSoundMgr::Get_Instance()->PlaySound(L" ", m_ChaId, 3.f);
                }
                else if ((!m_bAir) && (m_bRange) && (!m_bExp)) // 지상 - 원거리 - 일반 (궁수)
                {
                    CSoundMgr::Get_Instance()->StopSound(m_ChaId);
                    CSoundMgr::Get_Instance()->PlaySound(L" ", m_ChaId, 3.f);
                }
                else if ((!m_bAir) && (!m_bRange) && (m_bExp)) // 지상 - 근거리 - 폭발 (자폭토끼)
                {
                    CSoundMgr::Get_Instance()->StopSound(m_ChaId);
                    CSoundMgr::Get_Instance()->PlaySound(L" ", m_ChaId, 3.f);
                }
                else if ((!m_bAir) && (!m_bRange) && (!m_bExp)) // 지상 - 근거리 - 일반 (좀비)
                {
                    CSoundMgr::Get_Instance()->StopSound(m_ChaId);
                    CSoundMgr::Get_Instance()->PlaySound(L"Zombie_Attack.wav", m_ChaId, 5.f);
                }
                else if ((m_bAir) && (m_bRange) && (m_bExp)) // 공중 - 원거리 - 폭발 (화난 구름)
                {
                    CSoundMgr::Get_Instance()->StopSound(m_ChaId);
                    CSoundMgr::Get_Instance()->PlaySound(L"Thunder_Attack.wav", m_ChaId, 5.f);
                }
                else if ((m_bAir) && (m_bRange) && (!m_bExp)) // 공중 - 원거리 - 일반 (도넛)
                {
                    CSoundMgr::Get_Instance()->StopSound(m_ChaId);
                    CSoundMgr::Get_Instance()->PlaySound(L" ", m_ChaId, 3.f);
                }
                else if ((m_bAir) && (!m_bRange) && (m_bExp)) // 공중 - 근거리 - 폭발 (해골)
                {
                    CSoundMgr::Get_Instance()->StopSound(m_ChaId);
                    CSoundMgr::Get_Instance()->PlaySound(L" ", m_ChaId, 3.f);
                }
                else if ((m_bAir) && (!m_bRange) && (!m_bExp)) // 공중 - 근거리 - 일반 (유령)
                {
                    CSoundMgr::Get_Instance()->StopSound(m_ChaId);
                    CSoundMgr::Get_Instance()->PlaySound(L" ", m_ChaId, 3.f);
                }
            }

            if((!m_bRange)&&(!m_bExp)) Attack_Melee();
            if(m_bRange) Attack_Gun();
        }
    }
        
    if ((!m_bRange) && (m_bExp))
        if (abs(fDistance) <= fLimit)
            if (abs(fDistanceY) <= 20)
                Attack_Exp();

    if ((m_bAir) && (m_bRange))
        if (0 == fPlayerJump)
        {
             if (m_fAttackY < m_tInfo.fY)
                   m_vVelocity.fY = -m_fSpeed;
                else 
                 m_vVelocity.fY = m_fSpeed;

              if (5 > abs(m_fAttackY - m_tInfo.fY))
                  m_vVelocity.fY = 0;
        }

    if ((!m_bRange) && (m_bAir)) // <- 공중 근접몬의 돌진 
    {
        if (abs(fDistance) < 100.f)
        {
            Vector2D vTemp = { 0,0 };

            vTemp.fX = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX - m_tInfo.fX;
            vTemp.fY = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY - m_tInfo.fY;

            vTemp = vTemp.Normalliztion(vTemp);

            m_vVelocity = vTemp * m_fSpeed;

            if (CTileMgr::Get_Instance()->IsGround(this))
            {
                m_vVelocity.fY = 0;
            }

            if (CTileMgr::Get_Instance()->Front_Tlie(this))
            {
                m_vVelocity.fY = -10;
            }
        }


    }
}

void CMonster::Attack_Melee()
{
    if (!m_bRight)
    {
        CObjMgr::Get_Instance()->Add_Object(OBJID::MELEE, CAbstractFactory<CMelee>::Create(m_tInfo.fX - m_tInfo.fCX * 0.5f, m_tInfo.fY));
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::MELEE)->Set_Player(m_bPlayer);
        dynamic_cast<CMelee*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::MELEE))->Set_AttackDirection(DIRECTION::WEST);
    }
    else 
    {
        CObjMgr::Get_Instance()->Add_Object(OBJID::MELEE, CAbstractFactory<CMelee>::Create(m_tInfo.fX + m_tInfo.fCX * 0.5f, m_tInfo.fY));
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::MELEE)->Set_Player(m_bPlayer);
        dynamic_cast<CMelee*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::MELEE))->Set_AttackDirection(DIRECTION::EAST);
    }

    m_bAttack = true;
}

void CMonster::Attack_Gun()
{
    if ((m_bAir) && (m_bExp)) // <- 공중 폭발 (화난 구름) 
    {
        Vector2D vTemp = { 0,0 };

        vTemp.fX = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX - m_tInfo.fX;
        vTemp.fY = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY - m_tInfo.fY;

        vTemp = vTemp.Normalliztion(vTemp);

        CObjMgr::Get_Instance()->Add_Object(OBJID::MISSILE, CAbstractFactory<CMissile>::Create(m_tInfo.fX, m_tInfo.fY));
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE)->Set_Player(m_bPlayer);
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE)->Set_Speed(3.f);
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE)->Set_Size(20, 20);
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE)->Set_Vecter2D(vTemp.fX, vTemp.fY);
        CObj* pTemp = CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE);
        dynamic_cast<CMissile*>(pTemp)->Set_Energy(true);
        dynamic_cast<CMissile*>(pTemp)->Set_Explosion(true, 70);
    }
    if ((m_bAir) && (!m_bExp)) // <- 공중 일반 (도넛) 
    {
        Vector2D vTemp = { 0,0 };

        vTemp.fX = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX - m_tInfo.fX;
        vTemp.fY = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY - m_tInfo.fY;

        vTemp = vTemp.Normalliztion(vTemp);

        CObjMgr::Get_Instance()->Add_Object(OBJID::MISSILE, CAbstractFactory<CMissile>::Create(m_tInfo.fX, m_tInfo.fY));
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE)->Set_Player(m_bPlayer);
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE)->Set_Vecter2D(vTemp.fX, vTemp.fY);
        CObj* pTemp = CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE);
    }
    if ((!m_bAir) && (m_bExp)) // <- 지상 폭발 (마법)
    {
        Vector2D vTemp = { 0,0 };

        vTemp.fX = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX - m_tInfo.fX;
        vTemp.fY = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY - m_tInfo.fY;

        vTemp = vTemp.Normalliztion(vTemp);

        CObjMgr::Get_Instance()->Add_Object(OBJID::MISSILE, CAbstractFactory<CMissile>::Create(m_tInfo.fX, m_tInfo.fY));
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE)->Set_Player(m_bPlayer);
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE)->Set_Speed(3.f);
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE)->Set_Size(16, 16);
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE)->Set_Vecter2D(vTemp.fX, vTemp.fY);
        CObj* pTemp = CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE);
        dynamic_cast<CMissile*>(pTemp)->Set_Explosion(true, 70);
        dynamic_cast<CMissile*>(pTemp)->Set_Magic(true);
    }
    if ((!m_bAir) && (!m_bExp)) // <- 지상 일반 (화살)
    {
        float fDistance = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX - m_tInfo.fX;

        Vector2D vTemp = { 0,0 };

        vTemp.fX = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX - m_tInfo.fX;
        vTemp.fY = abs(vTemp.fX) * -1;

        vTemp = vTemp.Normalliztion(vTemp);

        CObjMgr::Get_Instance()->Add_Object(OBJID::MISSILE, CAbstractFactory<CMissile>::Create(m_tInfo.fX, m_tInfo.fY));
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE)->Set_Player(m_bPlayer);
        CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE)->Set_Vecter2D(vTemp.fX, vTemp.fY);
        CObj* pTemp = CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE);

        dynamic_cast<CMissile*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE))->Set_Arrow(true);
        dynamic_cast<CMissile*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE))->Set_Speed(2.1f);
        dynamic_cast<CMissile*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE))->Set_Fire(true);
    }

    m_bAttack = true;
}

void CMonster::Attack_Exp()
{
    CObjMgr::Get_Instance()->Add_Object(OBJID::EXPLOSION, CAbstractFactory<CExplosion>().Create(m_tInfo.fX, m_tInfo.fY));
    CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Size(70, 70);
    CObjMgr::Get_Instance()->Get_LastObj(OBJID::EXPLOSION)->Set_Player(m_bPlayer);

    Dead();
}

void CMonster::Make_Image_Rect()
{
    m_tImageRect.left = LONG(m_tImage.fX - (m_tImage.fCX * 0.5f));
    m_tImageRect.top = LONG(m_tImage.fY - (m_tImage.fCY * 0.5f));
    m_tImageRect.right = LONG(m_tImage.fX + (m_tImage.fCX * 0.5f));
    m_tImageRect.bottom = LONG(m_tImage.fY + (m_tImage.fCY * 0.5f));
}
