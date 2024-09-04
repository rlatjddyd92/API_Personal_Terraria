#pragma once
#include "Obj.h"
#include "define.h"

class CPlayer :	public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual OBJRESULT Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;

	void		 Set_UsingItem(CObj* Input) { m_pUsingItem = Input; }
	void		 Get_EquipItemSpec();

private:
	void		Key_Input();
	void		Key_Num();
	void		Pick_item();
	void		Make_Tile();
	void		Offset();
	void		Offset_Special(INFO Input);

	void		Attack_Melee(POINT pt);
	void		Attack_Missile(POINT pt, bool bArrow);
	void		Throw_Bomb(POINT pt);
	void		Throw_Item(POINT pt);
	void		Throw_Fire();

	bool		m_bUsingInven;

	INFO		m_tBeforeInfo;

	HDC			m_hPlayer_IDEL;
	HDC			m_hPlayer_USING;
	HDC			m_hPlayer_Action;

	CObj*		m_pPickedItem; // <- 마우스로 집은 아이템 
	CObj*		m_pUsingItem; // <- 퀵슬롯에서 선택한 아이템

	int			m_iSelectQuick; // <- 선택한 퀵슬롯 번호 (0~9)

	int			m_iTest;

	bool		m_bFlyMode;

	bool		m_bAutoHeal;

	int			m_iHealCoolTime;
};

