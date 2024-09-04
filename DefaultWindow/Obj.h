#pragma once

#include "Define.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();
	
public:
	INFO		Get_Info() { return m_tInfo; }
	RECT		Get_Rect() { return m_tRect; }
	void		Set_Pos(float _fX, float _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}
	void		Set_Size(float _fX, float _fY) { m_tInfo.fCX = _fX;  m_tInfo.fCY = _fY; }
	void		Set_Vecter2D(float _fX, float _fY) { m_vVelocity.fX = _fX;  m_vVelocity.fY = _fY; }
	Vector2D	Get_Vector2D() { return m_vVelocity; }
	float		Get_Speed() { return m_fSpeed; }
	void		Set_Speed(float Input) { m_fSpeed = Input; }
	bool		Get_Dead() { return m_bDead; }
	int			Get_Spec(OBJSPEC Input) { return m_vecSpec[int(Input)]; }
	int			Get_SpecLimit(OBJSPEC Input) { return m_vecSpecLimit[int(Input)]; }
	bool		Get_Special(OBJSPECIAL Input) { return m_vecSpecial[int(Input)]; }
	void		Set_Special(OBJSPECIAL Special, bool Input) { m_vecSpecial[int(Special)] = Input; }

	void		Set_Spec(OBJSPEC spec, int Input) { m_vecSpec[int(spec)] = Input; Adjust_Spec(spec); }
	void		Add_Spec(OBJSPEC spec, int Input) { m_vecSpec[int(spec)] += Input; Adjust_Spec(spec);	}
	void		Adjust_Spec(OBJSPEC spec) 
	{
		if (0 > m_vecSpec[int(spec)])
			m_vecSpec[int(spec)] = 0;
		else if (m_vecSpecLimit[int(spec)] < m_vecSpec[int(spec)])
			m_vecSpec[int(spec)] = m_vecSpecLimit[int(spec)];
	}

public:
	virtual void Initialize()		PURE;
	virtual void Update()	    PURE;
	virtual OBJRESULT Late_Update()		PURE;
	virtual void Render(HDC hDC)	PURE;
	virtual void Release()			PURE;
	virtual void Collision_Rect_Obj (CObj* opponent)	PURE;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	PURE;

	bool		Get_Player() { return m_bPlayer; }
	void		Set_Player(bool Input) { m_bPlayer = Input; }
	OBJID		Get_OBJID() { return m_ObjId; }

	void		Set_Image(HDC Input) { m_hImage = Input; }

protected:
	void		Update_Rect();
	void		Dead() { m_bDead = true; }
	void		Rotate_Image();


protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	Vector2D	m_vVelocity;

	float		m_fSpeed;
	float		m_fGravity;
	float		m_fJumpPower;

	vector<int> m_vecSpec;
	vector<int> m_vecSpecLimit;

	bool		m_bPushed;
	bool		m_bDead;
	bool		m_bPlayer;

protected:
	ACTION		m_nowAction;
	vector<bool>	m_vecSpecial;
	int			m_iActionNum; // <- 스프라이트 조절용
	int			m_iAttackNum; // <- 공격 간격 조절용

	OBJID		m_ObjId;

	HDC			m_hImage;

	POINT		m_tPoint[3];

	int			m_iSoundCount;
};

