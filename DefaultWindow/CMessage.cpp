#include "stdafx.h"
#include "CMessage.h"
#include "CBmpMgr.h"

void CMessage::Initialize()
{

	// �߾� �޽��� ��¿� 
	/*
	m_vecMessage.push_back(L"���� ��ᰡ �����մϴ�."); // ������ ������ ���� ��, �������� ������ ��� 
	m_vecMessage.push_back(L"��� ���̴� �������� ��� �����Ͽ����ϴ�."); // stack ���� �������� ��� �߿� ������ ��� ������ ��� 
	m_vecMessage.push_back(L"������ �޼��Ͽ����ϴ�."); // ���� �޼� ��
	m_vecMessage.push_back(L"������ ���� �� ���� �ʾҽ��ϴ�."); // 
	m_vecMessage.push_back(L"������ ���Դϴ�. ��Ƴ�������.");
	m_vecMessage.push_back(L"������ ���߿� ��ġ�� �� �����ϴ�.");
	m_vecMessage.push_back(L"������ ������ ��ġ�� ��ġ�� �� �����ϴ�.");
	m_vecMessage.push_back(L"������ �����Ͽ����ϴ�."); // ���� Ȱ��ȭ ��
	m_vecMessage.push_back(L"������ ���� ���������ϴ�."); // ���� 2������ ��
	m_vecMessage.push_back(L"������ ��� ���� �Ҹ��Ű�� �մϴ�."); // �߾����� ���� ��
	m_vecMessage.push_back(L"�������� ������ �������"); // �߾����� �߰����� 
	*/

	// ������ ������
	m_mapItem.insert({ L"Axe",L"[����] - ������ �� �� �ֽ��ϴ�." });
	m_mapItem.insert({ L"Bomb",L"[��ź] - ��� ���� �ı��մϴ�." });
	m_mapItem.insert({ L"Bow",L"[Ȱ] - �ָ� �ִ� ���� ����ϼ���." });
	m_mapItem.insert({ L"CopperBar",L"[���� �ֱ�] - ������ ���ۿ� ����ϼ���." });
	m_mapItem.insert({ L"FireThrower",L"[�Ҳɻ�����] - ������ ���� ��ȭ�մϴ�." });
	m_mapItem.insert({ L"Furnace",L"[ȭ��] - ������ ������ �� �ֽ��ϴ�." });
	m_mapItem.insert({ L"GoldBar",L"[Ȳ�� �ֱ�] - ������ ���ۿ� ����ϼ���." });
	m_mapItem.insert({ L"Gun",L"[��ź�߻��] - ������ ���� ����ϼ���." });
	m_mapItem.insert({ L"Heal",L"[ü�� ����] - ü���� �������� �ڵ����� ���." });
	m_mapItem.insert({ L"IronBar",L"[ö �ֱ�] - ������ ���ۿ� ����ϼ���." });
	m_mapItem.insert({ L"Melee",L"[��ö��] - ��ó�� ���� �����ϰ� �о���ϴ�." });
	m_mapItem.insert({ L"Pick",L"[���] - ������ Ķ �� �ֽ��ϴ�." });
	m_mapItem.insert({ L"SilverBar",L"[�� �ֱ�] - ������ ���ۿ� ����ϼ���." });
	m_mapItem.insert({ L"WorkDesk",L"[���� �۾���] - ���� ���ۿ� ����մϴ�." });
	m_mapItem.insert({ L"WorkStation",L"[���� �۾���] - ��� ���ۿ� ����մϴ�." });
	m_mapItem.insert({ L"Coal",L"[��ź] - ���� ������ ���ۿ� ����մϴ�." });
	m_mapItem.insert({ L"Copper",L"[����] - ȭ�ο��� ���� �� ��� �����մϴ�." });
	m_mapItem.insert({ L"Dirt",L"[��] - ���࿡ ����մϴ�." });
	m_mapItem.insert({ L"Gold",L"[��] - ȭ�ο��� ���� �� ��� �����մϴ�." });
	m_mapItem.insert({ L"Iron",L"[ö] - ȭ�ο��� ���� �� ��� �����մϴ�." });
	m_mapItem.insert({ L"Rock",L"[��] - ���࿡ ����մϴ�." });
	m_mapItem.insert({ L"Sand",L"[��] - ���࿡ ����մϴ�." });
	m_mapItem.insert({ L"Silver",L"[��] - ȭ�ο��� ���� �� ��� �����մϴ�." });
	m_mapItem.insert({ L"Wooden",L"[����] - ȭ���� ����� ����մϴ�." });
	m_mapItem.insert({ L"Mine1",L"[ȭ������] - �ֺ��� ������ ȭ���� �վ���ϴ�." });
	m_mapItem.insert({ L"Mine2",L"[��������] - ���� ��Ÿ���� Ƣ��ö� �����մϴ�." });
	m_mapItem.insert({ L"SuperJump",L"[��������] - ���� �ھƿ��� �� �ֽ��ϴ�." });

	if (m_bItem)
	{
		m_tInfo.fCX = 400.f;
		m_tInfo.fCY = 20.f;
	}
	else
	{
		m_tInfo = { 400,200,500.f, 32.f };
	}
}

void CMessage::Update()
{
	if (!m_bItem)
		--m_iOpenCount;
	else
		m_bItemOpen = false;
}

OBJRESULT CMessage::Late_Update()
{
	__super::Update_Rect();

	if (0 >= m_iOpenCount)
		return OBJRESULT::DEAD;

	return OBJRESULT();
}

void CMessage::Render(HDC hDC)
{
	if (!m_bItem)
	{
		HDC hMessage = CBmpMgr::Get_Instance()->Find_Img(L"Text_Back");

		GdiTransparentBlt(hDC,
			m_tRect.left,
			m_tRect.top,
			m_tInfo.fCX,
			m_tInfo.fCY,
			hMessage,
			0,
			0,
			500,
			32,
			RGB(255, 255, 255));

		TCHAR	szCnt[32] = L"";
		SetTextColor(hDC, RGB(255, 255, 255));
		swprintf_s(szCnt, m_tMessage);
		TextOut(hDC, m_tInfo.fX, m_tInfo.fY - 8, szCnt, lstrlen(szCnt));
		SetTextColor(hDC, RGB(0, 0, 0));
	}
	
	if ((m_bItem) &&(m_bItemOpen))
	{
		HDC hMessage = CBmpMgr::Get_Instance()->Find_Img(L"Text_Back");

		GdiTransparentBlt(hDC,
			m_tInfo.fX,
			m_tInfo.fY,
			m_tInfo.fCX,
			m_tInfo.fCY,
			hMessage,
			0,
			0,
			500,
			32,
			RGB(255, 255, 255));

		TCHAR	szCnt[32] = L"";
		SetTextColor(hDC, RGB(255, 255, 255));
		SetTextAlign(hDC, TA_LEFT);
		swprintf_s(szCnt, m_tMessage);
		TextOut(hDC, m_tInfo.fX + 8, m_tInfo.fY + 2, szCnt, lstrlen(szCnt));
		SetTextColor(hDC, RGB(0, 0, 0));
		SetTextAlign(hDC, TA_CENTER);
	}
}

void CMessage::Release()
{
	m_mapItem.clear();
}

void CMessage::Collision_Rect_Obj(CObj* opponent)
{




}

void CMessage::Collision_Eillpse_Obj(CObj* opponent)
{
}

void CMessage::Render_Item(HDC hDC)
{
	
}


