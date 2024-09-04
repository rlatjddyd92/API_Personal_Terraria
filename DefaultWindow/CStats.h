#pragma once
#include "Define.h"
#include "Obj.h"

class CStats : public CObj
{
public:
	CStats() :  m_Type (STATS::END), m_pLinkedObj(nullptr) {}
	virtual ~CStats() { Release(); }

public:
	virtual void Initialize()		override;
	virtual void Update()	    override;
	virtual OBJRESULT Late_Update()		override;
	virtual void Render(HDC hDC)	override;
	virtual void Release() 	override { m_pLinkedObj = nullptr; }
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;

public:
	void		 Set_Stats_Type(STATS Input) { m_Type = Input; }
	void		 Set_Linked_Obj(CObj* Linked) { m_pLinkedObj = Linked; }

private:
	void		 Player_Hp_Bar(HDC hDC);
	void		 Mon_Hp_bar(HDC hDC);
	void		 Boss_Hp_bar(HDC hDC);
	void		 Fly_Gage(HDC hDC);
	STATS		 m_Type;
	CObj*		 m_pLinkedObj;
};

