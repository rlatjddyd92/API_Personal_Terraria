#pragma once

#include "define.h"
#include "Obj.h"

class CTileMgr
{
public:
	CTileMgr() : m_iPick_Row(-1), m_iPick_Col(-1), m_iMiningNum(0) {};
	~CTileMgr() { Release(); };

public:
	void		Initialize();
	void		MakeTile(TILETYPE ttype, POINT pt);
	void		MakeTile(TILETYPE ttype, INDEX Input); // 반드시 가구 배치용으로만 쓸 것 
	bool		MakeTile(const TCHAR* pKey, POINT pt);
	void		MakeTree(POINT pt);
	void		Late_Update();
	void		Render(HDC hDC);
	void		Render_Crack(HDC hDC);
	void		Release();
	void		DeleteTile(POINT pt);
	INFO		Coillsion_Tile(CObj* CollisionObj);
	bool		IsGround(CObj* CollisionObj);
	void		Explosion_Tile(CObj* Explosion, bool Make);
	void		Pick_Tile(POINT pt);
	void		Remove_Tree(POINT pt);
	void		Remove_Tree(int iRow, int iCol);
	void		Burn_Tree(int iRow, int iCol);
	void		Cancel_Pick() { m_iPick_Row = -1; m_iPick_Col = -1; m_iMiningNum = 0; }
	void		Save_Map();
	void		Load_Map();
	bool		Front_Tlie(CObj* Obj);
	TILETYPE	Get_Tile(int iRow, int iCol) { return m_vecMapTile[iRow][iCol]; }

public:
	static CTileMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTileMgr;

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
	INFO		Adjust_Rect(INFO ObjInfo, Vector2D ObjVector, int StartRow, int StartCol, int EndRow, int EndCol, CObj* Obj);
	static CTileMgr* m_pInstance;
	vector<vector<TILETYPE>> m_vecMapTile;
	int			m_iPick_Row;
	int			m_iPick_Col;
	int			m_iMiningNum;
};

