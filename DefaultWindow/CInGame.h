#pragma once

#include "define.h"
#include "CScene.h"

class CInGame : public CScene
{
public:
	CInGame() : m_back(BACKGROUND::NORMAL), m_iSurvivalCount(SURTIME), m_iSafeCount(SAFETIME), m_bWarning(true) {}
	virtual ~CInGame() { Release(); }

public:
	virtual void		Initialize();
	virtual void	    Update();
	virtual void		Late_Update();
	virtual void		Render(HDC hDC);
	virtual void		Release();
	BACKGROUND			Get_Back() { return m_back; }
	void				Change_BackGround(BACKGROUND Input)
	{ 
		if (BACKGROUND::SURVIVAL == Input)
		{
			if (BACKGROUND::SURVIVAL == m_back)
				m_back = BACKGROUND::NORMAL;
			else
				m_back = BACKGROUND::SURVIVAL;

			m_iSafeCount = SAFETIME;
			m_iSurvivalCount = SURTIME;
		}
		else
		{
			if (BACKGROUND::SURVIVAL == m_back)
				return;
			else 
				m_back = Input;
		}
			
	
	}

private:
	void				Render_Time(HDC hDC);
	BACKGROUND			m_back;
	int					m_iSurvivalCount;
	int					m_iSafeCount;
	bool				m_bWarning;
};

