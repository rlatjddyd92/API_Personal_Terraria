#include "stdafx.h"
#include "CMainMenu.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "Obj.h"
#include "CAbstractFactory.h"
#include "CButton.h"
#include "CMouse.h"

void CMainMenu::Initialize()
{
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CButton>().Create(400, 200));
	dynamic_cast<CButton*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::UI))->Set_Function(BUTTONFUNCTION::INGAME);
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CButton>().Create(400, 300));
	dynamic_cast<CButton*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::UI))->Set_Function(BUTTONFUNCTION::EDIT);
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CButton>().Create(400, 400));
	dynamic_cast<CButton*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::UI))->Set_Function(BUTTONFUNCTION::EXIT);

	CScrollMgr::Get_Instance()->Reset_Scroll();
	CObjMgr::Get_Instance()->Add_Object(OBJID::MOUSE, CAbstractFactory<CMouse>().Create());
}

void CMainMenu::Update()
{
	CObjMgr::Get_Instance()->Update(OBJID::UI);
	CObjMgr::Get_Instance()->Update(OBJID::MOUSE);
}

void CMainMenu::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update(OBJID::UI);
	CObjMgr::Get_Instance()->Late_Update(OBJID::MOUSE);
}

void CMainMenu::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"InGameMem");
	HDC hBackDC = CBmpMgr::Get_Instance()->Find_Img(L"InGame");
	HDC hTitleDC = CBmpMgr::Get_Instance()->Find_Img(L"Logo3");
	BitBlt(hMemDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);
	GdiTransparentBlt(hMemDC, 279, 50, 243, 72, hTitleDC, 0, 0, 486, 142, RGB(255, 255, 255));
	CObjMgr::Get_Instance()->Render(hMemDC, OBJID::UI);
	CObjMgr::Get_Instance()->Render(hMemDC, OBJID::MOUSE);
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CMainMenu::Release()
{
	
}

void CMainMenu::Key_Input()
{

}
