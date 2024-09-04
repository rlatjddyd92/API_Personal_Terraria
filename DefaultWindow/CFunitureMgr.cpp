#include "stdafx.h"
#include "CFunitureMgr.h"
#include "CScrollMgr.h"
#include "Obj.h"
#include "CItem.h"
#include "CTileMgr.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CInven.h"
#include "CInvenMgr.h"
#include "CMessage.h"
#include "CSoundMgr.h"
#include "CBomb.h"

CFunitureMgr* CFunitureMgr::m_pInstance = nullptr;

void CFunitureMgr::Initialize()
{
	Make_WorkDesk_Menu();
	Make_WorkStation_Menu();
	Make_Funance_Menu();
}

void CFunitureMgr::UsingFuniture(POINT pt)
{
	if (m_NowUsingFuniture) { EndUsing(); return; } // 가구 사용 중인 경우 닫기

	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	int iRow = int(-ScrollY + pt.y) / TILECY;
	int iCol = int(-ScrollX + pt.x) / TILECX;

	if (0 > iRow) { return; }
	if (0 > iCol) { return; }
	if (MAPCOL <= iCol) { return; }
	if (MAPROW - 2 <= iRow) { return; }

	if (TILETYPE::FUNITURE != CTileMgr::Get_Instance()->Get_Tile(iRow, iCol)) { return; }

	list<CObj*>::iterator iterObj = m_SettiedFuniturelist.begin();
	list<list<INDEX>>::iterator iterIndex = m_SettiedFunitureIndex.begin();




	for (iterObj, iterIndex; iterObj != m_SettiedFuniturelist.end(), iterIndex != m_SettiedFunitureIndex.end(); ++iterObj, ++iterIndex)
	{
		for (list<INDEX>::iterator iteriter = iterIndex->begin(); iteriter != iterIndex->end(); ++iteriter)
			if ((iteriter->iRow == iRow) && (iteriter->iCol == iCol))
			{
				m_NowUsingFuniture = *iterObj;
			}
	}
}

void CFunitureMgr::EndUsing()
{
	m_NowUsingFuniture = nullptr;
}

