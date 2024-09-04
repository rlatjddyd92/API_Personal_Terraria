#include "stdafx.h"
#include "CTileMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CItem.h"
#include "CFire.h"
#include "CFunitureMgr.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

void CTileMgr::Initialize()
{
	m_vecMapTile.resize(MAPROW+1);

	for (auto& iter : m_vecMapTile)
		iter.resize(MAPCOL);

	Load_Map();

	for (auto& iter : m_vecMapTile[MAPROW - 2])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW - 1])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW])
		(iter) = TILETYPE::BEDROCK;
}

void CTileMgr::MakeTile(TILETYPE ttype, POINT pt) 
{
	float iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	int iRow = int(-iScrollY + pt.y) / TILECY;
	int iCol = int(-iScrollX + pt.x) / TILECX;

	if ((0 > iRow) || (0 > iCol))
		return;

	if ((MAPROW-2 <= iRow) || (MAPCOL <= iCol))
		return;

	if (TILETYPE::TREE == ttype)
		MakeTree(pt);
	if (TILETYPE::EMPTY == ttype)
	{
		if (TILETYPE::TREE == m_vecMapTile[iRow][iCol])
			Remove_Tree(iRow, iCol);
		else 
			m_vecMapTile[iRow][iCol] = ttype;
	}
	else
		m_vecMapTile[iRow][iCol] = ttype;

	for (auto& iter : m_vecMapTile[MAPROW - 2])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW - 1])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW])
		(iter) = TILETYPE::BEDROCK;
}

void CTileMgr::MakeTile(TILETYPE ttype, INDEX Input)
{
	int iRow = Input.iRow;
	int iCol = Input.iCol;

	if ((0 > iRow) || (0 > iCol))
		return;

	if ((MAPROW - 2 <= iRow) || (MAPCOL <= iCol))
		return;

	m_vecMapTile[iRow][iCol] = ttype;

	for (auto& iter : m_vecMapTile[MAPROW - 2])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW - 1])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW])
		(iter) = TILETYPE::BEDROCK;
}

bool CTileMgr::MakeTile(const TCHAR* pKey, POINT pt)
{
	float iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	int iRow = int(-iScrollY + pt.y) / TILECY;
	int iCol = int(-iScrollX + pt.x) / TILECX;

	if ((0 > iRow) || (0 > iCol))
		return false;

	if ((MAPROW - 2 <= iRow) || (MAPCOL <= iCol))
		return false;

	if (m_vecMapTile[iRow][iCol] != TILETYPE::EMPTY)
		return false;

	if (pKey == L"Dirt") { m_vecMapTile[iRow][iCol] = TILETYPE::DIRT; }
	if (pKey == L"Rock") { m_vecMapTile[iRow][iCol] = TILETYPE::ROCK; }
	if (pKey == L"Copper") { m_vecMapTile[iRow][iCol] = TILETYPE::COPPER; }
	if (pKey == L"Iron") { m_vecMapTile[iRow][iCol] = TILETYPE::IORN; }
	if (pKey == L"Silver") { m_vecMapTile[iRow][iCol] = TILETYPE::SILVER; }
	if (pKey == L"Sand") { m_vecMapTile[iRow][iCol] = TILETYPE::SAND; }
	if (pKey == L"Gold") { m_vecMapTile[iRow][iCol] = TILETYPE::GOLD; }
	if (pKey == L"Wooden") { m_vecMapTile[iRow][iCol] = TILETYPE::WOODEN; }
	if (pKey == L"Coal") { m_vecMapTile[iRow][iCol] = TILETYPE::COAL; }

	for (auto& iter : m_vecMapTile[MAPROW - 2])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW - 1])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW])
		(iter) = TILETYPE::BEDROCK;
}

void CTileMgr::MakeTree(POINT pt)
{
	float iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	int iRow = int(-iScrollY + pt.y) / TILECY;
	int iCol = int(-iScrollX + pt.x) / TILECX;

	if ((0 > iRow) || (0 > iCol))
		return;

	if ((MAPROW - 2 <= iRow) || (MAPCOL <= iCol))
		return;

	int iCount = 0; 

	while (1)
	{
		++iCount;

		if (15 < iCount)
			return;

		if (0 > iRow + iCount)
			break;

		if (TILETYPE::EMPTY == m_vecMapTile[iRow + iCount][iCol])
			continue;
		else
			break;
	}

	for (int i = 0; i < iCount; ++i)
		m_vecMapTile[iRow + i][iCol] = TILETYPE::TREE;

	for (auto& iter : m_vecMapTile[MAPROW - 2])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW - 1])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW])
		(iter) = TILETYPE::BEDROCK;
}

