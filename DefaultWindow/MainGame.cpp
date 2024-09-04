#include "stdafx.h"
#include "MainGame.h"
#include "CSceneMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CKeyMgr.h"
#include "CTileMgr.h"
#include "CScrollMgr.h"
#include "CCollisionMgr.h"
#include "CInvenMgr.h"
#include "CSoundMgr.h"
#include "CFunitureMgr.h"

CMainGame::CMainGame() : m_DC(nullptr), m_dwTime(GetTickCount()), m_iFPS(0)
{

}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_DC = GetDC(g_hWnd);
	CBmpMgr::Get_Instance()->Initialize();
	CObjMgr::Get_Instance()->Initialize();
	CSceneMgr::Get_Instance()->Initialize();
	CTileMgr::Get_Instance()->Initialize();
	CSoundMgr::Get_Instance()->Initialize();

	ShowCursor(false);
}

void CMainGame::Update()
{
	CSceneMgr::Get_Instance()->Update();	
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
}

void CMainGame::Render()
{
	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);

		// SetWindowText : 윈도우 타일에 출력하는 함수
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}

	CSceneMgr::Get_Instance()->Render(m_DC);
}

void CMainGame::Release()
{
	CSceneMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CTileMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CCollisionMgr::Destroy_Instance();
	CInvenMgr::Destroy_Instance();
	CSoundMgr::Destroy_Instance();
	CFunitureMgr::Destroy_Instance();
	ReleaseDC(g_hWnd, m_DC);
}