CObj* CFunitureMgr::ExchangeItem(POINT pt)
{
	if (!m_NowUsingFuniture) { return nullptr; }

	FUNITURETYPE tTemp = dynamic_cast<CItem*>(m_NowUsingFuniture)->Get_FunitureType();
	int iUsingNum = -1; 

	for (int i = 0; i < 5; ++i)
	{
		if (PtInRect(&m_vecOutput[int(tTemp)][i]->Get_Rect(), pt))
		{
			iUsingNum = i;
			break;
		}
	}

	if (-1 == iUsingNum)
		return nullptr;

	
	const TCHAR* tInputf = L"";
	CObj* pFirst = nullptr;

	if (dynamic_cast<CInven*>(m_vecInputFirst[int(tTemp)][iUsingNum])->Get_Item_Obj())
	{
		tInputf = dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputFirst[int(tTemp)][iUsingNum])->Get_Item_Obj())->Get_ItemKey();
		pFirst = CInvenMgr::Get_Instance()->Get_Item_by_Key(tInputf);

		if (!pFirst)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"재료 아이템이 부족합니다."));
			CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Dig.wav", CHANNELID::SOUND_EFFECT, 2.f);
			return nullptr;
		}
			

		if (dynamic_cast<CItem*>(pFirst)->Get_Count() < dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputFirst[int(tTemp)][iUsingNum])->Get_Item_Obj())->Get_Count())
		{
			CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"재료 아이템이 부족합니다."));
			CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Dig.wav", CHANNELID::SOUND_EFFECT, 2.f);
			return nullptr;
		}
			
	}
		
	const TCHAR* tInputs = L"";
	CObj* pSecond = nullptr;
		
	if (dynamic_cast<CInven*>(m_vecInputSecond[int(tTemp)][iUsingNum])->Get_Item_Obj())
	{
		tInputs = dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputSecond[int(tTemp)][iUsingNum])->Get_Item_Obj())->Get_ItemKey();
		pSecond = CInvenMgr::Get_Instance()->Get_Item_by_Key(tInputs);

		if (!pSecond)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"재료 아이템이 부족합니다."));
			CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Dig.wav", CHANNELID::SOUND_EFFECT, 2.f);
			return nullptr;
		}
			

		if (dynamic_cast<CItem*>(pSecond)->Get_Count() < dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputSecond[int(tTemp)][iUsingNum])->Get_Item_Obj())->Get_Count())
		{
			CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"재료 아이템이 부족합니다."));
			CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Dig.wav", CHANNELID::SOUND_EFFECT, 2.f);
			return nullptr;
		}
	}

	if (pFirst)
		dynamic_cast<CItem*>(pFirst)->Add_Count(-dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputFirst[int(tTemp)][iUsingNum])->Get_Item_Obj())->Get_Count());

	if (pSecond)
		dynamic_cast<CItem*>(pSecond)->Add_Count(-dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputSecond[int(tTemp)][iUsingNum])->Get_Item_Obj())->Get_Count());

	CObj* pTemp = dynamic_cast<CInven*>(m_vecOutput[int(tTemp)][iUsingNum])->Get_Item_Obj();

	CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>().Create_Item(dynamic_cast<CItem*>(pTemp)->Get_ItemKey(), dynamic_cast<CItem*>(pTemp)->Get_ItemType(), dynamic_cast<CItem*>(pTemp)->Get_CanStack()));
	memcpy(dynamic_cast<CItem*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM)), pTemp, sizeof(CObj));
	
	if (dynamic_cast<CItem*>(pTemp)->Get_ItemKey() == L"Mine1")
		dynamic_cast<CItem*>(dynamic_cast<CItem*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM)))->Set_FireSpray();

	if (dynamic_cast<CItem*>(pTemp)->Get_ItemKey() == L"Mine2")
		dynamic_cast<CItem*>(dynamic_cast<CItem*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM)))->Set_High();

	
	dynamic_cast<CItem*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM))->Set_ItemMode(ITEMMODE::PICKUP);

	if (true == dynamic_cast<CItem*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM))->Get_CanStack())
		dynamic_cast<CItem*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM))->Add_Count(9);

	return dynamic_cast<CItem*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM));
}

void CFunitureMgr::Check_Stable()
{
}

void CFunitureMgr::Render(HDC hDC)
{
	for (list<CObj*>::iterator iter = m_SettiedFuniturelist.begin(); iter != m_SettiedFuniturelist.end(); ++iter)
	{
		CObj* pTemp = *iter;

		const TCHAR* m_pItemKey = dynamic_cast<CItem*>(pTemp)->Get_ItemKey();

		HDC hFuni = CBmpMgr::Get_Instance()->Find_Img(m_pItemKey);

		float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
		float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();


		if (m_pItemKey == L"Door")
		{
			//GdiTransparentBlt(hDC, pTemp->Get_Rect().left + ScrollX, pTemp->Get_Rect().top + ScrollY, pTemp->Get_Info().fCX, pTemp->Get_Info().fCY, hFuni, 0, 0, 18, 32, RGB(255, 255, 255));
		}
		else if (m_pItemKey == L"Furnace")
		{
			GdiTransparentBlt(hDC, pTemp->Get_Rect().left + ScrollX, pTemp->Get_Rect().top + ScrollY, pTemp->Get_Info().fCX, pTemp->Get_Info().fCY, hFuni, 0, 0, 48, 48, RGB(255, 255, 255));
		}
		else if (m_pItemKey == L"WorkDesk")
		{
			GdiTransparentBlt(hDC, pTemp->Get_Rect().left + ScrollX, pTemp->Get_Rect().top + ScrollY + 10, pTemp->Get_Info().fCX, pTemp->Get_Info().fCY, hFuni, 0, 0, 32, 32, RGB(255, 255, 255));
		}
		else if (m_pItemKey == L"WorkStation")
		{
			GdiTransparentBlt(hDC, pTemp->Get_Rect().left + ScrollX, pTemp->Get_Rect().top + ScrollY, pTemp->Get_Info().fCX, pTemp->Get_Info().fCY, hFuni, 0, 0, 48, 48, RGB(255, 255, 255));
		}

	}

	if (m_NowUsingFuniture)
		Show_Menu(hDC);
}

