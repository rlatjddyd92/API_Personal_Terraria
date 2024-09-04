#pragma once
#include "define.h"
#include "Obj.h"
#include "CSceneMgr.h"

class CBoss : public CObj
{
public:
	CBoss () : m_bActive(false), m_iPhase(0) {}
	virtual ~CBoss() {}

public:
	virtual void Initialize()		PURE;
	virtual void Update()	    PURE;
	virtual OBJRESULT Late_Update()		PURE;
	virtual void Render(HDC hDC)	PURE;
	virtual void Release()			PURE;
	virtual void Collision_Rect_Obj(CObj* opponent)	PURE;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	PURE;

	bool		 Get_Active() { return m_bActive; }

protected:
	void		 Set_Phase(int Input)
	{
		if (m_iPhase == ENDPHASE)
			return;

		m_iPhase = Input;

		if (0 == m_iPhase)
			m_vecSpec[int(OBJSPEC::HEALTH)] = m_vecSpecLimit[int(OBJSPEC::HEALTH)];

		CSceneMgr::Get_Instance()->Change_InGame_BackGround(BACKGROUND::NORMAL);
	}
	void		 Next_Phase() { if (m_iPhase == ENDPHASE) { return; } ++m_iPhase; }
	void		 Set_Last_Phase() { m_iPhase = ENDPHASE; }
	void		 Is_Active_Boss(bool Input) { m_bActive = Input; }

	bool		 m_bActive;
	int			 m_iPhase;

	INFO		m_tBeforeInfo;
};

