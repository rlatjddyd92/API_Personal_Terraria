#pragma once
#include "Obj.h"
#include "define.h"

class CItem : public CObj
{
public:
	CItem() : m_bCanStack(false), m_iCount(1), m_hItemSkin(nullptr), m_Mode(ITEMMODE::INVEN), m_Type(ITEMTYPE::END), m_pItemKey(nullptr), m_iGround_Delay(0), m_bEquip(false)
	, m_bFIreSpray(false), m_bHighExp(false), m_FType(FUNITURETYPE::END){}
	virtual ~CItem() { Release(); }

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual OBJRESULT Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;

public:
	void		 Render_Inven(HDC hDC);
	CObj*		 Get_Item() { return this; }
	ITEMTYPE	 Get_ItemType() { return m_Type; }
	const TCHAR* Get_ItemKey() { return m_pItemKey; }
	void		 Set_CanStack(bool Input) { m_bCanStack = Input; }
	void		 Set_ItemKey(const TCHAR* Input) { m_pItemKey = Input; }
	void		 Set_ItemMode(ITEMMODE Input) { m_Mode = Input; if (Input == ITEMMODE::GROUND) { m_iGround_Delay = 50; }}
	void		 Set_ItemType(ITEMTYPE Input) { m_Type = Input;}
	int			 Get_Count() { return m_iCount; }
	void		 Add_Count(int Input) { m_iCount += Input; }
	void		 Show_Initial(HDC hDC); // <- 테스트용
	void		 Show_Count(HDC hDC);
	bool		 Get_CanStack() { return m_bCanStack; }
	void		 Set_Equip() { m_bEquip = true; }
	bool		 Get_Equip() { return m_bEquip; }

	void		 Set_FireSpray() { m_bFIreSpray = true; m_tInfo.fCX = 16.f; m_tInfo.fCY = 8.f; }
	bool		 Get_FireSpray() { return m_bFIreSpray; }

	void		 Set_High() { m_bHighExp = true; m_tInfo.fCX = 16.f; m_tInfo.fCY = 8.f;}
	bool		 Get_High() { return m_bHighExp; }

	void		 Set_FunitureType(FUNITURETYPE Input) { m_FType = Input; }
	FUNITURETYPE Get_FunitureType() { return m_FType; }

private:
	void		m_AdjustFuniture();
	bool		m_bCanStack;
	int			m_iCount;
	HDC			m_hItemSkin;
	ITEMMODE	m_Mode;
	ITEMTYPE    m_Type;
	const TCHAR* m_pItemKey;
	int			m_iGround_Delay;
	bool		m_bEquip;

	CObj*		m_pMessage; // 아이템 툴팁

	// Throw 전용
	bool		m_bFIreSpray; // 화염을 많이 많듬
	bool		m_bHighExp;  // 위로 튀어올라 폭발

	// 가구 전용 
	FUNITURETYPE m_FType;
};