bool CFunitureMgr::SetFuniture(POINT pt, CObj* Funiture)
{
	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	int iRow = int(-ScrollY + pt.y) / TILECY;
	int iCol = int(-ScrollX + pt.x) / TILECX;

	if (0 > iRow) { return false; }
	if (0 > iCol) { return false; }
	if (MAPCOL <= iCol) { return false; }
	if (MAPROW - 2 <= iRow) { return false; }

	if (TILETYPE::EMPTY != CTileMgr::Get_Instance()->Get_Tile(iRow, iCol)) { return false; }

	FUNITURETYPE tTemp = dynamic_cast<CItem*>(Funiture)->Get_FunitureType();

	bool tResult = false;

	switch (tTemp)
	{
	case FUNITURETYPE::WORKDESK:
		tResult = Set_2X2(iRow, iCol);
		break;
	case FUNITURETYPE::FURNACE:
		tResult = Set_3X3(iRow, iCol);
		break;
	case FUNITURETYPE::WORKSTATION:
		tResult = Set_3X3(iRow, iCol);
		break;
	case FUNITURETYPE::DOOR:
		tResult = Set_1X3(iRow, iCol);
		break;
	case FUNITURETYPE::END:
		break;
	default:
		break;
	}

	if (!tResult)
		return false;

	CObj* pTemp = new CItem;

	memcpy(pTemp, Funiture, sizeof(CItem));

	m_SettiedFuniturelist.push_back(pTemp);
	m_SettiedFuniturelist.back()->Set_Pos(iCol* TILECX + TILECX * 0.5f, iRow * TILECY + TILECY * 0.5f);
	m_SettiedFuniturelist.back()->Set_Size(m_SettiedFuniturelist.back()->Get_Info().fCX * (10.f / 7.f), m_SettiedFuniturelist.back()->Get_Info().fCY * (10.f / 7.f));
	m_SettiedFuniturelist.back()->Late_Update();
	
	pTemp = m_SettiedFuniturelist.back();
	return true;

	// 배치 성공한 경우 반드시 OBjlist와 Invenlist에서 삭제 처리할 것 
}

void CFunitureMgr::RemoveFuniture(int iRow, int iCol, bool IsDestroy)
{
	list<CObj*>::iterator iterObj = m_SettiedFuniturelist.begin();
	list<list<INDEX>>::iterator iterIndex = m_SettiedFunitureIndex.begin();

	for (iterObj, iterIndex; iterObj != m_SettiedFuniturelist.end(), iterIndex != m_SettiedFunitureIndex.end(); ++iterObj, ++iterIndex)
	{
		for (list<INDEX>::iterator iteriter = iterIndex->begin(); iteriter != iterIndex->end(); ++iteriter)
			if ((iteriter->iRow == iRow) && (iteriter->iCol == iCol))
			{
				if (!IsDestroy)
				{
					CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, *iterObj);
					CObj* pTemp = CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM);
					dynamic_cast<CItem*>(pTemp)->Set_ItemMode(ITEMMODE::GROUND);
				}
				
				m_SettiedFuniturelist.erase(iterObj);
				m_SettiedFunitureIndex.erase(iterIndex);

				return;
			}
	}

	return;
}

void CFunitureMgr::Late_Update()
{

	if (!m_NowUsingFuniture)
		return;

	FUNITURETYPE tTemp = dynamic_cast<CItem*>(m_NowUsingFuniture)->Get_FunitureType();

	for (int i = 0; i < 5; ++i)
	{
		if (dynamic_cast<CInven*>(m_vecOutput[int(tTemp)][i])->Get_Item_Obj())
		{
			dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecOutput[int(tTemp)][i])->Get_Item_Obj())->Update();
			dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecOutput[int(tTemp)][i])->Get_Item_Obj())->Late_Update();
		}
			
		if (dynamic_cast<CInven*>(m_vecInputFirst[int(tTemp)][i])->Get_Item_Obj())
		{
			dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputFirst[int(tTemp)][i])->Get_Item_Obj())->Update();
			dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputFirst[int(tTemp)][i])->Get_Item_Obj())->Late_Update();
		}
			
		if (dynamic_cast<CInven*>(m_vecInputSecond[int(tTemp)][i])->Get_Item_Obj())
		{
			dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputSecond[int(tTemp)][i])->Get_Item_Obj())->Update();
			dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputSecond[int(tTemp)][i])->Get_Item_Obj())->Late_Update();
		}
			
	}
	
}