void CTileMgr::Late_Update()
{

}

void CTileMgr::Render(HDC hDC)
{
	float iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	int Left = (-int(iScrollX) - TILECX) / TILECX;
	int Top = (-int(iScrollY) - TILECY) / TILECY;
	int Right = ((WINCX - int(iScrollX)) + TILECX) / TILECX;
	int Bottom = ((WINCY - int(iScrollY)) + TILECY) / TILECY;

	if (Left < 0) { Left = 0; }
	if (Top < 0) { Top = 0; }
	if (Right >= MAPCOL) { Right = MAPCOL - 1; }
	if (Bottom >= MAPROW) { Bottom = MAPROW -1; }

	for (int i = Top; i <= Bottom; ++i)
		for (int j = Left; j <= Right; ++j)
		{
			if (TILETYPE::EMPTY == m_vecMapTile[i][j])
				continue;

			if (TILETYPE::FUNITURE == m_vecMapTile[i][j])
				continue;

			bool bIsGrass = false;
			bool bTree = false;
			bool bTreetop = false;
			bool bBurnTree = false;
			HDC hTile = nullptr;
			HDC hGrass = CBmpMgr::Get_Instance()->Find_Img(L"Grass");
			HDC hTop = CBmpMgr::Get_Instance()->Find_Img(L"Tree_Tops");
			
			switch (m_vecMapTile[i][j])
			{
			case TILETYPE::DIRT:
				hTile = CBmpMgr::Get_Instance()->Find_Img(L"Dirt");
				bIsGrass = true;
				break;
			case TILETYPE::ROCK:
				hTile = CBmpMgr::Get_Instance()->Find_Img(L"Rock");
				break;
			case TILETYPE::COPPER:
				hTile = CBmpMgr::Get_Instance()->Find_Img(L"Copper");
				break;
			case TILETYPE::IORN:
				hTile = CBmpMgr::Get_Instance()->Find_Img(L"Iron");
				break;
			case TILETYPE::SILVER:
				hTile = CBmpMgr::Get_Instance()->Find_Img(L"Silver");
				break;
			case TILETYPE::SAND:
				hTile = CBmpMgr::Get_Instance()->Find_Img(L"Sand");
				break;
			case TILETYPE::BEDROCK:
				hTile = CBmpMgr::Get_Instance()->Find_Img(L"BedRock");
				break;
			case TILETYPE::PRISON:
				hTile = CBmpMgr::Get_Instance()->Find_Img(L"Prison");
				break;
			case TILETYPE::GOLD:
				hTile = CBmpMgr::Get_Instance()->Find_Img(L"Gold");
				break;
			case TILETYPE::WOODEN:
				hTile = CBmpMgr::Get_Instance()->Find_Img(L"Wooden");
				break;
			case TILETYPE::COAL:
				hTile = CBmpMgr::Get_Instance()->Find_Img(L"Coal");
				break;
			case TILETYPE::TREE:
				hTile = CBmpMgr::Get_Instance()->Find_Img(L"Tree_2");
				bTree = true;
				break;
			case TILETYPE::BURNTREE:
				hTile = CBmpMgr::Get_Instance()->Find_Img(L"Tree_2");
				bBurnTree = true;
			default:
				break;
			}
			
			float fStartX = 0;
			float fStartY = 0;

			bool bEmpty[4] = { 1,1,1,1 };
			


			if ((0 < i) && (TILETYPE::EMPTY == m_vecMapTile[i - 1][j])) // 위
				bEmpty[0] = 0;

			if ((MAPROW > i) && (TILETYPE::EMPTY == m_vecMapTile[i + 1][j])) // 아래 
				bEmpty[1] = 0;
			
			if ((0 < j) && (TILETYPE::EMPTY == m_vecMapTile[i][j - 1])) // 왼쪽
				bEmpty[2] = 0;

			if ((MAPCOL - 1> j) && (TILETYPE::EMPTY == m_vecMapTile[i][j + 1])) // 오른쪽
				bEmpty[3] = 0;

			if ((0 < i) && (TILETYPE::FUNITURE == m_vecMapTile[i - 1][j])) // 위
				bEmpty[0] = 0;

			if ((MAPROW > i) && (TILETYPE::FUNITURE == m_vecMapTile[i + 1][j])) // 아래 
				bEmpty[1] = 0;

			if ((0 < j) && (TILETYPE::FUNITURE == m_vecMapTile[i][j - 1])) // 왼쪽
				bEmpty[2] = 0;

			if ((MAPCOL - 1 > j) && (TILETYPE::FUNITURE == m_vecMapTile[i][j + 1])) // 오른쪽
				bEmpty[3] = 0;

			if ((0 == bEmpty[0]) && (bTree)) { bTreetop = true; }

			if ((0 == bEmpty[0]) && (bBurnTree)) { bTreetop = true; }

			if ((0 == bEmpty[0]) && (0 == bEmpty[1]) && (0 == bEmpty[2]) && (0 == bEmpty[3])) // 상하좌우 모두 비었음
				fStartX = 18 * 9, fStartY = 18 * 3;
			else if ((0 == bEmpty[0]) && (0 == bEmpty[1]) && (0 == bEmpty[2]) && (1 == bEmpty[3])) // 상하좌 비었음
				fStartX = 18 * 9, fStartY = 18 * 0;
			else if ((0 == bEmpty[0]) && (0 == bEmpty[1]) && (1 == bEmpty[2]) && (0 == bEmpty[3])) // 상하우 비었음
				fStartX = 18 * 12, fStartY = 18 * 0;
			else if ((0 == bEmpty[0]) && (0 == bEmpty[1]) && (1 == bEmpty[2]) && (1 == bEmpty[3])) // 상하 비었음
				fStartX = 18 * 6, fStartY = 18 * 4;
			else if ((0 == bEmpty[0]) && (1 == bEmpty[1]) && (0 == bEmpty[2]) && (0 == bEmpty[3])) // 상좌우 비었음
				fStartX = 18 * 6, fStartY = 18 * 0;
			else if ((0 == bEmpty[0]) && (1 == bEmpty[1]) && (0 == bEmpty[2]) && (1 == bEmpty[3])) // 상좌 비었음
				fStartX = 18 * 0, fStartY = 18 * 3;
			else if ((0 == bEmpty[0]) && (1 == bEmpty[1]) && (1 == bEmpty[2]) && (0 == bEmpty[3])) // 상우 비었음
				fStartX = 18 * 1, fStartY = 18 * 3;
			else if ((0 == bEmpty[0]) && (1 == bEmpty[1]) && (1 == bEmpty[2]) && (1 == bEmpty[3])) // 상 비었음
				fStartX = 18 * 1, fStartY = 18 * 0;
			else if ((1 == bEmpty[0]) && (0 == bEmpty[1]) && (0 == bEmpty[2]) && (0 == bEmpty[3])) // 하좌우 비었음
				fStartX = 18 * 6, fStartY = 18 * 3;
			else if ((1 == bEmpty[0]) && (0 == bEmpty[1]) && (0 == bEmpty[2]) && (1 == bEmpty[3])) // 하좌 비었음
				fStartX = 18 * 0, fStartY = 18 * 4;
			else if ((1 == bEmpty[0]) && (0 == bEmpty[1]) && (1 == bEmpty[2]) && (0 == bEmpty[3])) // 하우 비었음
				fStartX = 18 * 1, fStartY = 18 * 4;
			else if ((1 == bEmpty[0]) && (0 == bEmpty[1]) && (1 == bEmpty[2]) && (1 == bEmpty[3])) // 하 비었음
				fStartX = 18 * 1, fStartY = 18 * 2;
			else if ((1 == bEmpty[0]) && (1 == bEmpty[1]) && (0 == bEmpty[2]) && (0 == bEmpty[3])) // 좌우 비었음
				fStartX = 18 * 5, fStartY = 18 * 0;
			else if ((1 == bEmpty[0]) && (1 == bEmpty[1]) && (0 == bEmpty[2]) && (1 == bEmpty[3])) // 좌 비었음
				fStartX = 18 * 0, fStartY = 18 * 0;
			else if ((1 == bEmpty[0]) && (1 == bEmpty[1]) && (1 == bEmpty[2]) && (0 == bEmpty[3])) // 우 비었음
				fStartX = 18 * 4, fStartY = 18 * 0;
			else if ((1 == bEmpty[0]) && (1 == bEmpty[1]) && (1 == bEmpty[2]) && (1 == bEmpty[3])) // 꽉참
			{
				fStartX = 18 * 1, fStartY = 18 * 1;
				bIsGrass = false;
			}
				
			if (bIsGrass == true)
				GdiTransparentBlt(hDC, (j * TILECX) + iScrollX,
					(i * TILECY) + iScrollY,
					TILECX,
					TILECY,
					hGrass,
					fStartX,
					fStartY,
					16,
					16,
					RGB(255, 255, 255));
			else if (bTree == true)
			{
				GdiTransparentBlt(hDC, (j* TILECX) + iScrollX,
					(i* TILECY) + iScrollY,
					TILECX,
					TILECY,
					hTile,
					68,
					132,
					16,
					16,
					RGB(255, 255, 255));

				if (bTreetop)
					GdiTransparentBlt(hDC, ((j-2) * TILECX) + iScrollX,
						((i-5) * TILECY) + iScrollY,
						TILECX*5,
						TILECY*5,
						hTop,
						0,
						0,
						80,
						80,
						RGB(255, 255, 255));
			}
			else if (bBurnTree == true)
			{
				GdiTransparentBlt(hDC, (j * TILECX) + iScrollX,
					(i * TILECY) + iScrollY,
					TILECX,
					TILECY,
					hTile,
					772,
					132,
					16,
					16,
					RGB(255, 255, 255));

				if (bTreetop)
					GdiTransparentBlt(hDC, (j * TILECX) + iScrollX,
						((i-1) * TILECY) + iScrollY +4,
						16,
						12,
						hTile,
						706,
						224,
						16,
						12,
						RGB(255, 255, 255));
			}
			else 
				GdiTransparentBlt(hDC, (j * TILECX) + iScrollX, 
					(i * TILECY) + iScrollY, 
					TILECX, 
					TILECY, 
					hTile, 
					fStartX,
					fStartY,
					16, 
					16, 
					RGB(255, 255, 255));
		}

	if (0 != m_iMiningNum)
		Render_Crack(hDC);
}

