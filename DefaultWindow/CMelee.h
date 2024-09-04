#pragma once

#include "define.h"
#include "Obj.h"

class CMelee : public CObj
{
public:
	CMelee () {}
	virtual ~CMelee() {}

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual OBJRESULT Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;

	void Set_AttackDirection(DIRECTION Input) { m_AttackDirection = Input; }
	DIRECTION Get_AttackDirection() { return m_AttackDirection; }

private:
	DIRECTION	m_AttackDirection; // <- 공격의 주체가 공격하는 방향 
};