void CFunitureMgr::Release()
{
	for (list<CObj*>::iterator iter = m_SettiedFuniturelist.begin(); iter != m_SettiedFuniturelist.end();)
	{
		Safe_Delete<CObj*>(*iter);
		iter = m_SettiedFuniturelist.erase(iter);
	}
	
	m_SettiedFuniturelist.clear();

	for (list<list<INDEX>>::iterator iter = m_SettiedFunitureIndex.begin(); iter != m_SettiedFunitureIndex.end();)
	{
		for (list<INDEX>::iterator iteriter = iter->begin(); iteriter != iter->end(); )
			iteriter = iter->erase(iteriter);

		iter->clear();
		iter = m_SettiedFunitureIndex.erase(iter);
	}

	m_SettiedFunitureIndex.clear();

	

	for (int i = 0; i < int(FUNITURETYPE::END); ++i)
	{
		for (vector<CObj*>::iterator iter = m_vecOutput[i].begin(); iter != m_vecOutput[i].end(); )
		{
			CObj* pTemp = dynamic_cast<CInven*>((*iter))->Get_Item_Obj();
			Safe_Delete<CObj*>(pTemp);
			Safe_Delete<CObj*>(*iter);
			iter = m_vecOutput[i].erase(iter);
		}

		for (vector<CObj*>::iterator iter = m_vecInputFirst[i].begin(); iter != m_vecInputFirst[i].end(); )
		{
			CObj* pTemp = dynamic_cast<CInven*>((*iter))->Get_Item_Obj();
			Safe_Delete<CObj*>(pTemp);
			Safe_Delete<CObj*>(*iter);
			iter = m_vecInputFirst[i].erase(iter);
		}

		for (vector<CObj*>::iterator iter = m_vecInputSecond[i].begin(); iter != m_vecInputSecond[i].end(); )
		{
			CObj* pTemp = dynamic_cast<CInven*>((*iter))->Get_Item_Obj();
			Safe_Delete<CObj*>(pTemp);
			Safe_Delete<CObj*>(*iter);
			iter = m_vecInputSecond[i].erase(iter);
		}

		m_vecOutput[i].clear();
		m_vecInputFirst[i].clear();
		m_vecInputSecond[i].clear();
	}

	Safe_Delete(m_NowUsingFuniture);



	
}

bool CFunitureMgr::Set_1X3(int iRow, int iCol)
{
	list<INDEX> Templist;
	INDEX tTemp = { 0,0 };

	bool bTile = true; // 타일 겹침 

	if (TILETYPE::EMPTY == CTileMgr::Get_Instance()->Get_Tile(iRow - 1, iCol)) // 위가 비었음
		if (TILETYPE::EMPTY == CTileMgr::Get_Instance()->Get_Tile(iRow + 1, iCol)) // 아래가 비었음
		{
			if (TILETYPE::EMPTY != CTileMgr::Get_Instance()->Get_Tile(iRow + 2, iCol)) // 바닥이 있음
			{
				Templist.push_back({ iRow - 1 ,iCol });
				tTemp = { iRow - 1 ,iCol };
				CTileMgr::Get_Instance()->MakeTile(TILETYPE::FUNITURE, tTemp);
				Templist.push_back({ iRow ,iCol });
				tTemp = { iRow ,iCol };
				CTileMgr::Get_Instance()->MakeTile(TILETYPE::FUNITURE, tTemp);
				Templist.push_back({ iRow + 1,iCol });
				tTemp = { iRow + 1,iCol };
				CTileMgr::Get_Instance()->MakeTile(TILETYPE::FUNITURE, tTemp);

				m_SettiedFunitureIndex.push_back(Templist);
				Templist.clear();

				return true;
			}
			bTile = false; // 공중
		}
			
	if (bTile)
		CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"가구는 지형과 겹치게 배치할 수 없습니다."));
	else 
		CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"가구는 공중에 배치할 수 없습니다."));

	CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_EFFECT);
	CSoundMgr::Get_Instance()->PlaySound(L"Dig.wav", CHANNELID::SOUND_EFFECT, 2.f);

	Templist.clear();
	return false;
}