void CTileMgr::Render_Crack(HDC hDC)
{
	float iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hTile = CBmpMgr::Get_Instance()->Find_Img(L"Crack");

	int iStartY = (m_iMiningNum / 5) * 18;

	GdiTransparentBlt(hDC, (m_iPick_Col * TILECX) + iScrollX,
		(m_iPick_Row * TILECY) + iScrollY,
		TILECX,
		TILECY,
		hTile,
		0,
		iStartY,
		16,
		16,
		RGB(255, 255, 255));
}

void CTileMgr::Release()
{
	for (int i = 0; i < MAPROW; ++i)
		m_vecMapTile[i].clear();
		
	m_vecMapTile.clear();
}

void CTileMgr::DeleteTile(POINT pt)
{
	float iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	int iRow = int(-iScrollY + pt.y) / TILECY;
	int iCol = int(-iScrollX + pt.x) / TILECX;

	if (MAPROW - 2 <= iRow)
		return;

	if ((m_vecMapTile[iCol][iRow] == TILETYPE::TREE) || (m_vecMapTile[iCol][iRow] == TILETYPE::BURNTREE))
		Remove_Tree(iRow, iCol);

	if (TILETYPE::FUNITURE == m_vecMapTile[iCol][iRow])
		CFunitureMgr::Get_Instance()->RemoveFuniture(iRow, iCol,false);

	m_vecMapTile[iCol][iRow] = TILETYPE::EMPTY;

	for (auto& iter : m_vecMapTile[MAPROW - 2])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW - 1])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW])
		(iter) = TILETYPE::BEDROCK;
}

