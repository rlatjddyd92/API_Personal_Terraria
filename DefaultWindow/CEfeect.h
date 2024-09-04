#pragma once

#include "define.h"
#include "Obj.h"

class CEfeect : public CObj
{
public:
	CEfeect() : m_bDead(false), m_bGround(false) {}
	virtual ~CEfeect() {}

public:
	virtual void Initialize()		override;
	virtual void Update()	    override;
	virtual OBJRESULT Late_Update()		override;
	virtual void Render(HDC hDC)	override;
	virtual void Release()			override;
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;

	void		 Set_Dead() { m_bDead = true; }
	void		 Set_Ground() { m_bGround = true; }

private:
	bool		m_bDead; // <- ��� ������� 
	bool		m_bGround; // true : ���� + �ü� / false : ���� + ����
	int			m_iExposureCount; // <- 0 �Ǹ� ������ 
};

