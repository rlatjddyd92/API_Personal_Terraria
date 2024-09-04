#include "stdafx.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CMainMenu.h"
#include "CEdit.h"
#include "CInGame.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;

void CSceneMgr::Initialize()
{
	m_vecScene.push_back(new CMainMenu);
	m_vecScene.push_back(new CEdit);
	m_vecScene.push_back(new CInGame);

	m_vecScene[int(m_NowScene)]->Initialize();
}

void CSceneMgr::Update()
{
	m_vecScene[int(m_NowScene)]->Update();
}

void CSceneMgr::Late_Update()
{
	SCENETYPE sTemp = m_NowScene;

	m_vecScene[int(m_NowScene)]->Late_Update();

	if (SCENETYPE::EXIT == m_NowScene)
		DestroyWindow(g_hWnd);

	if (sTemp != m_NowScene)
	{
		m_vecScene[int(sTemp)]->Destroy_Scene_Obj();
		m_vecScene[int(m_NowScene)]->Initialize();
	}
		
}

void CSceneMgr::Render(HDC hDC)
{
	m_vecScene[int(m_NowScene)]->Render(hDC);
}

void CSceneMgr::Release()
{
	for (vector<CScene*>::iterator iter = m_vecScene.begin(); iter != m_vecScene.end();)
	{
		Safe_Delete(*iter);
		iter = m_vecScene.erase(iter);
	}

	m_vecScene.clear();
}

void CSceneMgr::Change_InGame_BackGround(BACKGROUND Input)
{
	dynamic_cast<CInGame*>(m_vecScene[int(SCENETYPE::INGAME)])->Change_BackGround(Input);
}

BACKGROUND CSceneMgr::Get_Ingame_BackGround()
{
	return dynamic_cast<CInGame*>(m_vecScene[int(SCENETYPE::INGAME)])->Get_Back();
}