INFO CTileMgr::Coillsion_Tile(CObj* CollisionObj)
{
	INFO tInfo = CollisionObj->Get_Info();
	Vector2D vVec = CollisionObj->Get_Vector2D();

	int iCount = 0; 

	while (true)
	{
		int StartRow = int(tInfo.fY - tInfo.fCY * 0.5f) / TILECY;
		int StartCol = int(tInfo.fX - tInfo.fCX * 0.5f) / TILECX;
		int EndRow = int(tInfo.fY + tInfo.fCY * 0.5f) / TILECY;
		int EndCol = int(tInfo.fX + tInfo.fCX * 0.5f) / TILECX;

		if (StartRow < 0) { StartRow = 0; }
		if (StartCol < 0) { StartCol = 0; }
		if (EndRow >= MAPROW) { EndRow = MAPROW - 1; }
		if (EndCol >= MAPCOL) { EndCol = MAPCOL - 1; }

		INFO tTemp = Adjust_Rect(tInfo, vVec, StartRow, StartCol, EndRow, EndCol, CollisionObj);

		if (isnan(tInfo.fY) || isnan(tInfo.fX))
		{
			tInfo.fX = -1;
			tInfo.fY = -1;
			break;
		}

		if (CollisionObj->Get_OBJID() == OBJID::FIRE)
		{
			tInfo = tTemp;

			return tInfo;
		}
			

		if ((tInfo.fX == tTemp.fX) && (tInfo.fY == tTemp.fY))
			break;

		if (10 < iCount)
		{
			tInfo.fX = -1;
			tInfo.fY = -1;
			break;
		}


		if ((0 > tTemp.fX - tInfo.fCX * 0.5f)||(MAPCOL* TILECX < tTemp.fX + tInfo.fCX * 0.5f))
		{
			vVec.fX = vVec.fX * -1;
			//vVec.fY = vVec.fY * -1;
			tInfo = CollisionObj->Get_Info();

			++iCount;
 			continue;
		}

		if ((0 > tTemp.fY - tInfo.fCY * 0.5f) || (MAPROW * TILECY < tTemp.fY + tInfo.fCY * 0.5f))
		{
			//vVec.fX = vVec.fX * -1;
			vVec.fY = vVec.fY * -1;
			tInfo = CollisionObj->Get_Info();

			++iCount;
			continue;
		}
		
		tInfo = tTemp;
	}

	return tInfo;
}