bool CFunitureMgr::Set_2X2(int iRow, int iCol) // 2X2칸짜리 가구 배치 
{
	list<INDEX> Templist;
	INDEX tTemp = { 0,0 };

	bool bTile = true; // 타일 겹침 

	if (TILETYPE::EMPTY == CTileMgr::Get_Instance()->Get_Tile(iRow, iCol+1)) // 오른쪽 비었음
		if (TILETYPE::EMPTY == CTileMgr::Get_Instance()->Get_Tile(iRow + 1, iCol)) // 아래가 비었음
			if (TILETYPE::EMPTY == CTileMgr::Get_Instance()->Get_Tile(iRow + 1, iCol + 1)) // 오른쪽 아래가 비었음
			{
				if (TILETYPE::EMPTY != CTileMgr::Get_Instance()->Get_Tile(iRow + 2, iCol)) // 왼쪽 바닥 있음 
					if (TILETYPE::EMPTY != CTileMgr::Get_Instance()->Get_Tile(iRow + 2, iCol + 1)) // 오른쪽 바닥 있음 
					{
						Templist.push_back({ iRow ,iCol });
						tTemp = { iRow ,iCol };
						CTileMgr::Get_Instance()->MakeTile(TILETYPE::FUNITURE, tTemp);
						Templist.push_back({ iRow ,iCol + 1 });
						tTemp = { iRow ,iCol + 1 };
						CTileMgr::Get_Instance()->MakeTile(TILETYPE::FUNITURE, tTemp);
						Templist.push_back({ iRow + 1,iCol });
						tTemp = { iRow + 1,iCol };
						CTileMgr::Get_Instance()->MakeTile(TILETYPE::FUNITURE, tTemp);
						Templist.push_back({ iRow + 1,iCol + 1 });
						tTemp = { iRow + 1,iCol + 1 };
						CTileMgr::Get_Instance()->MakeTile(TILETYPE::FUNITURE, tTemp);

						m_SettiedFunitureIndex.push_back(Templist);
						Templist.clear();

						return true;
					}

				bTile = false; // 공중
			}
				

	if (bTile)
		CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"가구는 지형과 겹치게 배치할 수 없습니다."));
	else
		CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"가구는 공중에 배치할 수 없습니다."));

	CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_EFFECT);
	CSoundMgr::Get_Instance()->PlaySound(L"Dig.wav", CHANNELID::SOUND_EFFECT, 2.f);

	Templist.clear();
	return false;
}

bool CFunitureMgr::Set_3X3(int iRow, int iCol)
{
	list<INDEX> Templist;
	INDEX tTemp = { 0,0 };

	for (int i = iRow-1 ; i < iRow + 2; ++i)
		for (int j = iCol - 1; j < iCol + 2; ++j)
			if (TILETYPE::EMPTY != CTileMgr::Get_Instance()->Get_Tile(i, j))
			{
				CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"가구는 지형과 겹치게 배치할 수 없습니다."));
				CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_EFFECT);
				CSoundMgr::Get_Instance()->PlaySound(L"Dig.wav", CHANNELID::SOUND_EFFECT, 2.f);
				Templist.clear();
				return false;
			}

	for (int i = iCol - 1; i < iCol + 2; ++i)
		if (TILETYPE::EMPTY == CTileMgr::Get_Instance()->Get_Tile(iRow + 2, i))
		{
			CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"가구는 공중에 배치할 수 없습니다."));
			CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Dig.wav", CHANNELID::SOUND_EFFECT, 2.f);
			Templist.clear();
			return false;
		}

	for (int i = iRow - 1; i < iRow + 2; ++i)
		for (int j = iCol - 1; j < iCol + 2; ++j)
		{
			Templist.push_back({ i ,j });
			tTemp = { i ,j };
			CTileMgr::Get_Instance()->MakeTile(TILETYPE::FUNITURE, tTemp);
		}

	m_SettiedFunitureIndex.push_back(Templist);
	Templist.clear();
	return true;
}

