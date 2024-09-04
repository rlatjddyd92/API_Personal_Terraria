#include "stdafx.h"
#include "CInvenMgr.h"
#include "CObjMgr.h"
#include "CInven.h"
#include "CAbstractFactory.h"
#include "Player.h"

CInvenMgr* CInvenMgr::m_pInstance = nullptr;

CInvenMgr::CInvenMgr() : m_bOpen(false), m_iSelectdQuickSlot(0)
{
}

CInvenMgr::~CInvenMgr()
{
	Release();
}

CObj* CInvenMgr::Pick_Up_Item_by_Mouse(POINT pt, bool bUseInven)
{
	for (int i = 0; i < INVENQ; ++i)
	{
		if (PtInRect(&m_vecQuickSlot[i]->Get_Rect(), pt))
		{
			return Put_Out_Quick(i);
		}
	}

	if (!bUseInven)
		return nullptr;

	for (int i = 0; i < INVENM; ++i)
	{
		if (PtInRect(&m_vecMainInven[i]->Get_Rect(), pt))
		{
			return Put_Out_Inven(i);
		}
	}

	for (int i = 0; i < INVENQ; ++i)
	{
		if (PtInRect(&m_vecEquip[i]->Get_Rect(), pt))
		{
			return Put_Out_Equip(i);
		}
	}

	return nullptr;
}

CObj* CInvenMgr::Put_In_Item_By_Mouse(POINT pt, CObj* item, bool bUseInven)
{
	if (Stack_Item(item))
		return nullptr;

	for (int i = 0; i < INVENQ; ++i)
	{
		if (PtInRect(&m_vecQuickSlot[i]->Get_Rect(), pt))
		{
			CObj* pTemp = Put_Out_Quick(i);
			Put_In_Quick_Direct(i, item);
			return pTemp;
		}
	}

	if (!bUseInven)
		return item;

	for (int i = 0; i < INVENM; ++i)
	{
		if (PtInRect(&m_vecMainInven[i]->Get_Rect(), pt))
		{
			CObj* pTemp = Put_Out_Inven(i);
			Put_In_Inven_Direct(i, item);
			return pTemp;
		}
	}

	if (!dynamic_cast<CItem*>(item)->Get_Equip())
		return item;

	for (int i = 0; i < INVENE; ++i)
	{
		if (PtInRect(&m_vecEquip[i]->Get_Rect(), pt))
		{
			CObj* pTemp = Put_Out_Equip(i);
			Put_In_Equip_Direct(i, item);
			return pTemp;
		}
	}

	return item;
}

bool CInvenMgr::Put_In_Quick_Direct(int Index, CObj* item)
{
	if (Stack_Item(item))
		return true;

	while (1)
	{
		if (dynamic_cast<CInven*>(m_vecQuickSlot[Index])->Get_Item_Obj())
			++Index;
		else
			break;

		if (Index >= INVENQ)
			return false;
	}
	dynamic_cast<CInven*>(m_vecQuickSlot[Index])->Put_in_Item(item);

	return true;
}

bool CInvenMgr::Put_In_Inven_Direct(int Index, CObj* item)
{
	if (Stack_Item(item))
		return true;

	while (1)
	{
		if (dynamic_cast<CInven*>(m_vecMainInven[Index])->Get_Item_Obj())
			++Index;
		else
			break;

		if (Index >= INVENM)
			return false;
	}
	dynamic_cast<CInven*>(m_vecMainInven[Index])->Put_in_Item(item);

	return true;
}

bool CInvenMgr::Put_In_Equip_Direct(int Index, CObj* item)
{
	if (Stack_Item(item))
		return true;

	while (1)
	{
		if (dynamic_cast<CInven*>(m_vecEquip[Index])->Get_Item_Obj())
			++Index;
		else
			break;

		if (Index >= INVENE)
			return false;
	}
	dynamic_cast<CInven*>(m_vecEquip[Index])->Put_in_Item(item);

	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_EquipItemSpec();

	return true;
}

CObj* CInvenMgr::Put_Out_Quick(int Index)
{
	return dynamic_cast<CInven*>(m_vecQuickSlot[Index])->Put_Out_Item();
}

CObj* CInvenMgr::Put_Out_Inven(int Index)
{
	return dynamic_cast<CInven*>(m_vecMainInven[Index])->Put_Out_Item();
}

CObj* CInvenMgr::Put_Out_Equip(int Index)
{
	CObj* pTemp = dynamic_cast<CInven*>(m_vecEquip[Index])->Put_Out_Item();
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_EquipItemSpec();

	return pTemp;
}

void CInvenMgr::Set_Funiture(int Index)
{
	CObj* pTemp = dynamic_cast<CInven*>(m_vecQuickSlot[Index])->Get_Item_Obj();
	dynamic_cast<CInven*>(m_vecQuickSlot[Index])->Reset_Item_Obj();
	dynamic_cast<CItem*>(pTemp)->Add_Count(-1);
}

