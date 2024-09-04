#pragma once

#include "define.h"
#include "Obj.h"

class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();

public:
	void		Initialize();
	void		Add_Object(OBJID eID, CObj* pObj);
	CObj*		Make_Item(const TCHAR* key, ITEMMODE Setting);
	void		Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

	void		Update(OBJID eID);
	void		Late_Update(OBJID eID);
	void		Render(HDC hDC, OBJID eID);
	void		Release(OBJID eID);

	CObj*		Get_LastObj(OBJID eID);
	void		Delete_ID(OBJID eID);

	void		Trace_Obj(OBJID eID); // <- Ư�� ������ OBJ�� ���� ����� �� ����
	void		Trace_Obj(OBJID eID, list<CObj*> Exceptionlist); // <- ���� ����Ʈ�� �ִ� ���� �����ϰ� ���� 
	CObj*		Get_Player() { return m_ObjList[int(OBJID::PLAYER)].front(); }
	list<CObj*> Get_List(OBJID eID) { return m_ObjList[int(eID)]; }

public:
	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjMgr;

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
	static CObjMgr* m_pInstance;
	list<CObj*>		m_ObjList[int(OBJID::END)];
};

