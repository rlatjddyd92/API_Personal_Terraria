#include "stdafx.h"
#include "CMessage.h"
#include "CBmpMgr.h"

void CMessage::Initialize()
{

	// 중앙 메시지 출력용 
	/*
	m_vecMessage.push_back(L"제작 재료가 부족합니다."); // 가구로 아이템 제작 시, 아이템이 부족할 경우 
	m_vecMessage.push_back(L"사용 중이던 아이템을 모두 소진하였습니다."); // stack 가능 아이템을 사용 중에 수량이 모두 소진될 경우 
	m_vecMessage.push_back(L"업적을 달성하였습니다."); // 업적 달성 시
	m_vecMessage.push_back(L"생존의 밤이 얼마 남지 않았습니다."); // 
	m_vecMessage.push_back(L"생존의 밤입니다. 살아남으세요.");
	m_vecMessage.push_back(L"가구는 공중에 배치할 수 없습니다.");
	m_vecMessage.push_back(L"가구는 지형과 겹치게 배치할 수 없습니다.");
	m_vecMessage.push_back(L"보스가 등장하였습니다."); // 보스 활성화 시
	m_vecMessage.push_back(L"보스가 더욱 강력해집니다."); // 보스 2페이즈 시
	m_vecMessage.push_back(L"보스가 모든 것을 소멸시키려 합니다."); // 발악패턴 시작 시
	m_vecMessage.push_back(L"범위에서 빠르게 벗어나세요"); // 발악패턴 중간지점 
	*/

	// 아이템 툴팁용
	m_mapItem.insert({ L"Axe",L"[도끼] - 나무를 벨 수 있습니다." });
	m_mapItem.insert({ L"Bomb",L"[폭탄] - 모든 것을 파괴합니다." });
	m_mapItem.insert({ L"Bow",L"[활] - 멀리 있는 적을 상대하세요." });
	m_mapItem.insert({ L"CopperBar",L"[구리 주괴] - 아이템 제작에 사용하세요." });
	m_mapItem.insert({ L"FireThrower",L"[불꽃샤워기] - 더러운 것을 정화합니다." });
	m_mapItem.insert({ L"Furnace",L"[화로] - 광물을 가공할 수 있습니다." });
	m_mapItem.insert({ L"GoldBar",L"[황금 주괴] - 아이템 제작에 사용하세요." });
	m_mapItem.insert({ L"Gun",L"[유탄발사기] - 강력한 적을 상대하세요." });
	m_mapItem.insert({ L"Heal",L"[체력 포션] - 체력이 떨어지면 자동으로 사용." });
	m_mapItem.insert({ L"IronBar",L"[철 주괴] - 아이템 제작에 사용하세요." });
	m_mapItem.insert({ L"Melee",L"[강철검] - 근처의 적을 공격하고 밀어냅니다." });
	m_mapItem.insert({ L"Pick",L"[곡괭이] - 광물을 캘 수 있습니다." });
	m_mapItem.insert({ L"SilverBar",L"[은 주괴] - 아이템 제작에 사용하세요." });
	m_mapItem.insert({ L"WorkDesk",L"[소형 작업대] - 가구 제작에 사용합니다." });
	m_mapItem.insert({ L"WorkStation",L"[대형 작업대] - 장비 제작에 사용합니다." });
	m_mapItem.insert({ L"Coal",L"[석탄] - 폭발 아이템 제작에 사용합니다." });
	m_mapItem.insert({ L"Copper",L"[구리] - 화로에서 가공 후 사용 가능합니다." });
	m_mapItem.insert({ L"Dirt",L"[흙] - 건축에 사용합니다." });
	m_mapItem.insert({ L"Gold",L"[금] - 화로에서 가공 후 사용 가능합니다." });
	m_mapItem.insert({ L"Iron",L"[철] - 화로에서 가공 후 사용 가능합니다." });
	m_mapItem.insert({ L"Rock",L"[돌] - 건축에 사용합니다." });
	m_mapItem.insert({ L"Sand",L"[모래] - 건축에 사용합니다." });
	m_mapItem.insert({ L"Silver",L"[은] - 화로에서 가공 후 사용 가능합니다." });
	m_mapItem.insert({ L"Wooden",L"[목재] - 화로의 연료로 사용합니다." });
	m_mapItem.insert({ L"Mine1",L"[화염지뢰] - 주변의 적에게 화염을 뿜어냅니다." });
	m_mapItem.insert({ L"Mine2",L"[도약지뢰] - 적이 나타나면 튀어올라 폭발합니다." });
	m_mapItem.insert({ L"SuperJump",L"[점프슈즈] - 높게 솟아오를 수 있습니다." });

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