void CFunitureMgr::Using_Door()
{
}

void CFunitureMgr::Make_WorkDesk_Menu()
{
	for (int i = 0; i < 5; ++i)
	{
		m_vecOutput[int(FUNITURETYPE::WORKDESK)].push_back(CAbstractFactory<CInven>().Create(400 + INVENC * 1.1f * i, 300));
		m_vecInputFirst[int(FUNITURETYPE::WORKDESK)].push_back(CAbstractFactory<CInven>().Create(400 + INVENC * 1.1f * i, 300 + INVENC * 1.1f));
		m_vecInputSecond[int(FUNITURETYPE::WORKDESK)].push_back(CAbstractFactory<CInven>().Create(400 + INVENC * 1.1f * i, 300 + INVENC * 1.0f * 2.f));

		m_vecInputFirst[int(FUNITURETYPE::WORKDESK)].back()->Set_Size(INVENC * 0.8f, INVENC * 0.8f);
		m_vecInputSecond[int(FUNITURETYPE::WORKDESK)].back()->Set_Size(INVENC * 0.8f, INVENC * 0.8f);

		m_vecOutput[int(FUNITURETYPE::WORKDESK)].back()->Late_Update();
		m_vecInputFirst[int(FUNITURETYPE::WORKDESK)].back()->Late_Update();
		m_vecInputSecond[int(FUNITURETYPE::WORKDESK)].back()->Late_Update();
	}
	
	// Funance 제작 - 재료 : 돌 30개
	dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKDESK)][0])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Furnace", ITEMTYPE::FUNITURE, false));
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKDESK)][0])->Get_Item_Obj())->Set_FunitureType(FUNITURETYPE::FURNACE);
	dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKDESK)][0])->Get_Item_Obj()->Late_Update();
	dynamic_cast<CInven*>(m_vecInputFirst[int(FUNITURETYPE::WORKDESK)][0])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Rock", ITEMTYPE::TILE, true));
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputFirst[int(FUNITURETYPE::WORKDESK)][0])->Get_Item_Obj())->Add_Count(20);

	// WorkStation 제작 - 재료 : 나무 30개 
	dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKDESK)][1])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"WorkStation", ITEMTYPE::FUNITURE, false));
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKDESK)][1])->Get_Item_Obj())->Set_FunitureType(FUNITURETYPE::WORKSTATION);
	dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKDESK)][1])->Get_Item_Obj()->Late_Update();
	dynamic_cast<CInven*>(m_vecInputFirst[int(FUNITURETYPE::WORKDESK)][1])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Wooden", ITEMTYPE::TILE, true));
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputFirst[int(FUNITURETYPE::WORKDESK)][1])->Get_Item_Obj())->Add_Count(20);

}

