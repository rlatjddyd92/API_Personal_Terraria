#include "stdafx.h"
#include "CInGame.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "Player.h"
#include "CAbstractFactory.h"
#include "CKeyMgr.h"
#include "CTileMgr.h"
#include "CBmpMgr.h"
#include "CImageButton.h"
#include "CMonster.h"
#include "CDynamiteMan.h"
#include "CSoundMgr.h"
#include "CFunitureMgr.h"

void CInGame::Initialize()
{
    CTileMgr::Get_Instance()->Load_Map();
    //CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CImageButton>().Create(700, 25));
    //dynamic_cast<CImageButton*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::UI))->Set_Function(BUTTONFUNCTION::MAINMENU);

    if (CObjMgr::Get_Instance()->Get_List(OBJID::PLAYER).empty())
        CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, CAbstractFactory<CPlayer>().Create(200, 200));

    //CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, CAbstractFactory<CMonster>().Create(50 * TILECX, 52 * TILECY));
    CObjMgr::Get_Instance()->Add_Object(OBJID::BOSS, CAbstractFactory<CDynamiteMan>().Create(296 * TILECX, 22* TILECY));
    CScrollMgr::Get_Instance()->Reset_Scroll();

    
}

void CInGame::Update()
{
    if (m_back == BACKGROUND::SURVIVAL)
    {
        --m_iSurvivalCount;

        if (0 == rand() % 100)
        {
            float fX = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX;

            bool bLeft = rand() % 2; // 화면 왼쪽 바깥 혹은 오른쪽 바깥
            bool bFly = rand() % 2; // 공중
            bool bRange = rand() % 2; // 원거리 
            bool bExp = rand() % 2; // 폭발 몬스터 (카운트 20% 남았을 때) 

            //if (m_iSurvivalCount > SURTIME * 0.2f)
                //bExp = false;



            if (bLeft)
                CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, CAbstractFactory<CMonster>::Create_Monster(fX - WINCX * 0.6f, 200, bFly, bRange, bExp));
            else
                CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, CAbstractFactory<CMonster>::Create_Monster(fX + WINCX * 1.1f, 200, bFly, bRange, bExp));

        }

        if (0 >= m_iSurvivalCount)
        {
            CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_BGM);
            CSoundMgr::Get_Instance()->PlayBGM(L"Peace_Mode.wav", 0.3f);

            m_back = BACKGROUND::NORMAL;
            m_iSurvivalCount = SURTIME;
        }
    }
    else if (m_back == BACKGROUND::NORMAL)
    {
        --m_iSafeCount;

        if (0 == rand() % 10000)
        {
            /*
            float fX = CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX;

            bool bLeft = rand() % 2; // 화면 왼쪽 바깥 혹은 오른쪽 바깥
            bool bFly = rand() % 2; // 공중
            bool bRange = rand() % 2; // 원거리 
            bool bExp = false;

            if (bLeft)
                CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, CAbstractFactory<CMonster>::Create_Monster(fX - WINCX * 0.8f, 200, bFly, bRange, bExp));
            else
                CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, CAbstractFactory<CMonster>::Create_Monster(fX + WINCX * 1.1f, 200, bFly, bRange, bExp));
                */
        }

        if (0 >= m_iSafeCount)
        {
            CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"◈생존의 밤◈ - ★화★염★파★티★를 즐기세요!!"));
            dynamic_cast<CMessage*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::MESSAGE))->Set_OpenCount(500);
            CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_BGM);
            CSoundMgr::Get_Instance()->PlayBGM(L"Servival_Mod.wav", 0.3f);
            m_back = BACKGROUND::SURVIVAL;
            m_iSafeCount = SAFETIME;
            m_bWarning = true;
        }
    }
    else if (m_back == BACKGROUND::DYNAMITE)
    {
        m_iSurvivalCount = SURTIME;
        m_iSafeCount = SAFETIME;
        m_bWarning = true;
    }

    CObjMgr::Get_Instance()->Update();

    if ((m_iSafeCount <= SAFETIME * 0.2f) && (m_bWarning))
    {
        m_bWarning = false;
        CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"생존의 밤이 곧 시작됩니다."));
    }
        
    CObjMgr::Get_Instance()->Update(OBJID::MOUSE);
}

