#pragma once

#include "CScene.h"

class CMainMenu : public CScene
{
public:
	CMainMenu() {}
	virtual ~CMainMenu() { Release(); }

public:
	virtual void		Initialize();
	virtual void	    Update();
	virtual void		Late_Update();
	virtual void		Render(HDC hDC);
	virtual void		Release();
	void				Key_Input();

private:
};