bool CTileMgr::IsGround(CObj* CollisionObj)
{
	INFO tInfo = CollisionObj->Get_Info();

	int Row = int(tInfo.fY + tInfo.fCY * 0.5f + 2) / TILECY;
	int StartCol = int(tInfo.fX - tInfo.fCX * 0.5f) / TILECX;
	int EndCol = int(tInfo.fX + tInfo.fCX * 0.5f) / TILECX;

	if (0 > Row) { Row = 0; }
	if (MAPROW <= Row) { Row = MAPROW - 1; }
	if (0 > StartCol) { StartCol = 0;}
	if (MAPCOL <= EndCol) { EndCol = MAPCOL - 1; }

	for (int i = StartCol; i <= EndCol; ++i)
	{
		if (CollisionObj->Get_OBJID() != OBJID::FIRE)
			if ((TILETYPE::EMPTY != m_vecMapTile[Row][i]) && (TILETYPE::TREE != m_vecMapTile[Row][i]) && (TILETYPE::FUNITURE != m_vecMapTile[Row][i]))
				if (TILETYPE::BURNTREE != m_vecMapTile[Row][i])
					return true;

		if (CollisionObj->Get_OBJID() == OBJID::FIRE)
			if (TILETYPE::EMPTY != m_vecMapTile[Row][i])
				return true;
	}
		

	return false;
}

void CTileMgr::Explosion_Tile(CObj* Explosion, bool Make)
{
	INFO tInfo = Explosion->Get_Info();

	int StartRow = int(tInfo.fY - tInfo.fCY * 0.5f) / TILECY;
	int StartCol = int(tInfo.fX - tInfo.fCX * 0.5f) / TILECX;
	int EndRow = int(tInfo.fY + tInfo.fCY * 0.5f) / TILECY;
	int EndCol = int(tInfo.fX + tInfo.fCX * 0.5f) / TILECX;

	if (StartRow < 0) { StartRow = 0; }
	if (StartCol < 0) { StartCol = 0; }
	if (EndRow >= MAPROW - 2) { EndRow = MAPROW - 3; }
	if (EndCol >= MAPCOL) { EndCol = MAPCOL - 1; }

	for (int i = StartRow; i <= EndRow; ++i)
		for (int j = StartCol; j <= EndCol; ++j)
			{
				if (TILETYPE::BEDROCK == m_vecMapTile[i][j])
					continue;

				float ColisionSize = Explosion->Get_Info().fCX * 0.5f + TILECX * 0.5f;

				float XDistance = Explosion->Get_Info().fX - ((j * TILECX) + (TILECX * 0.5f));
				float YDistance = Explosion->Get_Info().fY - ((i * TILECY) + (TILECY * 0.5f));

				float Distance = sqrt(XDistance * XDistance + YDistance * YDistance);

				if (Distance <= ColisionSize)
				{
					if (Make)
						m_vecMapTile[i][j] = TILETYPE::PRISON;
					else
					{
						if ((m_vecMapTile[i][j] == TILETYPE::TREE) || (m_vecMapTile[i][j] == TILETYPE::BURNTREE))
							Remove_Tree(i, j);
						else if (m_vecMapTile[i][j] == TILETYPE::FUNITURE)
							CFunitureMgr::Get_Instance()->RemoveFuniture(i, j, true);
						else
							m_vecMapTile[i][j] = TILETYPE::EMPTY;
					}
						
				}
			}

	for (auto& iter : m_vecMapTile[MAPROW - 2])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW - 1])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW])
		(iter) = TILETYPE::BEDROCK;
}