void CFunitureMgr::Make_WorkStation_Menu()
{
	for (int i = 0; i < 5; ++i)
	{
		m_vecOutput[int(FUNITURETYPE::WORKSTATION)].push_back(CAbstractFactory<CInven>().Create(400 + INVENC * 1.1f * i, 300));
		m_vecInputFirst[int(FUNITURETYPE::WORKSTATION)].push_back(CAbstractFactory<CInven>().Create(400 + INVENC * 1.1f * i, 300 + INVENC * 1.1f));
		m_vecInputSecond[int(FUNITURETYPE::WORKSTATION)].push_back(CAbstractFactory<CInven>().Create(400 + INVENC * 1.1f * i, 300 + INVENC * 1.0f * 2.f));

		m_vecInputFirst[int(FUNITURETYPE::WORKSTATION)].back()->Set_Size(INVENC * 0.8f, INVENC * 0.8f);
		m_vecInputSecond[int(FUNITURETYPE::WORKSTATION)].back()->Set_Size(INVENC * 0.8f, INVENC * 0.8f);

		m_vecOutput[int(FUNITURETYPE::WORKSTATION)].back()->Late_Update();
		m_vecInputFirst[int(FUNITURETYPE::WORKSTATION)].back()->Late_Update();
		m_vecInputSecond[int(FUNITURETYPE::WORKSTATION)].back()->Late_Update();
	}

	// 문짝 제작 - 재료 : 나무 10개, 철 주괴 10개 
	dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKSTATION)][0])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Door", ITEMTYPE::FUNITURE, false));
	dynamic_cast<CInven*>(m_vecInputFirst[int(FUNITURETYPE::WORKSTATION)][0])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Wooden", ITEMTYPE::TILE, true));
	dynamic_cast<CInven*>(m_vecInputSecond[int(FUNITURETYPE::WORKSTATION)][0])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"IronBar", ITEMTYPE::BAR, true));
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputSecond[int(FUNITURETYPE::WORKSTATION)][0])->Get_Item_Obj())->Add_Count(9);

	// 화염지뢰 10개 제작 - 재료 : 구리 주괴 10개, 석탄 10개
	dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKSTATION)][1])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Mine1", ITEMTYPE::THROW, true));
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKSTATION)][1])->Get_Item_Obj())->Add_Count(9);
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKSTATION)][1])->Get_Item_Obj())->Set_FireSpray();
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKSTATION)][1])->Get_Item_Obj())->Late_Update();
	dynamic_cast<CInven*>(m_vecInputFirst[int(FUNITURETYPE::WORKSTATION)][1])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"CopperBar", ITEMTYPE::BAR, true));
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputFirst[int(FUNITURETYPE::WORKSTATION)][1])->Get_Item_Obj())->Add_Count(9);
	dynamic_cast<CInven*>(m_vecInputSecond[int(FUNITURETYPE::WORKSTATION)][1])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Coal", ITEMTYPE::TILE, true));

	// 도약지뢰 10개 제작 - 재료 : 철 주괴 10개, 석탄 10개 
	dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKSTATION)][2])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Mine2", ITEMTYPE::THROW, true));
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKSTATION)][2])->Get_Item_Obj())->Add_Count(9);
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKSTATION)][2])->Get_Item_Obj())->Set_High();
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKSTATION)][2])->Get_Item_Obj())->Late_Update();
	dynamic_cast<CInven*>(m_vecInputFirst[int(FUNITURETYPE::WORKSTATION)][2])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"IronBar", ITEMTYPE::BAR, true));
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputFirst[int(FUNITURETYPE::WORKSTATION)][2])->Get_Item_Obj())->Add_Count(9);
	dynamic_cast<CInven*>(m_vecInputSecond[int(FUNITURETYPE::WORKSTATION)][2])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Coal", ITEMTYPE::TILE, true));

	// 화염방사기 제작 - 재료 : 은 주괴 10개, 석탄 10개 
	dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::WORKSTATION)][3])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"FireThrower", ITEMTYPE::FIRETHROWER, false));
	dynamic_cast<CInven*>(m_vecInputFirst[int(FUNITURETYPE::WORKSTATION)][3])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"SilverBar", ITEMTYPE::BAR, true));
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecInputFirst[int(FUNITURETYPE::WORKSTATION)][3])->Get_Item_Obj())->Add_Count(9);
	dynamic_cast<CInven*>(m_vecInputSecond[int(FUNITURETYPE::WORKSTATION)][3])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Coal", ITEMTYPE::TILE, true));

	
}

