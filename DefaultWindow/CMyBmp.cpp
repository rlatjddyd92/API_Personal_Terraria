#include "stdafx.h"
#include "CMyBmp.h"

CMyBmp::CMyBmp()
{
}

CMyBmp::~CMyBmp()
{
	Release();
}

void CMyBmp::Load_Bmp(const TCHAR* pFilePath)
{
	HDC		hDC = GetDC(g_hWnd);

	// CreateCompatibleDC : ���� ȭ�� dc�� ȣȯ�Ǵ� dc�� ����

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL,			// ���α׷� �ν��Ͻ� �ڵ�(�츮�� ���Ϸκ��� �о���� ������ NULL���� ����)
									pFilePath,		// ������ ���
									IMAGE_BITMAP,	// � Ÿ���� �о���� ����(��Ʈ�� , Ŀ��, ������ ���)
									0,				// ����, ���� ������
									0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION); // LR_LOADFROMFILE : ���Ϸκ��� �̹����� �ҷ������� LR_CREATEDIBSECTION : �о�� ������ DIB ���·� ��ȯ

	// SelectObject : gdi ������Ʈ�� �����ϱ� ���� ������ ������ �ִ� ������Ʈ�� ��ȯ
	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CMyBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldBmp);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
