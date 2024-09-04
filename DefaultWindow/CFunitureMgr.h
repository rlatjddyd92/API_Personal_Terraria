#pragma once

#include "Define.h"
#include "Obj.h"

class CFunitureMgr
{
public:
	CFunitureMgr() : m_NowUsingFuniture(nullptr) {}
	~CFunitureMgr() { Release(); }

public:
	void		Initialize();
	void		UsingFuniture(POINT pt); // ������ ��Ŭ�� ���� �� ���� �̿� ���� Ȥ�� �ݱ�
	void		EndUsing(); // ���� �̿� �� ��Ŭ���� �ٽ� �� ��� (UsingFuniture) �Լ��� ���Ե�
	CObj*		ExchangeItem(POINT pt); // ������ �̿��� ������ ��ȯ 
	void		Check_Stable(); // �ٴ��� �μ������� üũ, �μ������� ������ Item-Ground ���·� �ǵ��� 
	void		Render(HDC hDC); // ��ġ�� ������ FunitureMgr�� �� �Ѵ� 
	bool		SetFuniture(POINT pt, CObj* Funiture); // <- ��ġ ���� �� true, ���� �� false
	void		RemoveFuniture(int iRow, int iCol, bool IsDestroy); // Destroy�� Ʈ��� Item �������� ���� 
	void		Late_Update();
	void		Release();

public:
	static CFunitureMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CFunitureMgr;

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
	bool		Set_1X3(int iRow, int iCol); // 1*3
	bool		Set_2X2(int iRow, int iCol); // 2*2
	bool		Set_3X3(int iRow, int iCol); // 3*3

	void		Using_Door();

	// Initialize ��, ������ Inven ����
	void		Make_WorkDesk_Menu();
	void		Make_WorkStation_Menu();
	void		Make_Funance_Menu();

	// Render ��, ���� ��� ���� ������ �޴� ���̱� 
	void		Show_Menu(HDC hDC);

	// Release ��, ������ Menu ���� 
	void		Release_Menu();


	static CFunitureMgr* m_pInstance;
	// �Ʒ� 2�� ����Ʈ�� ���� ¦�� �¾ƾ� �� 
	list<CObj*>			 m_SettiedFuniturelist; 
	list<list<INDEX>>	 m_SettiedFunitureIndex;
	CObj*				 m_NowUsingFuniture;

	vector<CObj*>		 m_vecOutput[int(FUNITURETYPE::END)];
	vector<CObj*>		 m_vecInputFirst[int(FUNITURETYPE::END)];
	vector<CObj*>		 m_vecInputSecond[int(FUNITURETYPE::END)];
};

