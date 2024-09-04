#pragma once
#include "Define.h"
#include "Obj.h"

class CAchievment : public CObj
{
public:
	CAchievment () {}
	virtual ~CAchievment() {}


public:
	virtual void Initialize()		override;
	virtual void Update()	    override;
	virtual OBJRESULT Late_Update()		override;
	virtual void Render(HDC hDC)	override;
	virtual void Release()			override;
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;
	void		 Check_Sucssess();
	void		 Check_Click();
	void		 Set_Achievment(ACHIEVMENT Input) { m_Achievement = Input; }

private:
	const TCHAR* m_tGoal; // <- ��ǥ ǥ�� 
	bool		 m_bSucssess;
	CObj*		 m_bRewardInven;
	
	ACHIEVMENT	 m_Achievement; // <- ��ǥ ����, �� ���뿡 ���� ���� ���� 
	BACKGROUND    m_NowBack;
};

