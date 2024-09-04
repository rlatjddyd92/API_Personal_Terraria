#include "stdafx.h"
#include "CScene.h"
#include "CObjMgr.h"

void CScene::Destroy_Scene_Obj()
{
	for (int i = int(OBJID::MONSTER); i <int(OBJID::END); ++i)
		CObjMgr::Get_Instance()->Release(OBJID(i));
}
