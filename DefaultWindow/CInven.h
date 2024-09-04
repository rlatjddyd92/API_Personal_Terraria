#pragma once
#include "Obj.h"
#include "define.h"
#include "CItem.h"

class CInven : public CObj
{
public:
	CInven() : m_pContainItem(nullptr), m_iQuickNum(-1), m_iCount(0), m_bSelected(false), m_bEquipment(false), m_InvenType(INVENTYPE::NORMAL){}
	virtual ~CInven() { Release(); }


public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual OBJRESULT Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override {  }
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;

public:
	void		Set_QuickNum(int Input) { m_iQuickNum = Input; }
	void		Set_Selected(bool Input) { m_bSelected = Input; }
	bool		Get_Selected() { return m_bSelected; }
	void		Put_in_Item(CObj* Item)
	{
		m_pContainItem = Item;
		m_pContainItem->Set_Pos(m_tInfo.fX, m_tInfo.fY);
		dynamic_cast<CItem*>(Item)->Set_ItemMode(ITEMMODE::INVEN);

		m_pContainItem->Late_Update();
	}
	CObj* Put_Out_Item()
	{
		if (!m_pContainItem)
			return nullptr;

		dynamic_cast<CItem*>(m_pContainItem)->Set_ItemMode(ITEMMODE::PICKUP);
		CObj* pTemp = m_pContainItem;
		m_pContainItem = nullptr;
		return pTemp;
	}
	CObj* Get_Item_Obj() { return m_pContainItem; }
	void		Reset_Item_Obj() { m_pContainItem = nullptr; }
	void		Set_Equipment() {m_bEquipment = true;}

	void		Set_InvenType(INVENTYPE Input) { m_InvenType = Input; }

private:
	CObj*	m_pContainItem;
	int		m_iQuickNum;
	int		m_iCount;
	bool	m_bSelected;
	bool	m_bEquipment;
	INVENTYPE m_InvenType;
};