void CInGame::Late_Update()
{
    CTileMgr::Get_Instance()->Late_Update();
    CObjMgr::Get_Instance()->Late_Update();
    CFunitureMgr::Get_Instance()->Late_Update();
    CObjMgr::Get_Instance()->Late_Update(OBJID::MOUSE);
}

void CInGame::Render(HDC hDC)
{
    float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"InGameMem");
    HDC hBackDC = nullptr;

    switch (m_back)
    {
    case BACKGROUND::NORMAL:
        hBackDC = CBmpMgr::Get_Instance()->Find_Img(L"InGame");
        break;
    case BACKGROUND::SURVIVAL:
        hBackDC = CBmpMgr::Get_Instance()->Find_Img(L"Survival");
        break;
    case BACKGROUND::DYNAMITE:
        hBackDC = CBmpMgr::Get_Instance()->Find_Img(L"Dynamaite_2Phase");
        break;
    case BACKGROUND::ABYSS:
        break;
    case BACKGROUND::RAVA:
        break;
    case BACKGROUND::END:
        break;
    default:
        break;
    }

    GdiTransparentBlt(hMemDC, 0, 0, WINCX, WINCY, hBackDC, -(fScrollX / 50.f), -(fScrollY / 10.f), WINCX, WINCY, RGB(255, 255, 255));
    CTileMgr::Get_Instance()->Render(hMemDC);
    CFunitureMgr::Get_Instance()->Render(hMemDC);
    Render_Time(hMemDC);
    CObjMgr::Get_Instance()->Render(hMemDC);
    CObjMgr::Get_Instance()->Render(hMemDC, OBJID::MOUSE);
    BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CInGame::Release()
{
}

void CInGame::Render_Time(HDC hDC)
{
    HDC hTime = CBmpMgr::Get_Instance()->Find_Img(L"Time");
    HDC hDynamite = CBmpMgr::Get_Instance()->Find_Img(L"Dynamaite_2Phase");
    HDC hSafe = CBmpMgr::Get_Instance()->Find_Img(L"InGame");
    HDC hSur = CBmpMgr::Get_Instance()->Find_Img(L"Survival");

    

    if (m_back == BACKGROUND::DYNAMITE)
    {
        GdiTransparentBlt(hDC, 406, 24, 52, 50, hTime, 0, 0, 520, 500, RGB(255, 255, 255));
    }
    if (m_back == BACKGROUND::NORMAL)
    {
        float fSize = 50 * float(SAFETIME - m_iSafeCount) / float(SAFETIME);

        GdiTransparentBlt(hDC, 406, 24, 52, 50, hSur, 0, 0, 520, 500, RGB(255, 255, 255));
        GdiTransparentBlt(hDC, 406, 24 + fSize, 52, 50 - fSize, hSafe, 0, fSize * 10, 520, 500 - fSize * 10, RGB(255, 255, 255));
    }
    if (m_back == BACKGROUND::SURVIVAL)
    {
        float fSize = 50 * float(SURTIME - m_iSurvivalCount) / float(SURTIME);

        GdiTransparentBlt(hDC, 406, 24, 52, 50, hSafe, 0, 0, 520, 500, RGB(255, 255, 255));
        GdiTransparentBlt(hDC, 406, 24 + fSize, 52, 50 - fSize, hSur, 0, fSize * 10, 520, 500 - fSize * 10, RGB(255, 255, 255));
    }

    GdiTransparentBlt(hDC, 400, 10, 64, 70, hTime, 0,0, 64, 70, RGB(255, 255, 255));
}
