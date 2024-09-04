#pragma once

#include "CScene.h"

class CEdit : public CScene
{
public:
	CEdit() : m_NowType(TILETYPE::EMPTY), m_iBrush(1){}
	virtual ~CEdit() { Release(); }

public:
	virtual void		Initialize();
	virtual void	    Update();
	virtual void		Late_Update();
	virtual void		Render(HDC hDC);
	virtual void		Release();
	void				Key_Input();
	void				ShowTileIndex(HDC hDC);
	void				ShowNowType(HDC hDC);
	void				ShowBrush(HDC hDC);

private:
	TILETYPE m_NowType;
	int m_iBrush;
};