void CTileMgr::Pick_Tile(POINT pt)
{
	float iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	int iRow = int(-iScrollY + pt.y) / TILECY;
	int iCol = int(-iScrollX + pt.x) / TILECX;

	if (0 > iRow) { return; }
	if (0 > iCol) { return; }
	if (MAPCOL <= iCol) { return; }
	if (MAPROW -2 <= iRow) { return; }
	if (m_vecMapTile[iRow][iCol] == TILETYPE::EMPTY) { return; }
	if (m_vecMapTile[iRow][iCol] == TILETYPE::FUNITURE) { return; }

	if ((iRow != m_iPick_Row) || (iCol != m_iPick_Col))
	{
		m_iPick_Row = iRow;
		m_iPick_Col = iCol;
		m_iMiningNum = 0;
	}
	else
	{
		++m_iMiningNum;
	}

	if ((20 <= m_iMiningNum) && (m_vecMapTile[iRow][iCol] != TILETYPE::TREE))
	{
		if (m_vecMapTile[iRow][iCol] != TILETYPE::BURNTREE)
		{
			switch (m_vecMapTile[m_iPick_Row][m_iPick_Col])
			{
			case TILETYPE::DIRT:
				CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Dirt", ITEMTYPE::TILE, true));
				break;
			case TILETYPE::ROCK:
				CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Rock", ITEMTYPE::TILE, true));
				break;
			case TILETYPE::COPPER:
				CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Copper", ITEMTYPE::TILE, true));
				break;
			case TILETYPE::IORN:
				CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Iron", ITEMTYPE::TILE, true));
				break;
			case TILETYPE::SILVER:
				CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Silver", ITEMTYPE::TILE, true));
				break;
			case TILETYPE::SAND:
				CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Sand", ITEMTYPE::TILE, true));
				break;
			case TILETYPE::PRISON:
				CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Prison", ITEMTYPE::TILE, true));
				break;
			case TILETYPE::GOLD:
				CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Gold", ITEMTYPE::TILE, true));
				break;
			case TILETYPE::WOODEN:
				CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Wooden", ITEMTYPE::TILE, true));
				break;
			case TILETYPE::COAL:
				CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Coal", ITEMTYPE::TILE, true));
				break;
			default:
				break;
			}
			CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM)->Set_Pos(m_iPick_Col * TILECX + TILECX * 0.5f, m_iPick_Row * TILECY + TILECY * 0.5f);
			dynamic_cast<CItem*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM))->Set_ItemMode(ITEMMODE::GROUND);
			m_vecMapTile[m_iPick_Row][m_iPick_Col] = TILETYPE::EMPTY;
		}
		else 
			Remove_Tree(pt);
	}
	else if ((20 <= m_iMiningNum) && (m_vecMapTile[iRow][iCol] == TILETYPE::TREE))
		Remove_Tree(pt);

	for (auto& iter : m_vecMapTile[MAPROW - 2])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW - 1])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW])
		(iter) = TILETYPE::BEDROCK;
}

void CTileMgr::Remove_Tree(POINT pt)
{
	float iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	int iRow = int(-iScrollY + pt.y) / TILECY;
	int iCol = int(-iScrollX + pt.x) / TILECX;

	if (MAPROW - 2 <= iRow)
		return;

	if (0 > iRow) { return; }
	if (0 > iCol) { return; }
	if (MAPCOL <= iCol) { return; }
	if (MAPROW - 2 <= iRow) { return; }

	if (m_vecMapTile[iRow][iCol] == TILETYPE::EMPTY)
		return;

	CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Wooden", ITEMTYPE::TILE, true));
	CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM)->Set_Pos(iCol * TILECX + TILECX * 0.5f, iRow * TILECY + TILECY * 0.5f);
	dynamic_cast<CItem*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM))->Set_ItemMode(ITEMMODE::GROUND);
	m_vecMapTile[iRow][iCol] = TILETYPE::EMPTY;

	for(int i = 16; i > -16; --i)
	{
		if ((m_vecMapTile[iRow + i][iCol] == TILETYPE::TREE)|| (m_vecMapTile[iRow + i][iCol] == TILETYPE::BURNTREE))
		{
			CObjMgr::Get_Instance()->Add_Object(OBJID::ITEM, CAbstractFactory<CItem>::Create_Item(L"Wooden", ITEMTYPE::TILE, true));
			CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM)->Set_Pos(iCol * TILECX + TILECX * 0.5f, (iRow +i) * TILECY + TILECY * 0.5f);
			dynamic_cast<CItem*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::ITEM))->Set_ItemMode(ITEMMODE::GROUND);
			m_vecMapTile[iRow + i][iCol] = TILETYPE::EMPTY;
		}
	}

	for (auto& iter : m_vecMapTile[MAPROW - 2])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW - 1])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW])
		(iter) = TILETYPE::BEDROCK;
}

