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
	// ���콺, Ű���� ���� 
	CObj*		Pick_Up_Item_by_Mouse(POINT pt, bool bUseInven);  // <- �κ��丮�� Ŭ���� ������ �Ⱦ� 
	CObj*		Put_In_Item_By_Mouse(POINT pt, CObj* item, bool bUseInven); // <- �������� �� ���¿��� �κ��丮 Ŭ���� ������ �ֱ�

	// �ε����� ���� 
	bool		Put_In_Quick_Direct(int Index, CObj* item); // <- �������� �����Կ� ����
	bool		Put_In_Inven_Direct(int Index, CObj* item); // <- �������� �κ��� ���� 
	bool		Put_In_Equip_Direct(int Index, CObj* item);
	CObj*		Put_Out_Quick(int Index); // <- �������� �����Կ��� ����
	CObj*		Put_Out_Inven(int Index); // <- �������� �κ����� ����
	CObj*		Put_Out_Equip(int Index);
	void		Set_Funiture(int Index); // <- ���� ��ġ �� �۵� 

	void		Erase_Item_Quick(int Index); // <- ������ ���� 
	void		Erase_Item_Inven(int Index);
	void		Erase_Item_Equip(int Index);

	CObj*		Get_Equip_Item_Spec(int Index);

	CObj*		Get_Item_by_Key(const TCHAR* Input);

public:
	void		Add_Item(CObj* item); // <- Ground�� �������� �浹�� ���� ȹ�� 
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

