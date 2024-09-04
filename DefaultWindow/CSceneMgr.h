#pragma once

#include "CScene.h"

class CSceneMgr
{
public:
	CSceneMgr(): m_NowScene(SCENETYPE::MAINMENU){};
	~CSceneMgr() { Release(); };

public:
	void		Initialize();
	void		Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();
	void		Set_NowScene(SCENETYPE Input) { m_NowScene = Input; };
	void		Change_InGame_BackGround(BACKGROUND Input);
	BACKGROUND  Get_Ingame_BackGround();

public:
	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CSceneMgr* m_pInstance;
	vector<CScene*> m_vecScene;
	SCENETYPE m_NowScene;
};

