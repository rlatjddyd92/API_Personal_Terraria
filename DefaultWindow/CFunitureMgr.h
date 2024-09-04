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
	void		UsingFuniture(POINT pt); // 유저가 우클릭 했을 때 가구 이용 시작 혹은 닫기
	void		EndUsing(); // 가구 이용 중 우클릭을 다시 한 경우 (UsingFuniture) 함수에 포함됨
	CObj*		ExchangeItem(POINT pt); // 가구를 이용해 아이템 교환 
	void		Check_Stable(); // 바닥이 부서졌는지 체크, 부서졌으면 가구를 Item-Ground 상태로 되돌림 
	void		Render(HDC hDC); // 설치된 가구는 FunitureMgr이 다 한다 
	bool		SetFuniture(POINT pt, CObj* Funiture); // <- 설치 성공 시 true, 실패 시 false
	void		RemoveFuniture(int iRow, int iCol, bool IsDestroy); // Destroy가 트루면 Item 생성하지 않음 
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

	// Initialize 시, 가구의 Inven 구성
	void		Make_WorkDesk_Menu();
	void		Make_WorkStation_Menu();
	void		Make_Funance_Menu();

	// Render 시, 현재 사용 중인 가구의 메뉴 보이기 
	void		Show_Menu(HDC hDC);

	// Release 시, 구성된 Menu 삭제 
	void		Release_Menu();


	static CFunitureMgr* m_pInstance;
	// 아래 2개 리스트는 서로 짝이 맞아야 함 
	list<CObj*>			 m_SettiedFuniturelist; 
	list<list<INDEX>>	 m_SettiedFunitureIndex;
	CObj*				 m_NowUsingFuniture;

	vector<CObj*>		 m_vecOutput[int(FUNITURETYPE::END)];
	vector<CObj*>		 m_vecInputFirst[int(FUNITURETYPE::END)];
	vector<CObj*>		 m_vecInputSecond[int(FUNITURETYPE::END)];
};