void CTileMgr::Remove_Tree(int iRow, int iCol)
{
	if (MAPROW - 2 <= iRow)
		return;

	m_vecMapTile[iRow][iCol] = TILETYPE::EMPTY;

	for (int i = 16; i > -16; --i)
	{
		if (iRow + i < 0)
			continue;

		if ((m_vecMapTile[iRow + i][iCol] == TILETYPE::TREE)||(m_vecMapTile[iRow + i][iCol] == TILETYPE::BURNTREE))
		{
			m_vecMapTile[iRow + i][iCol] = TILETYPE::EMPTY;
		}
	}

	for (auto& iter : m_vecMapTile[MAPROW - 2])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW - 1])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW])
		(iter) = TILETYPE::BEDROCK;
}

void CTileMgr::Burn_Tree(int iRow, int iCol)
{
	if (MAPROW - 2 <= iRow)
		return;

	if (m_vecMapTile[iRow][iCol] == TILETYPE::TREE)
		m_vecMapTile[iRow][iCol] = TILETYPE::BURNTREE;

	for (int i = 16; i > -16; --i)
	{
		if (iRow + i < 0)
			continue;

		if (m_vecMapTile[iRow + i][iCol] == TILETYPE::TREE)
		{
			m_vecMapTile[iRow + i][iCol] = TILETYPE::BURNTREE;
		}
	}

	for (auto& iter : m_vecMapTile[MAPROW - 2])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW - 1])
		(iter) = TILETYPE::BEDROCK;

	for (auto& iter : m_vecMapTile[MAPROW])
		(iter) = TILETYPE::BEDROCK;
}

void CTileMgr::Save_Map()
{
	HANDLE		hFile = CreateFile(L"../Map/Save_Map.dat", // 파일 경로와 이름을 명시
		GENERIC_WRITE, // 파일 접근 모드(GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL, // 공유 방식, 파일이 개방된 상태에서 다른 프로세스가 파일 개방을 하는 것을 허가할 것인가(NULL 공유하지 않음)
		NULL, // 보안 속성, NULL 인 경우 기본 보안 상태
		CREATE_ALWAYS,	// 생성 방식, (CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 열기)
		FILE_ATTRIBUTE_NORMAL, // 파일 속성(읽기 전용, 숨김 파일 등등),FILE_ATTRIBUTE_NORMAL : 특수 속성이 없는 일반 파일 형태 
		NULL); // 생성될 파일의 속성을 제공할 템플릿 파일 ( 사용하지 않기 때문에 NULL)

	DWORD dwBytesWritten;

	TILETYPE tTemp = TILETYPE::EMPTY;

	for (int i = 0; i < MAPROW ; ++i)
		for (int j = 0; j < MAPCOL; ++j)
		{
			tTemp = m_vecMapTile[i][j];
			WriteFile(hFile, &tTemp, sizeof(TILETYPE), &dwBytesWritten, NULL);
		}
			

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Save 완료"), L"성공", MB_OK);
}

void CTileMgr::Load_Map()
{
	HANDLE		hFile = CreateFile(L"../Map/Save_Map.dat", // 파일 경로와 이름을 명시
		GENERIC_READ, // 파일 접근 모드(GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL, // 공유 방식, 파일이 개방된 상태에서 다른 프로세스가 파일 개방을 하는 것을 허가할 것인가(NULL 공유하지 않음)
		NULL, // 보안 속성, NULL 인 경우 기본 보안 상태
		OPEN_EXISTING,	// 생성 방식, (CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 열기)
		FILE_ATTRIBUTE_NORMAL, // 파일 속성(읽기 전용, 숨김 파일 등등),FILE_ATTRIBUTE_NORMAL : 특수 속성이 없는 일반 파일 형태 
		NULL); // 생성될 파일의 속성을 제공할 템플릿 파일 ( 사용하지 않기 때문에 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OKCANCEL);
		return;
	}

	DWORD dwBytesRead;
	TILETYPE tTemp = TILETYPE::EMPTY;

	for (int i = 0; i < MAPROW; ++i)
		for (int j = 0; j < MAPCOL; ++j)
		{
			ReadFile(hFile, &tTemp, sizeof(TILETYPE), &dwBytesRead, NULL);
			m_vecMapTile[i][j] = tTemp;
		}	

	CloseHandle(hFile);

	//MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}