void CInvenMgr::Erase_Item_Quick(int Index)
{
	dynamic_cast<CInven*>(m_vecQuickSlot[Index])->Put_Out_Item();
}

void CInvenMgr::Erase_Item_Inven(int Index)
{
	dynamic_cast<CInven*>(m_vecMainInven[Index])->Put_Out_Item();
}

void CInvenMgr::Erase_Item_Equip(int Index)
{
	dynamic_cast<CInven*>(m_vecEquip[Index])->Put_Out_Item();
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_EquipItemSpec();
}

CObj* CInvenMgr::Get_Equip_Item_Spec(int Index)
{
	return dynamic_cast<CInven*>(m_vecEquip[Index])->Get_Item_Obj();
}

CObj* CInvenMgr::Get_Item_by_Key(const TCHAR* Input)
{
	for (int i = 0; i < 10; ++i)
	{
		CObj* pTemp = dynamic_cast<CInven*>(m_vecQuickSlot[i])->Get_Item_Obj();
		if (pTemp)
			if (dynamic_cast<CItem*>(pTemp)->Get_ItemKey() == Input)
				return pTemp;
	}

	for (int i = 0; i < 20; ++i)
	{
		CObj* pTemp = dynamic_cast<CInven*>(m_vecMainInven[i])->Get_Item_Obj();
		if (pTemp)
			if (dynamic_cast<CItem*>(pTemp)->Get_ItemKey() == Input)
				return pTemp;
	}

	return nullptr;
}

void CInvenMgr::Add_Item(CObj* item)
{
}

void CInvenMgr::Equip_Item(POINT pt, CObj* item)
{

}

void CInvenMgr::Initialize()
{
	m_vecQuickSlot.resize(INVENQ);
	m_vecMainInven.resize(INVENM);
	m_vecEquip.resize(INVENE);

	for (int i = 0; i < INVENQ; ++i)
	{
		m_vecQuickSlot[i] = CAbstractFactory<CInven>::Create(INVENC * 0.5f + (INVENC * 1.1f) * i, INVENC * 0.5f);
		dynamic_cast<CInven*>(m_vecQuickSlot[i])->Set_QuickNum(i);
	}
		
	dynamic_cast<CInven*>(m_vecQuickSlot[0])->Set_Selected(true);

	for (int i = 1; i < INVENM / INVENQ + 1; ++i)
		for (int j = 0; j < INVENQ; ++j)
			m_vecMainInven[(i-1) * INVENQ + j] = CAbstractFactory<CInven>::Create(INVENC * 0.5f + (INVENC * 1.1f) * j, INVENC * 0.5f + (INVENC * 1.1f) * i);

	for (int i = 0; i < INVENE; ++i)
	{
		m_vecEquip[i] = CAbstractFactory<CInven>::Create(INVENC * 0.5f + (INVENC * 1.1f) * i, INVENC * 0.5f + (INVENC * 1.1f) * 3);
		dynamic_cast<CInven*>(m_vecEquip[i])->Set_Equipment();
	}

	CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Melee", ITEMTYPE::MELEE, false));
	Put_In_Inven_Direct(0, CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));
	CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Gun", ITEMTYPE::GUN, false));
	Put_In_Inven_Direct(1, CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));
	CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Bow", ITEMTYPE::BOW, false));
	Put_In_Inven_Direct(2, CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));
	CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Bomb", ITEMTYPE::THROW, true));
	Put_In_Inven_Direct(3, CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));
	dynamic_cast<CItem*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM))->Add_Count(9);

	CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Pick", ITEMTYPE::PICK, false));
	Put_In_Inven_Direct(4, CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));
	//CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"FireThrower", ITEMTYPE::FIRETHROWER, false));
	//Put_In_Inven_Direct(5, CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));
	CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Axe", ITEMTYPE::AXE, false));
	Put_In_Inven_Direct(5, CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));

	//CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Mine1", ITEMTYPE::THROW, true));
	//Put_In_Inven_Direct(6, CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));
	//dynamic_cast<CItem*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM))->Set_FireSpray();
	//dynamic_cast<CItem*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM))->Add_Count(9);

	//CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Mine2", ITEMTYPE::THROW, true));
	//Put_In_Inven_Direct(7, CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));
	//dynamic_cast<CItem*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM))->Set_High();
	//dynamic_cast<CItem*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM))->Add_Count(9);

	//CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Door", ITEMTYPE::FUNITURE, false));
	//Put_In_Inven_Direct(9, CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));
	//CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Furnace", ITEMTYPE::FUNITURE, false));
	//Put_In_Inven_Direct(10, CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));
	CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"WorkDesk", ITEMTYPE::FUNITURE, false));
	Put_In_Inven_Direct(6, CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));
	//CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"WorkStation", ITEMTYPE::FUNITURE, false));
	//Put_In_Inven_Direct(12, CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));


	CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"SuperJump", ITEMTYPE::SUPERJUMP, false));
	CObj* pTemp = CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM);
	dynamic_cast<CItem*>(pTemp)->Set_Equip();
	dynamic_cast<CItem*>(pTemp)->Set_Special(OBJSPECIAL::SUPERJUMP, true);
	Put_In_Inven_Direct(7, pTemp);

}