void CFunitureMgr::Make_Funance_Menu()
{
	for (int i = 0; i < 5; ++i)
	{
		m_vecOutput[int(FUNITURETYPE::FURNACE)].push_back(CAbstractFactory<CInven>().Create(400 + INVENC * 1.1f * i, 300));
		m_vecInputFirst[int(FUNITURETYPE::FURNACE)].push_back(CAbstractFactory<CInven>().Create(400 + INVENC * 1.1f * i, 300 + INVENC * 1.1f));
		m_vecInputSecond[int(FUNITURETYPE::FURNACE)].push_back(CAbstractFactory<CInven>().Create(400 + INVENC * 1.1f * i, 300 + INVENC * 1.0f * 2.f));

		m_vecInputFirst[int(FUNITURETYPE::FURNACE)].back()->Set_Size(INVENC * 0.8f, INVENC * 0.8f);
		m_vecInputSecond[int(FUNITURETYPE::FURNACE)].back()->Set_Size(INVENC * 0.8f, INVENC * 0.8f);

		m_vecOutput[int(FUNITURETYPE::FURNACE)].back()->Late_Update();
		m_vecInputFirst[int(FUNITURETYPE::FURNACE)].back()->Late_Update();
		m_vecInputSecond[int(FUNITURETYPE::FURNACE)].back()->Late_Update();
	}

	// 구리 주괴 10개 제작 - 재료 : 구리 10개, 나무 10개 
	dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::FURNACE)][0])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"CopperBar", ITEMTYPE::BAR, true));
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::FURNACE)][0])->Get_Item_Obj())->Add_Count(9);
	dynamic_cast<CInven*>(m_vecInputFirst[int(FUNITURETYPE::FURNACE)][0])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Copper", ITEMTYPE::TILE, true));
	dynamic_cast<CInven*>(m_vecInputSecond[int(FUNITURETYPE::FURNACE)][0])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Wooden", ITEMTYPE::TILE, true));

	// 철 주괴 10개 제작 - 재료 : 철 10개
	dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::FURNACE)][1])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"IronBar", ITEMTYPE::BAR, true));
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::FURNACE)][1])->Get_Item_Obj())->Add_Count(9);
	dynamic_cast<CInven*>(m_vecInputFirst[int(FUNITURETYPE::FURNACE)][1])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Iron", ITEMTYPE::TILE, true));
	dynamic_cast<CInven*>(m_vecInputSecond[int(FUNITURETYPE::FURNACE)][1])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Wooden", ITEMTYPE::TILE, true));

	// 은 주괴 10개 제작 - 재료 : 은 10개
	dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::FURNACE)][2])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"SilverBar", ITEMTYPE::BAR, true));
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::FURNACE)][2])->Get_Item_Obj())->Add_Count(9);
	dynamic_cast<CInven*>(m_vecInputFirst[int(FUNITURETYPE::FURNACE)][2])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Silver", ITEMTYPE::TILE, true));
	dynamic_cast<CInven*>(m_vecInputSecond[int(FUNITURETYPE::FURNACE)][2])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Wooden", ITEMTYPE::TILE, true));

	// 금 주괴 10개 제작 - 재료 : 금 10개
	dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::FURNACE)][3])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"GoldBar", ITEMTYPE::BAR, true));
	dynamic_cast<CItem*>(dynamic_cast<CInven*>(m_vecOutput[int(FUNITURETYPE::FURNACE)][3])->Get_Item_Obj())->Add_Count(9);
	dynamic_cast<CInven*>(m_vecInputFirst[int(FUNITURETYPE::FURNACE)][3])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Gold", ITEMTYPE::TILE, true));
	dynamic_cast<CInven*>(m_vecInputSecond[int(FUNITURETYPE::FURNACE)][3])->Put_in_Item(CAbstractFactory<CItem>().Create_Item(L"Wooden", ITEMTYPE::TILE, true));


}

void CFunitureMgr::Show_Menu(HDC hDC)
{
	FUNITURETYPE tTemp = dynamic_cast<CItem*>(m_NowUsingFuniture)->Get_FunitureType();

	for (int i = 0; i < 5; ++i)
	{
		dynamic_cast<CInven*>(m_vecOutput[int(tTemp)][i])->Render(hDC);
		dynamic_cast<CInven*>(m_vecInputFirst[int(tTemp)][i])->Render(hDC);
		dynamic_cast<CInven*>(m_vecInputSecond[int(tTemp)][i])->Render(hDC);
	}
}

void CFunitureMgr::Release_Menu()
{
}