bool CTileMgr::Front_Tlie(CObj* Obj)
{
	INFO tInfo = Obj->Get_Info();

	int Col = 0.f;

	if (Obj->Get_Vector2D().fX < 0)
		Col = int(tInfo.fX - tInfo.fCX * 0.5f) / TILECX - 1;
	else 
		Col = int(tInfo.fX + tInfo.fCX * 0.5f) / TILECX + 1;

	int StartRow = int(tInfo.fY - tInfo.fCY * 0.5f) / TILECY;
	int EndRow = int(tInfo.fY + tInfo.fCY * 0.5f) / TILECY;

	if (0 > Col) { Col = 0; }
	if (MAPCOL <= Col) { Col = MAPCOL - 1; }
	if (0 > StartRow) { StartRow = 0; }
	if (MAPROW <= EndRow) { EndRow = MAPROW - 1; }

	for (int i = StartRow; i <= EndRow; ++i)
		if ((m_vecMapTile[i][Col] != TILETYPE::EMPTY) && (m_vecMapTile[i][Col] != TILETYPE::TREE))
			if (m_vecMapTile[i][Col] != TILETYPE::BURNTREE)
				return true;

	return false;
}

INFO CTileMgr::Adjust_Rect(INFO ObjInfo, Vector2D ObjVector, int StartRow, int StartCol, int EndRow, int EndCol, CObj* Obj)
{
	INFO tTemp = ObjInfo;

	for (int i = StartRow; i <= EndRow; ++i)
		for (int j = StartCol; j <= EndCol; ++j)
			if (TILETYPE::EMPTY != m_vecMapTile[i][j])
			{
				if ((TILETYPE::TREE == m_vecMapTile[i][j]) && (OBJID::FIRE != Obj->Get_OBJID()))
					continue;

				if ((TILETYPE::BURNTREE == m_vecMapTile[i][j]) && (OBJID::FIRE != Obj->Get_OBJID()))
					continue;

				if (TILETYPE::FUNITURE == m_vecMapTile[i][j])
					continue;

				if ((TILETYPE::TREE == m_vecMapTile[i][j]) && (OBJID::FIRE == Obj->Get_OBJID()))
				{
					if (0 >= Obj->Get_Vector2D().fY)
						continue;
					else
					{
						dynamic_cast<CFire*>(Obj)->Burn_Tree();

						tTemp.fX = j * TILECX + TILECX * (0.5f + (rand() % 10) * 0.1f);
						tTemp.fY = i * TILECY - 1;

						return tTemp;
					}
				}
				if ((TILETYPE::BURNTREE == m_vecMapTile[i][j]) && (OBJID::FIRE == Obj->Get_OBJID()))
				{
					if (0 >= Obj->Get_Vector2D().fY)
						continue;
					else
					{
						dynamic_cast<CFire*>(Obj)->Burn_Tree();

						tTemp.fX = j * TILECX + TILECX * (0.5f + (rand() % 10) * 0.1f);
						tTemp.fY = i * TILECY - 1;

						return tTemp;
					}
				}
					

				float fTileTop = i * TILECY;
				float fTileLeft = j * TILECX;
				float fTileBottom = (i * TILECY) + TILECY;
				float fTileRight = (j * TILECX) + TILECX;

				float fObjTop = ObjInfo.fY - ObjInfo.fCY * 0.5f;
				float fObjLeft = ObjInfo.fX - ObjInfo.fCX * 0.5f;
				float fObjBottom = ObjInfo.fY + ObjInfo.fCY * 0.5f;
				float fObjRight = ObjInfo.fX + ObjInfo.fCX * 0.5f;

				float fAdjustX = 0.f;
				float fAdjustY = 0.f;
				
				if (0 < ObjVector.fY)
				{
					fAdjustY = fObjBottom - fTileTop;
					fAdjustX = fAdjustY * (ObjVector.fX / ObjVector.fY);
				}
				else if (0 > ObjVector.fY)
				{
					fAdjustY = fObjTop - fTileBottom;
					fAdjustX = fAdjustY * (ObjVector.fX / ObjVector.fY);
				}

				if ((fTileRight < fObjLeft - fAdjustX) || (fTileLeft > fObjRight - fAdjustX) || (0 == ObjVector.fY))
				{
					if (0 < ObjVector.fX)
					{
						fAdjustX = fObjRight - fTileLeft;
						fAdjustY = fAdjustX * (ObjVector.fY / ObjVector.fX);
					}
					else if (0 > ObjVector.fX)
					{
						fAdjustX = fObjLeft - fTileRight;
						fAdjustY = fAdjustX * (ObjVector.fY / ObjVector.fX);
					}
				}

				if (0 < fAdjustX) { fAdjustX += 1; }
				if (0 < fAdjustY) { fAdjustY += 1; }
				if (0 > fAdjustX) { fAdjustX -= 1; }
				if (0 > fAdjustY) { fAdjustY -= 1; }

				tTemp.fX -= fAdjustX;
				tTemp.fY -= fAdjustY;

				return tTemp;
			}

	return tTemp;
}


