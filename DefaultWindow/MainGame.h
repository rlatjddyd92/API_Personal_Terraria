#pragma once

#include "Define.h"
#include "Player.h"

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void		Initialize();
	void		Update();
	void		Late_Update();
	void		Render();
	void		Release();

private:
	HDC			m_DC;

public:
	static ULONGLONG		prev_update_time_ms;
	static ULONGLONG		delta_time_ms;

	DWORD			m_dwTime;
	int				m_iFPS;
	TCHAR			m_szFPS[32];
};

