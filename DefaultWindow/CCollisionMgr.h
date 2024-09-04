#pragma once

#include "define.h"
#include "Obj.h"


class CCollisionMgr
{
private:
	CCollisionMgr();
	~CCollisionMgr();


public:
	INFO Collision_MapBorder(CObj* Obj);
	void Collision_Rect(list<CObj*> ObjList, list<CObj*> Opponentlist);
	void Collision_Eillpse(list<CObj*> Eillpselist, list<CObj*> Opponentlist);

public:
	static CCollisionMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CCollisionMgr;

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
	static CCollisionMgr* m_pInstance;


};

