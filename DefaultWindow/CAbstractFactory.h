#pragma once

#include "Obj.h"
#include "CItem.h"
#include "CStats.h"
#include "CNumber.h"
#include "CMonster.h"
#include "CMessage.h"
#include "CAchievment.h"

template<typename T>
class CAbstractFactory
{
public:
	CAbstractFactory() {}
	~CAbstractFactory() {}

public:
	static CObj* Create()
	{
		CObj* pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CObj* Create(float _fX, float _fY)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);

		return pObj;
	}

	static CObj* Create_Item(const TCHAR* Key, ITEMTYPE type, bool Canstack)
	{
		CObj* pObj = new T;
		dynamic_cast<CItem*>(pObj)->Set_CanStack(Canstack);
		dynamic_cast<CItem*>(pObj)->Set_ItemKey(Key);
		dynamic_cast<CItem*>(pObj)->Set_ItemType(type);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create_Stats(STATS Input, CObj* linkedObj)
	{
		CObj* pObj = new T;
		dynamic_cast<CStats*>(pObj)->Set_Stats_Type(Input);
		dynamic_cast<CStats*>(pObj)->Set_Linked_Obj(linkedObj);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create_Number(float _fX, float _fY, float Input)
	{
		CObj* pObj = new T;
		dynamic_cast<CNumber*>(pObj)->Set_NumBer(Input);
		pObj->Set_Pos(_fX, _fY);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create_Monster(float _fX, float _fY, bool fly, bool range, bool Exp)
	{
		CObj* pObj = new T;
		dynamic_cast<CMonster*>(pObj)->Set_Air(fly);
		dynamic_cast<CMonster*>(pObj)->Set_Range(range);
		dynamic_cast<CMonster*>(pObj)->Set_Exp(Exp);
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		
		return pObj;
	}

	static CObj* Create_Message(const TCHAR* message)
	{
		CObj* pObj = new T;
		dynamic_cast<CMessage*>(pObj)->Set_Message(message);
		pObj->Initialize();

		return pObj;
	}

	static CObj* Create_ToolTip(const TCHAR* ItemKey, INFO ItemInfo)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		dynamic_cast<CMessage*>(pObj)->Set_ItemToolTip();
		dynamic_cast<CMessage*>(pObj)->Set_Message(ItemKey);
		pObj->Set_Pos(ItemInfo.fX, ItemInfo.fY);
		pObj->Initialize();

		return pObj;
	}

	static CObj* Create_Achievement(float _fX, float _fY, ACHIEVMENT Input)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_fX, _fY);
		dynamic_cast<CAchievment*>(pObj)->Set_Achievment(Input);
		pObj->Initialize();

		return pObj;
	}
};

