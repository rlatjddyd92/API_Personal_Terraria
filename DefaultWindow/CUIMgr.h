#pragma once

#include "define.h"

class CUIMgr
{
private:
	CUIMgr();
	~CUIMgr();

public:
	void Make_UIComponent(UITYPE type, float fX, float fY);
	void Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();



public:
	static CUIMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CUIMgr;

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
	static CUIMgr* m_pInstance;
};