void CInvenMgr::Update()
{
	for (auto& iter : m_vecQuickSlot)
		(*iter).Update();

	for (auto& iter : m_vecMainInven)
		(*iter).Update();

	for (auto& iter : m_vecEquip)
		(*iter).Update();
}

void CInvenMgr::Late_Update()
{
	for (auto& iter : m_vecQuickSlot)
		(*iter).Late_Update();

	for (auto& iter : m_vecMainInven)
		(*iter).Late_Update();

	for (auto& iter : m_vecEquip)
		(*iter).Late_Update();
}

void CInvenMgr::Render_Quick(HDC hdc)
{
	for (auto& iter : m_vecQuickSlot)
		(*iter).Render(hdc);
}

void CInvenMgr::Render_Inven(HDC hdc)
{
	for (auto& iter : m_vecMainInven)
		(*iter).Render(hdc);
}

void CInvenMgr::Render_Equip(HDC hdc)
{
	for (auto& iter : m_vecEquip)
		(*iter).Render(hdc);
}

void CInvenMgr::Release()
{
	for (vector<CObj*>::iterator iter = m_vecQuickSlot.begin(); iter != m_vecQuickSlot.end(); )
	{
		Safe_Delete(*iter);
		iter = m_vecQuickSlot.erase(iter);
	}

	for (vector<CObj*>::iterator iter = m_vecMainInven.begin(); iter != m_vecMainInven.end(); )
	{
		Safe_Delete(*iter);
		iter = m_vecMainInven.erase(iter);
	}

	for (vector<CObj*>::iterator iter = m_vecEquip.begin(); iter != m_vecEquip.end(); )
	{
		Safe_Delete(*iter);
		iter = m_vecEquip.erase(iter);
	}

	m_vecQuickSlot.clear();
	m_vecMainInven.clear();
	m_vecEquip.clear();
}

void CInvenMgr::Using_Stack_Item(int iCount)
{
	CObj* pTemp = dynamic_cast<CInven*>(m_vecQuickSlot[m_iSelectdQuickSlot])->Get_Item_Obj();

	if (!pTemp)
		return;

	dynamic_cast<CItem*>(pTemp)->Add_Count(-iCount);

	if (0 == dynamic_cast<CItem*>(pTemp)->Get_Count())
	{
		CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"아이템을 모두 사용하였습니다."));
		dynamic_cast<CInven*>(m_vecQuickSlot[m_iSelectdQuickSlot])->Reset_Item_Obj();
	}
}

bool CInvenMgr::Stack_Item(CObj* Item) // <- false면 못쌓음 (새로 추가해야 함), true면 쌓음 (새로 추가 X)
{
	if (!dynamic_cast<CItem*>(Item)->Get_CanStack())
		return false;

	const TCHAR* pTemp = dynamic_cast<CItem*>(Item)->Get_ItemKey();

	for (auto& iter : m_vecQuickSlot)
	{
		if (!dynamic_cast<CInven*>(iter)->Get_Item_Obj())
			continue;

		CObj* pObj = dynamic_cast<CInven*>(iter)->Get_Item_Obj();

		if (dynamic_cast<CItem*>(pObj)->Get_ItemKey() == pTemp)
		{
			dynamic_cast<CItem*>(pObj)->Add_Count(dynamic_cast<CItem*>(Item)->Get_Count());
			dynamic_cast<CItem*>(Item)->Add_Count(-dynamic_cast<CItem*>(Item)->Get_Count());
			return true;
		}
	}

	for (auto& iter : m_vecMainInven)
	{
		if (!dynamic_cast<CInven*>(iter)->Get_Item_Obj())
			continue;

		CObj* pObj = dynamic_cast<CInven*>(iter)->Get_Item_Obj();

		if (dynamic_cast<CItem*>(pObj)->Get_ItemKey() == pTemp)
		{
			dynamic_cast<CItem*>(pObj)->Add_Count(dynamic_cast<CItem*>(Item)->Get_Count());
			dynamic_cast<CItem*>(Item)->Add_Count(-dynamic_cast<CItem*>(Item)->Get_Count());
			return true;
		}
	}
		
	return false;
}
