#pragma once

#include "define.h"

class CScene
{
public:
	CScene() {};
	virtual ~CScene() {};

public:
	virtual void		Initialize() PURE;
	virtual void	    Update() PURE;
	virtual void		Late_Update() PURE;
	virtual void		Render(HDC hDC) PURE;
	virtual void		Release() PURE;
	
public:
	void				Destroy_Scene_Obj();

private:
	
};

