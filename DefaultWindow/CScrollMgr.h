#pragma once
#include "Define.h"

class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	float		Get_ScrollX()  { return m_fScrollX; } 
	float		Get_ScrollY() { return m_fScrollY; } 

	void		Set_ScrollX(float _fX) 
	{ 
		m_fScrollX += _fX; 
	
		if (m_fScrollX > 0) { m_fScrollX = 0; }
		if (m_fScrollX < -(TILECX * MAPCOL) + WINCX) { m_fScrollX = -(TILECX * MAPCOL) + WINCX; }
	}
	void		Set_ScrollY(float _fY) 
	{ 
		m_fScrollY += _fY; 
	
		if (m_fScrollY > 0) { m_fScrollY = 0; }
		if (m_fScrollY < -(TILECY * MAPROW) + WINCY) { m_fScrollY = -(TILECY * MAPROW) + (WINCY / TILECY) * TILECY; }
	}
	void		Reset_Scroll() { m_fScrollX = 0; m_fScrollY = 0;}

public:
	static CScrollMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CScrollMgr;

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
	static CScrollMgr* m_pInstance;

	float				m_fScrollX;
	float				m_fScrollY;
};

