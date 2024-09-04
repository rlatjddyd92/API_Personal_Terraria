#pragma once

#include "define.h"

class CMyBmp
{
public:
	CMyBmp();
	~CMyBmp();

public:
	HDC			Get_MemDC() { return m_hMemDC; }
	void		Load_Bmp(const TCHAR* pFilePath);
	void		Release();

private:
	HDC			m_hMemDC;
	HBITMAP		m_hBitmap;
	HBITMAP		m_hOldBmp;
};

