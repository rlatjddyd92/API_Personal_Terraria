#include "stdafx.h"
#include "CCollisionMgr.h"

CCollisionMgr* CCollisionMgr::m_pInstance = nullptr;

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
}

INFO CCollisionMgr::Collision_MapBorder(CObj* Obj)
{
	INFO tTemp = Obj->Get_Info();

	if (0 > Obj->Get_Info().fX - Obj->Get_Info().fCX * 0.5f)
		tTemp.fX = 0 + Obj->Get_Info().fCX * 0.5f;

	if (0 > Obj->Get_Info().fY - Obj->Get_Info().fCY * 0.5f)
		tTemp.fY = 0 + Obj->Get_Info().fCY * 0.5f;

	if (TILECX*MAPCOL < Obj->Get_Info().fX + Obj->Get_Info().fCX * 0.5f)
		tTemp.fX = TILECX * MAPCOL - Obj->Get_Info().fCX * 0.5f;

	if (TILECY * MAPROW < Obj->Get_Info().fY + Obj->Get_Info().fCY * 0.5f)
		tTemp.fY = TILECY * MAPROW - Obj->Get_Info().fCY * 0.5f;

	return tTemp;
}

void CCollisionMgr::Collision_Rect(list<CObj*> ObjList, list<CObj*> Opponentlist)
{
	for (auto& iterDest : ObjList)
		for (auto& iterSorc : Opponentlist)
		{
			if (iterDest->Get_Info().fX + iterDest->Get_Info().fCX * 0.5f < iterSorc->Get_Info().fX - iterSorc->Get_Info().fCX * 0.5f)
				continue;

			if (iterDest->Get_Info().fX - iterDest->Get_Info().fCX * 0.5f > iterSorc->Get_Info().fX + iterSorc->Get_Info().fCX * 0.5f)
				continue;

			if (iterDest->Get_Info().fY + iterDest->Get_Info().fCY * 0.5f < iterSorc->Get_Info().fY - iterSorc->Get_Info().fCY * 0.5f)
				continue;

			if (iterDest->Get_Info().fY - iterDest->Get_Info().fCY * 0.5f > iterSorc->Get_Info().fY + iterSorc->Get_Info().fCY * 0.5f)
				continue;

			iterDest->Collision_Rect_Obj(iterSorc);
			iterSorc->Collision_Rect_Obj(iterDest);
		}
}

void CCollisionMgr::Collision_Eillpse(list<CObj*> Eillpselist, list<CObj*> Opponentlist)
{
	for (auto& iterDest : Eillpselist)
		for (auto& iterSorc : Opponentlist)
		{
			float ColisionSize = min(iterDest->Get_Info().fCX, iterDest->Get_Info().fCY) * 0.5f + min(iterSorc->Get_Info().fCX, iterSorc->Get_Info().fCY) * 0.5f;
			
			float XDistance = iterDest->Get_Info().fX - iterSorc->Get_Info().fX;
			float YDistance = iterDest->Get_Info().fY - iterSorc->Get_Info().fY;

			float Distance = sqrt(XDistance * XDistance + YDistance * YDistance);

			if (Distance <= ColisionSize)
			{
				iterDest->Collision_Eillpse_Obj(iterSorc);
				iterSorc->Collision_Eillpse_Obj(iterDest);
			}
		}
}
