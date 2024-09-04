#include "stdafx.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"
#include "CAbstractFactory.h"
#include "CItem.h"
#include "CMouse.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}

CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Initialize()
{
	m_ObjList[int(OBJID::MOUSE)].push_back(CAbstractFactory<CMouse>::Create());
}

void CObjMgr::Add_Object(OBJID eID, CObj* pObj)
{
	m_ObjList[int(eID)].push_back(pObj);
}

CObj* CObjMgr::Make_Item(const TCHAR* key, ITEMMODE Setting)
{
	return nullptr;
}

void CObjMgr::Update()
{
	if (m_ObjList[int(OBJID::MOUSE)].empty())
		m_ObjList[int(OBJID::MOUSE)].push_back(CAbstractFactory<CMouse>::Create());

	for (int i = int(OBJID::PLAYER); i < int(OBJID::END); ++i)
		for (list<CObj*>::iterator iter = m_ObjList[i].begin(); iter != m_ObjList[i].end();++iter)
			(*iter)->Update();
}

void CObjMgr::Late_Update()
{
	for (int i = int(OBJID::PLAYER); i < int(OBJID::END); ++i)
		for (list<CObj*>::iterator iter = m_ObjList[i].begin(); iter != m_ObjList[i].end();)
		{
			if (OBJRESULT::DEAD == (*iter)->Late_Update())
			{
				Safe_Delete(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}	

	CCollisionMgr::Get_Instance()->Collision_Rect(m_ObjList[int(OBJID::MELEE)], m_ObjList[int(OBJID::MONSTER)]);
	CCollisionMgr::Get_Instance()->Collision_Rect(m_ObjList[int(OBJID::MELEE)], m_ObjList[int(OBJID::PLAYER)]);
	CCollisionMgr::Get_Instance()->Collision_Rect(m_ObjList[int(OBJID::MELEE)], m_ObjList[int(OBJID::BOSS)]);
	CCollisionMgr::Get_Instance()->Collision_Rect(m_ObjList[int(OBJID::MISSILE)], m_ObjList[int(OBJID::MONSTER)]);
	CCollisionMgr::Get_Instance()->Collision_Rect(m_ObjList[int(OBJID::MISSILE)], m_ObjList[int(OBJID::PLAYER)]);
	CCollisionMgr::Get_Instance()->Collision_Rect(m_ObjList[int(OBJID::MISSILE)], m_ObjList[int(OBJID::BOSS)]);
	CCollisionMgr::Get_Instance()->Collision_Eillpse(m_ObjList[int(OBJID::EXPLOSION)], m_ObjList[int(OBJID::MONSTER)]);
	CCollisionMgr::Get_Instance()->Collision_Eillpse(m_ObjList[int(OBJID::EXPLOSION)], m_ObjList[int(OBJID::PLAYER)]);
	CCollisionMgr::Get_Instance()->Collision_Eillpse(m_ObjList[int(OBJID::EXPLOSION)], m_ObjList[int(OBJID::BOSS)]);
	CCollisionMgr::Get_Instance()->Collision_Rect(m_ObjList[int(OBJID::FIRE)], m_ObjList[int(OBJID::MONSTER)]);
	CCollisionMgr::Get_Instance()->Collision_Rect(m_ObjList[int(OBJID::FIRE)], m_ObjList[int(OBJID::PLAYER)]);
	CCollisionMgr::Get_Instance()->Collision_Rect(m_ObjList[int(OBJID::FIRE)], m_ObjList[int(OBJID::BOSS)]);

	CCollisionMgr::Get_Instance()->Collision_Rect(m_ObjList[int(OBJID::PLAYER)], m_ObjList[int(OBJID::ITEM)]);


}

void CObjMgr::Render(HDC hDC)
{
	for (int i = int(OBJID::PLAYER); i < int(OBJID::END); ++i)
		for (list<CObj*>::iterator iter = m_ObjList[i].begin(); iter != m_ObjList[i].end();++iter)
			(*iter)->Render(hDC);
}

void CObjMgr::Release()
{
	for (int i = int(OBJID::PLAYER); i < int(OBJID::END); ++i)
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
}

void CObjMgr::Update(OBJID eID)
{
	for (list<CObj*>::iterator iter = m_ObjList[int(eID)].begin(); iter != m_ObjList[int(eID)].end();++iter)
		(*iter)->Update();
}

void CObjMgr::Late_Update(OBJID eID)
{
	for (list<CObj*>::iterator iter = m_ObjList[int(eID)].begin(); iter != m_ObjList[int(eID)].end();++iter)
		(*iter)->Late_Update();
}

void CObjMgr::Render(HDC hDC, OBJID eID)
{
	for (list<CObj*>::iterator iter = m_ObjList[int(eID)].begin(); iter != m_ObjList[int(eID)].end();++iter)
		(*iter)->Render(hDC);
}

void CObjMgr::Release(OBJID eID)
{
	for (list<CObj*>::iterator iter = m_ObjList[int(eID)].begin(); iter != m_ObjList[int(eID)].end();)
	{
		Safe_Delete(*iter);
		iter = m_ObjList[int(eID)].erase(iter);
	}
}

CObj* CObjMgr::Get_LastObj(OBJID eID)
{
	CObj* pTemp = m_ObjList[int(eID)].back();
	return pTemp;
}

void CObjMgr::Delete_ID(OBJID eID)
{

}

void CObjMgr::Trace_Obj(OBJID eID)
{
}

void CObjMgr::Trace_Obj(OBJID eID, list<CObj*> Exceptionlist)
{
}
