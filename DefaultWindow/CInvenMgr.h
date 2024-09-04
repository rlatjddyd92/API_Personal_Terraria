#pragma once
#include "define.h"
#include "CInven.h"
#include "Obj.h"
#include "CItem.h"

class CInvenMgr
{
private:
	CInvenMgr();
	~CInvenMgr();

public:
	// 마우스, 키보드 접근 
	CObj*		Pick_Up_Item_by_Mouse(POINT pt, bool bUseInven);  // <- 인벤토리를 클릭해 아이템 픽업 
	CObj*		Put_In_Item_By_Mouse(POINT pt, CObj* item, bool bUseInven); // <- 아이템을 든 상태에서 인벤토리 클릭해 아이템 넣기

	// 인덱스로 접근 
	bool		Put_In_Quick_Direct(int Index, CObj* item); // <- 아이템을 퀵슬롯에 넣음
	bool		Put_In_Inven_Direct(int Index, CObj* item); // <- 아이템을 인벤에 넣음 
	bool		Put_In_Equip_Direct(int Index, CObj* item);
	CObj*		Put_Out_Quick(int Index); // <- 아이템을 퀵슬롯에서 꺼냄
	CObj*		Put_Out_Inven(int Index); // <- 아이템을 인벤에서 꺼냄
	CObj*		Put_Out_Equip(int Index);
	void		Set_Funiture(int Index); // <- 가구 설치 시 작동 

	void		Erase_Item_Quick(int Index); // <- 아이템 제거 
	void		Erase_Item_Inven(int Index);
	void		Erase_Item_Equip(int Index);

	CObj*		Get_Equip_Item_Spec(int Index);

	CObj*		Get_Item_by_Key(const TCHAR* Input);

public:
	void		Add_Item(CObj* item); // <- Ground의 아이템을 충돌을 통해 획득 
	void		Equip_Item(POINT pt, CObj* item);
	CObj*		Select_Quick(int Input) 
	{ 
		for (auto& iter : m_vecQuickSlot)
			dynamic_cast<CInven*>(iter)->Set_Selected(false);

		dynamic_cast<CInven*>(m_vecQuickSlot[Input])->Set_Selected(true);

		m_iSelectdQuickSlot = Input;

		return dynamic_cast<CInven*>(m_vecQuickSlot[Input])->Get_Item_Obj();
	};
	CObj*		Select_Quick()
	{
		for (int i = 0; i < INVENQ; ++i)
		{
			if (dynamic_cast<CInven*>(m_vecQuickSlot[i])->Get_Selected())
				return dynamic_cast<CInven*>(m_vecQuickSlot[i])->Get_Item_Obj();
		}
	};
	void		Initialize();
	void		Update();
	void		Late_Update();
	void		Render_Quick(HDC hdc);
	void		Render_Inven(HDC hdc);
	void		Render_Equip(HDC hdc);
	void		Release();
	void		Using_Stack_Item(int iCount);

private:
	bool		Stack_Item(CObj* Item);

public:
	static CInvenMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CInvenMgr;

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}


private:
	static CInvenMgr* m_pInstance;

	bool	m_bOpen;

	vector<CObj*>		m_vecQuickSlot;
	vector<CObj*>		m_vecMainInven;
	vector<CObj*>		m_vecEquip;

	int		m_iSelectdQuickSlot;
};

