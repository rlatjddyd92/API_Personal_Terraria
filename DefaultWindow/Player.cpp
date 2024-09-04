#include "stdafx.h"
#include "Player.h"
#include "CKeyMgr.h"
#include "CTileMgr.h"
#include "CAbstractFactory.h"
#include "CScrollMgr.h"
#include "CCollisionMgr.h"
#include "CBmpMgr.h"
#include "CInvenMgr.h"
#include "CObjMgr.h"
#include "CMelee.h"
#include "CMissile.h"
#include "CItem.h"
#include "CBomb.h"
#include "CStats.h"
#include "CExplosion.h"
#include "CFire.h"
#include "CSceneMgr.h"
#include "CSoundMgr.h"
#include "CFunitureMgr.h"

float		g_fVolume(1.f);

CPlayer::CPlayer() : m_tBeforeInfo({0,0,0,0}), m_bUsingInven(false), m_iSelectQuick(0), m_bAutoHeal(false), m_iHealCoolTime(0)
{
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo = { WINCX / 2.f, WINCY / 2.f, 24.f, 40.f };
	m_fSpeed = 2.f;
	m_fJumpPower = 10.f;

	m_vecSpec[int(OBJSPEC::HEALTH)] = 1000;				m_vecSpecLimit[int(OBJSPEC::HEALTH)] = 1000;
	m_vecSpec[int(OBJSPEC::ATTACK)] = 10;				m_vecSpecLimit[int(OBJSPEC::ATTACK)] = 10;
	m_vecSpec[int(OBJSPEC::FUEL)] = 1000;				m_vecSpecLimit[int(OBJSPEC::FUEL)] = 1000;
	

	m_bPlayer = true;
	m_ObjId = OBJID::PLAYER;

	m_hPlayer_IDEL = CBmpMgr::Get_Instance()->Find_Img(L"Player_Idle_L");
	m_hPlayer_USING = CBmpMgr::Get_Instance()->Find_Img(L"Player_Using_L");
	m_hPlayer_Action = nullptr;

	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CStats>().Create_Stats(STATS::HEALTH_PLAYER, this));
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CStats>().Create_Stats(STATS::FLYGAGE, this));

	CInvenMgr::Get_Instance()->Initialize();
	CFunitureMgr::Get_Instance()->Initialize();
}

void CPlayer::Update()
{
	if (0 < m_iSoundCount)
		--m_iSoundCount;

	m_tBeforeInfo.fX = m_tInfo.fX;
	m_tBeforeInfo.fY = m_tInfo.fY;

	if (m_bAutoHeal)
		if (0 == m_iHealCoolTime)
			if (900 > m_vecSpec[int(OBJSPEC::HEALTH)])
			{
				m_vecSpec[int(OBJSPEC::HEALTH)] += 100;
				CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CNumber>().Create_Number(m_tInfo.fX, m_tInfo.fY, 100));
				m_iHealCoolTime = HEALCOOL;
			}

	if (0 != m_iHealCoolTime)
		--m_iHealCoolTime;

	if (!CTileMgr::Get_Instance()->IsGround(this))
	{
		if (!m_vecSpecial[int(OBJSPECIAL::SUPERJUMP)])
			m_vVelocity.fY += m_fGravity;
		else 
			m_vVelocity.fY += m_fGravity*0.5f;
	}
	else
		m_vVelocity.fY = 0;

	Key_Input();

	m_tInfo.fX += m_vVelocity.fX;
	m_tInfo.fY += m_vVelocity.fY;

	if (0 < m_iAttackNum)
	{
		++m_iAttackNum;

		if (30 <= m_iAttackNum)
			m_iAttackNum = 0;
	}

	CInvenMgr::Get_Instance()->Update();

	m_pUsingItem = CInvenMgr::Get_Instance()->Select_Quick();
}

OBJRESULT CPlayer::Late_Update()
{
	m_tInfo = CCollisionMgr::Get_Instance()->Collision_MapBorder(this);
	m_tInfo = CTileMgr::Get_Instance()->Coillsion_Tile(this);

	if (-1 == m_tInfo.fX)
	{
		m_tInfo.fX = m_tBeforeInfo.fX;
  		m_tInfo.fY = m_tBeforeInfo.fY;
	}

	Offset();

	__super::Update_Rect();

	CInvenMgr::Get_Instance()->Late_Update();

	return OBJRESULT::NONE;
}

void CPlayer::Render(HDC hDC)
{
	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
		
	if (0 > m_vVelocity.fX)
		m_hPlayer_IDEL = CBmpMgr::Get_Instance()->Find_Img(L"Player_Idle_L");

	if (0 < m_vVelocity.fX)
		m_hPlayer_IDEL = CBmpMgr::Get_Instance()->Find_Img(L"Player_Idle_R");

	if ((ACTION::WALKING == m_nowAction) && (0 > m_vVelocity.fX))
	{
		if (0 == (m_iActionNum/10) % 2)
			m_hPlayer_Action = CBmpMgr::Get_Instance()->Find_Img(L"Player_Walk_L");

		if (1 == (m_iActionNum / 10) % 2)
			m_hPlayer_Action = CBmpMgr::Get_Instance()->Find_Img(L"Player_Idle_L");
	}

	if ((ACTION::WALKING == m_nowAction) && (0 < m_vVelocity.fX))
	{
		if (0 == (m_iActionNum / 10) % 2)
			m_hPlayer_Action = CBmpMgr::Get_Instance()->Find_Img(L"Player_Walk_R");

		if (1 == (m_iActionNum / 10) % 2)
			m_hPlayer_Action = CBmpMgr::Get_Instance()->Find_Img(L"Player_Idle_R");
	}

	if ((ACTION::USING == m_nowAction) && (0 > m_vVelocity.fX))
		m_hPlayer_USING = CBmpMgr::Get_Instance()->Find_Img(L"Player_Using_L");

	if ((ACTION::USING == m_nowAction) && (0 < m_vVelocity.fX))
		m_hPlayer_USING = CBmpMgr::Get_Instance()->Find_Img(L"Player_Using_R");

	if (ACTION::IDLE == m_nowAction)
		GdiTransparentBlt(hDC,
			m_tRect.left + ScrollX,
			m_tRect.top + ScrollY,
			m_tInfo.fCX,
			m_tInfo.fCY + 3,
			m_hPlayer_IDEL,
			0,
			0,
			21,
			40,
			RGB(255, 255, 255));
	else if (ACTION::WALKING == m_nowAction)
		GdiTransparentBlt(hDC,
			m_tRect.left + ScrollX,
			m_tRect.top + ScrollY,
			m_tInfo.fCX,
			m_tInfo.fCY + 3,
			m_hPlayer_Action,
			0,
			0,
			24 - (((m_iActionNum / 10) % 2)*3),
			38,
			RGB(255, 255, 255));
	else if (ACTION::USING == m_nowAction)
		GdiTransparentBlt(hDC,
			m_tRect.left + ScrollX,
			m_tRect.top + ScrollY,
			m_tInfo.fCX,
			m_tInfo.fCY + 3,
			m_hPlayer_USING,
			0,
			0,
			23,
			38,
			RGB(255, 255, 255));

	CInvenMgr::Get_Instance()->Render_Quick(hDC);

	if (true == m_bUsingInven)
	{
		CInvenMgr::Get_Instance()->Render_Inven(hDC);
		CInvenMgr::Get_Instance()->Render_Equip(hDC);
	}
		

	if (m_pUsingItem)
		if ((ITEMTYPE::PICK == dynamic_cast<CItem*>(m_pUsingItem)->Get_ItemType()) || (ITEMTYPE::TILE == dynamic_cast<CItem*>(m_pUsingItem)->Get_ItemType()))
	{
		HPEN hNewPen = CreatePen(PS_SOLID, 1, RGB(255, 153, 0));
		HPEN hOldPen = (HPEN)SelectObject(hDC, hNewPen);

		HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);

		Rectangle(hDC, m_tInfo.fX - PICKLIMIT + ScrollX, m_tInfo.fY - PICKLIMIT + ScrollY, m_tInfo.fX + PICKLIMIT + ScrollX, m_tInfo.fY + PICKLIMIT + ScrollY);

		SelectObject(hDC, hOldPen);
		DeleteObject(hNewPen);

		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);
	}

	if (m_pUsingItem)
	{
		HDC hPick = nullptr;

		POINT	pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
		float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

		pt.x -= ScrollX;
		pt.y -= ScrollY;

		Vector2D vTemp = { 0,0 };

		vTemp.fX = pt.x - m_tInfo.fX;
		vTemp.fY = pt.y - m_tInfo.fY;

		vTemp = vTemp.Normalliztion(vTemp);

		if ((ITEMTYPE::PICK == dynamic_cast<CItem*>(m_pUsingItem)->Get_ItemType()))
		{
			if (0 <= vTemp.fX)
			{
				if (!(CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON)))
				{
					hPick = CBmpMgr::Get_Instance()->Find_Img(L"Pick_1");
				}
				else
				{
					if (0 == (GetTickCount() / 100) % 2)
						hPick = CBmpMgr::Get_Instance()->Find_Img(L"Pick_1");
					else
						hPick = CBmpMgr::Get_Instance()->Find_Img(L"Pick_2");
				}
			}
			else
			{
				if (!(CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON)))
				{
					hPick = CBmpMgr::Get_Instance()->Find_Img(L"Pick_1_L");
				}
				else
				{
					if (0 == (GetTickCount() / 100) % 2)
						hPick = CBmpMgr::Get_Instance()->Find_Img(L"Pick_1_L");
					else
						hPick = CBmpMgr::Get_Instance()->Find_Img(L"Pick_2_L");
				}
			}
			
		}
		else if ((ITEMTYPE::AXE == dynamic_cast<CItem*>(m_pUsingItem)->Get_ItemType()))
		{
			if (0 <= vTemp.fX)
			{
				if (!(CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON)))
				{
					hPick = CBmpMgr::Get_Instance()->Find_Img(L"Axe_1");
				}
				else
				{
					if (0 == (GetTickCount() / 100) % 2)
						hPick = CBmpMgr::Get_Instance()->Find_Img(L"Axe_1");
					else
						hPick = CBmpMgr::Get_Instance()->Find_Img(L"Axe_2");
				}
			}
			else
			{
				if (!(CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON)))
				{
					hPick = CBmpMgr::Get_Instance()->Find_Img(L"Axe_1_L");
				}
				else
				{
					if (0 == (GetTickCount() / 100) % 2)
						hPick = CBmpMgr::Get_Instance()->Find_Img(L"Axe_1_L");
					else
						hPick = CBmpMgr::Get_Instance()->Find_Img(L"Axe_2_L");
				}
			}
		}
		else if ((ITEMTYPE::FIRETHROWER == dynamic_cast<CItem*>(m_pUsingItem)->Get_ItemType()))
		{
			if ((0 <= vTemp.fX)&&(0 <= vTemp.fY)&&(abs(vTemp.fX) >= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"FireThrower_1");
			else if ((0 <= vTemp.fX) && (0 <= vTemp.fY) && (abs(vTemp.fX) <= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"FireThrower_2");
			else if ((0 >= vTemp.fX) && (0 <= vTemp.fY) && (abs(vTemp.fX) <= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"FireThrower_3");
			else if ((0 >= vTemp.fX) && (0 <= vTemp.fY) && (abs(vTemp.fX) >= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"FireThrower_4");
			else if ((0 >= vTemp.fX) && (0 >= vTemp.fY) && (abs(vTemp.fX) >= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"FireThrower_5");
			else if ((0 >= vTemp.fX) && (0 >= vTemp.fY) && (abs(vTemp.fX) <= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"FireThrower_6");
			else if ((0 <= vTemp.fX) && (0 >= vTemp.fY) && (abs(vTemp.fX) <= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"FireThrower_7");
			else if ((0 <= vTemp.fX) && (0 >= vTemp.fY) && (abs(vTemp.fX) >= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"FireThrower_8");
		}
		else if ((ITEMTYPE::GUN == dynamic_cast<CItem*>(m_pUsingItem)->Get_ItemType()))
		{
			if ((0 <= vTemp.fX) && (0 <= vTemp.fY) && (abs(vTemp.fX) >= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"Gun_Using_1");
			else if ((0 <= vTemp.fX) && (0 <= vTemp.fY) && (abs(vTemp.fX) <= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"Gun_Using_2");
			else if ((0 >= vTemp.fX) && (0 <= vTemp.fY) && (abs(vTemp.fX) <= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"Gun_Using_3");
			else if ((0 >= vTemp.fX) && (0 <= vTemp.fY) && (abs(vTemp.fX) >= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"Gun_Using_4");
			else if ((0 >= vTemp.fX) && (0 >= vTemp.fY) && (abs(vTemp.fX) >= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"Gun_Using_5");
			else if ((0 >= vTemp.fX) && (0 >= vTemp.fY) && (abs(vTemp.fX) <= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"Gun_Using_6");
			else if ((0 <= vTemp.fX) && (0 >= vTemp.fY) && (abs(vTemp.fX) <= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"Gun_Using_7");
			else if ((0 <= vTemp.fX) && (0 >= vTemp.fY) && (abs(vTemp.fX) >= abs(vTemp.fY)))
				hPick = CBmpMgr::Get_Instance()->Find_Img(L"Gun_Using_8");
		}

		if ((ITEMTYPE::PICK == dynamic_cast<CItem*>(m_pUsingItem)->Get_ItemType()) || ((ITEMTYPE::AXE == dynamic_cast<CItem*>(m_pUsingItem)->Get_ItemType())))
			GdiTransparentBlt(hDC,
				m_tInfo.fX - 35 + ScrollX,
				m_tInfo.fY - 35 + ScrollY,
				70,
				70,
				hPick,
				0,
				0,
				70,
				70,
				RGB(255, 255, 255));
		else 
			GdiTransparentBlt(hDC,
				m_tInfo.fX - 30 + ScrollX,
				m_tInfo.fY - 30 + ScrollY,
				60,
				60,
				hPick,
				0,
				0,
				70,
				70,
				RGB(255, 255, 255));
	}

}

void CPlayer::Release()
{
	m_pPickedItem = nullptr;
	m_pUsingItem = nullptr;
}

void CPlayer::Collision_Rect_Obj(CObj* opponent)
{
	if ((!opponent->Get_Player()) && (OBJID::MELEE == opponent->Get_OBJID()))
	{
		Add_Spec(OBJSPEC::HEALTH, -(opponent->Get_Spec(OBJSPEC::ATTACK)));
		CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CNumber>().Create_Number(m_tInfo.fX, m_tInfo.fY - 10, -(opponent->Get_Spec(OBJSPEC::ATTACK))));

		if (DIRECTION::EAST == dynamic_cast<CMelee*>(opponent)->Get_AttackDirection())
			m_vVelocity.fX += 10;
		else
			m_vVelocity.fX -= 10;
	}

	if ((!opponent->Get_Player()) && (OBJID::MISSILE == opponent->Get_OBJID()))
	{
		Add_Spec(OBJSPEC::HEALTH, -(opponent->Get_Spec(OBJSPEC::ATTACK)));
		CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CNumber>().Create_Number(m_tInfo.fX, m_tInfo.fY - 10, -(opponent->Get_Spec(OBJSPEC::ATTACK))));
	}
}

void CPlayer::Collision_Eillpse_Obj(CObj* opponent)
{
	if (OBJID::EXPLOSION == opponent->Get_OBJID())
		if (dynamic_cast<CExplosion*>(opponent)->Get_Nuke())
			if (opponent->Get_Info().fCX >= NUKE_CX - 10)
				m_vecSpec[int(OBJSPEC::HEALTH)] = 0;

	if ((!opponent->Get_Player()) && (OBJID::EXPLOSION == opponent->Get_OBJID()))
	{

		Add_Spec(OBJSPEC::HEALTH, -(opponent->Get_Spec(OBJSPEC::ATTACK)));

		int iTemp = GetTickCount();
	}
}

void CPlayer::Get_EquipItemSpec()
{
	m_vecSpecial[int(OBJSPECIAL::SUPERJUMP)] = false;

	for (int i = 0; i < INVENE; ++i)
	{
		CObj* pTemp = CInvenMgr::Get_Instance()->Get_Equip_Item_Spec(i);

		if (!pTemp)
			continue;

		if (pTemp->Get_Special(OBJSPECIAL::SUPERJUMP))
			m_vecSpecial[int(OBJSPECIAL::SUPERJUMP)] = true;
	}
}

void CPlayer::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
		m_vVelocity.fX = m_fSpeed, m_nowAction = ACTION::WALKING, ++m_iActionNum;
	else if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
		m_vVelocity.fX = -m_fSpeed, m_nowAction = ACTION::WALKING, ++m_iActionNum;
	else
	{
		if (0 < m_vVelocity.fX)
			m_vVelocity.fX -= m_fSpeed;
		else if (0 > m_vVelocity.fX)
			m_vVelocity.fX += m_fSpeed;

		m_nowAction = ACTION::IDLE;
		m_iActionNum = 0;
	}

	if (m_vecSpecial[int(OBJSPECIAL::SUPERJUMP)])
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
		{
			if (0 < m_vecSpec[int(OBJSPEC::FUEL)])
			{
				m_vVelocity.fY -= 0.6f;
				m_vecSpec[int(OBJSPEC::FUEL)] -= 1;
				m_vVelocity.fX = 0;
			}
		}
		else
			m_vecSpec[int(OBJSPEC::FUEL)] += 1;

		if (m_vecSpec[int(OBJSPEC::FUEL)] > m_vecSpecLimit[int(OBJSPEC::FUEL)])
			m_vecSpec[int(OBJSPEC::FUEL)] = m_vecSpecLimit[int(OBJSPEC::FUEL)];

		if (m_vecSpec[int(OBJSPEC::FUEL)] < 0)
			m_vecSpec[int(OBJSPEC::FUEL)] = 0;
	}
	else
	{
		if ((CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE)) && (CTileMgr::Get_Instance()->IsGround(this)))
			m_vVelocity.fY -= m_fJumpPower, m_nowAction = ACTION::JUMPING;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('I'))
	{
		if (!m_bUsingInven) { m_bUsingInven = true; }
		else { m_bUsingInven = false; }
	}
		
	Key_Num();

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT	pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
		float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

		if (m_pPickedItem)
		{
			CObj* pTemp = CInvenMgr::Get_Instance()->Put_In_Item_By_Mouse(pt, m_pPickedItem, m_bUsingInven);

			if (pTemp == m_pPickedItem) // <- 인벤이 아닌 곳을 터치 
			{
				dynamic_cast<CItem*>(m_pPickedItem)->Set_ItemMode(ITEMMODE::GROUND);
				Throw_Item(pt);
				m_pPickedItem = nullptr;
				return;
			}
			
			m_pPickedItem = pTemp;
			return;
		}

		CObj* pTemp = CInvenMgr::Get_Instance()->Pick_Up_Item_by_Mouse(pt, m_bUsingInven);

		if (pTemp)
		{
			m_pPickedItem = pTemp;
			return;
		}

		if (!m_pUsingItem) // 가구 메뉴 이용 
		{
			CObj* pTemp = CFunitureMgr::Get_Instance()->ExchangeItem(pt);

			if (pTemp)
				m_pPickedItem = pTemp;

			return;
		}
			

		switch (dynamic_cast<CItem*>(m_pUsingItem)->Get_ItemType())
		{
		case ITEMTYPE::MELEE:
			Attack_Melee(pt);
			break;
		case ITEMTYPE::GUN:
			Attack_Missile(pt, false);
			break;
		case ITEMTYPE::BOW:
			Attack_Missile(pt, true);
			break;
		case ITEMTYPE::THROW:
			Throw_Bomb(pt);
			break;
		case ITEMTYPE::TILE:
			Make_Tile();
			break;
		case ITEMTYPE::FUNITURE:
			if (CFunitureMgr::Get_Instance()->SetFuniture(pt, m_pUsingItem)) // <- 가구 배치 
			{
				// 배치 성공 시
				CInvenMgr::Get_Instance()->Set_Funiture(m_iSelectQuick); // <- 인벤에서 가구 제거 

				if (0 == m_iSoundCount)
				{
					CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_PLAYER);
					CSoundMgr::Get_Instance()->PlaySound(L"Dig.wav", CHANNELID::SOUND_PLAYER, g_fVolume);
					m_iSoundCount = 50;
				}
			}
			break;
		default:
			break;
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
	{
		POINT	pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		CFunitureMgr::Get_Instance()->UsingFuniture(pt);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('P'))
	{
		CSceneMgr::Get_Instance()->Change_InGame_BackGround(BACKGROUND::SURVIVAL);
		CObjMgr::Get_Instance()->Add_Object(OBJID::MESSAGE, CAbstractFactory<CMessage>().Create_Message(L"◈생존의 밤◈ - ★화★염★파★티★를 즐기세요!!"));
		dynamic_cast<CMessage*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::MESSAGE))->Set_OpenCount(500);
	}

	Pick_item();
	Throw_Fire();

	
}

void CPlayer::Key_Num()
{
	if (CKeyMgr::Get_Instance()->Key_Down('1')) { m_pUsingItem = CInvenMgr::Get_Instance()->Select_Quick(0); m_iSelectQuick = 0;}
	if (CKeyMgr::Get_Instance()->Key_Down('2')) { m_pUsingItem = CInvenMgr::Get_Instance()->Select_Quick(1); m_iSelectQuick = 1;}
	if (CKeyMgr::Get_Instance()->Key_Down('3')) { m_pUsingItem = CInvenMgr::Get_Instance()->Select_Quick(2); m_iSelectQuick = 2;}
	if (CKeyMgr::Get_Instance()->Key_Down('4')) { m_pUsingItem = CInvenMgr::Get_Instance()->Select_Quick(3); m_iSelectQuick = 3;}
	if (CKeyMgr::Get_Instance()->Key_Down('5')) { m_pUsingItem = CInvenMgr::Get_Instance()->Select_Quick(4); m_iSelectQuick = 4;}
	if (CKeyMgr::Get_Instance()->Key_Down('6')) { m_pUsingItem = CInvenMgr::Get_Instance()->Select_Quick(5); m_iSelectQuick = 5;}
	if (CKeyMgr::Get_Instance()->Key_Down('7')) { m_pUsingItem = CInvenMgr::Get_Instance()->Select_Quick(6); m_iSelectQuick = 6;}
	if (CKeyMgr::Get_Instance()->Key_Down('8')) { m_pUsingItem = CInvenMgr::Get_Instance()->Select_Quick(7); m_iSelectQuick = 7;}
	if (CKeyMgr::Get_Instance()->Key_Down('9')) { m_pUsingItem = CInvenMgr::Get_Instance()->Select_Quick(8); m_iSelectQuick = 8;}
	if (CKeyMgr::Get_Instance()->Key_Down('0')) { m_pUsingItem = CInvenMgr::Get_Instance()->Select_Quick(9); m_iSelectQuick = 9;}
}

void CPlayer::Pick_item()
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if ((CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON)) && (dynamic_cast<CItem*>(m_pUsingItem)->Get_ItemType() == ITEMTYPE::PICK))
		CTileMgr::Get_Instance()->Cancel_Pick();

	if ((CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON)) && (dynamic_cast<CItem*>(m_pUsingItem)->Get_ItemType() == ITEMTYPE::AXE))
		CTileMgr::Get_Instance()->Cancel_Pick();

	if ((abs((pt.x - ScrollX) - m_tInfo.fX) > PICKLIMIT) || (abs((pt.y - ScrollY) - m_tInfo.fY) > PICKLIMIT))
	{
		CTileMgr::Get_Instance()->Cancel_Pick();
		return;
	}

	if (!m_pUsingItem)
		return;

	if ((CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON)) && (dynamic_cast<CItem*>(m_pUsingItem)->Get_ItemType() == ITEMTYPE::PICK))
	{
		if (0 == m_iSoundCount)
		{
			CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Dig.wav", CHANNELID::SOUND_PLAYER, g_fVolume);
			m_iSoundCount = 50;
		}
		CTileMgr::Get_Instance()->Pick_Tile(pt);
	}
		

	if ((CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON)) && (dynamic_cast<CItem*>(m_pUsingItem)->Get_ItemType() == ITEMTYPE::AXE))
	{
		if (0 == m_iSoundCount)
		{
			CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Dig.wav", CHANNELID::SOUND_PLAYER, g_fVolume);
			m_iSoundCount = 50;
		}
		CTileMgr::Get_Instance()->Pick_Tile(pt);
	}


}

void CPlayer::Make_Tile()
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if ((abs((pt.x - ScrollX) - m_tInfo.fX) > PICKLIMIT) || (abs((pt.y - ScrollY) - m_tInfo.fY) > PICKLIMIT))
		return;


	if (CTileMgr::Get_Instance()->MakeTile(dynamic_cast<CItem*>(m_pUsingItem)->Get_ItemKey(), pt))
	{
		if (0 == m_iSoundCount)
		{
			CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Dig.wav", CHANNELID::SOUND_PLAYER, g_fVolume);
			m_iSoundCount = 50;
		}
		CInvenMgr::Get_Instance()->Using_Stack_Item(1);
	}
		
}

void CPlayer::Offset()
{
	int	iOffsetminX = 200;
	int	iOffsetmaxX = 600;
	int iOffsetminY = 200;
	int iOffsetmaxY = 400;

	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if ((iOffsetminX > m_tInfo.fX + iScrollX) && (200<m_tInfo.fX))
		CScrollMgr::Get_Instance()->Set_ScrollX(-(m_tInfo.fX - m_tBeforeInfo.fX));

	if ((iOffsetmaxX < m_tInfo.fX + iScrollX) && (MAPCOL * TILECX - 200 > m_tInfo.fX))
		CScrollMgr::Get_Instance()->Set_ScrollX(-(m_tInfo.fX - m_tBeforeInfo.fX));

	if ((iOffsetminY > m_tInfo.fY + iScrollY) && (200 < m_tInfo.fY))
		CScrollMgr::Get_Instance()->Set_ScrollY(-(m_tInfo.fY - m_tBeforeInfo.fY));

	if ((iOffsetmaxY < m_tInfo.fY + iScrollY) && (MAPROW * TILECY - 200 > m_tInfo.fY))
		CScrollMgr::Get_Instance()->Set_ScrollY(-(m_tInfo.fY - m_tBeforeInfo.fY));
}

void CPlayer::Offset_Special(INFO Input)
{
	int	iOffsetminX = 300;
	int	iOffsetmaxX = 500;
	int iOffsetminY = 250;
	int iOffsetmaxY = 350;

	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if ((iOffsetminX > m_tInfo.fX + iScrollX) && (200 < m_tInfo.fX))
		CScrollMgr::Get_Instance()->Set_ScrollX(-(m_tInfo.fX - Input.fX));

	if ((iOffsetmaxX < m_tInfo.fX + iScrollX) && (MAPCOL * TILECX - 200 > m_tInfo.fX))
		CScrollMgr::Get_Instance()->Set_ScrollX(-(m_tInfo.fX - Input.fX));

	if ((iOffsetminY > m_tInfo.fY + iScrollY) && (200 < m_tInfo.fY))
		CScrollMgr::Get_Instance()->Set_ScrollY(-(m_tInfo.fY - Input.fY));

	if ((iOffsetmaxY < m_tInfo.fY + iScrollY) && (MAPROW * TILECY - 200 > m_tInfo.fY))
		CScrollMgr::Get_Instance()->Set_ScrollY(-(m_tInfo.fY - Input.fY));
}

void CPlayer::Attack_Melee(POINT pt)
{
	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	pt.x -= ScrollX;
	pt.y -= ScrollY;

	if (m_tInfo.fX >= pt.x)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJID::MELEE, CAbstractFactory<CMelee>::Create(m_tInfo.fX - m_tInfo.fCX * 0.5f, m_tInfo.fY));
		CObjMgr::Get_Instance()->Get_LastObj(OBJID::MELEE)->Set_Player(true);
		dynamic_cast<CMelee*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::MELEE))->Set_AttackDirection(DIRECTION::WEST);
	}
	else
	{
		CObjMgr::Get_Instance()->Add_Object(OBJID::MELEE, CAbstractFactory<CMelee>::Create(m_tInfo.fX + m_tInfo.fCX * 0.5f, m_tInfo.fY));
		CObjMgr::Get_Instance()->Get_LastObj(OBJID::MELEE)->Set_Player(true);
		dynamic_cast<CMelee*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::MELEE))->Set_AttackDirection(DIRECTION::EAST);
	}
}

void CPlayer::Attack_Missile(POINT pt, bool bArrow)
{
	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	pt.x -= ScrollX;
	pt.y -= ScrollY;

	Vector2D vTemp = { 0,0 };

	vTemp.fX = pt.x - m_tInfo.fX;
	vTemp.fY = pt.y - m_tInfo.fY;

	vTemp = vTemp.Normalliztion(vTemp);

	CObjMgr::Get_Instance()->Add_Object(OBJID::MISSILE, CAbstractFactory<CMissile>::Create(m_tInfo.fX, m_tInfo.fY));
	CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE)->Set_Player(true);
	CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE)->Set_Vecter2D(vTemp.fX, vTemp.fY);
	dynamic_cast<CMissile*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE))->Set_Explosion(true, 30);
	if (bArrow)
		dynamic_cast<CMissile*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::MISSILE))->Set_Arrow(true);
}

void CPlayer::Throw_Bomb(POINT pt)
{
	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	pt.x -= ScrollX;
	pt.y -= ScrollY;

	Vector2D vTemp = { 0,0 };

	vTemp.fX = pt.x - m_tInfo.fX;
	vTemp.fY = pt.y - m_tInfo.fY;

	vTemp = vTemp.Normalliztion(vTemp);

	CObjMgr::Get_Instance()->Add_Object(OBJID::BOMB, CAbstractFactory<CBomb>::Create(m_tInfo.fX, m_tInfo.fY));
	CObjMgr::Get_Instance()->Get_LastObj(OBJID::BOMB)->Set_Player(true);
	CObjMgr::Get_Instance()->Get_LastObj(OBJID::BOMB)->Set_Vecter2D(vTemp.fX, vTemp.fY);

	CItem* pSelect = dynamic_cast<CItem*>(m_pUsingItem);
	CBomb* pTemp = dynamic_cast<CBomb*>(CObjMgr::Get_Instance()->Get_LastObj(OBJID::BOMB));

	if (pSelect->Get_FireSpray())
	{
		pTemp->Set_Mine();
		pTemp->Set_FireSpray();
	}
	else if (pSelect->Get_High())
	{
		pTemp->Set_Mine();
		pTemp->Set_High();
	}
	else
	{
		pTemp->Set_ExplosionCount(100);
		pTemp->Set_Fire();
	}

	CInvenMgr::Get_Instance()->Using_Stack_Item(1);
}

void CPlayer::Throw_Item(POINT pt)
{
	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	pt.x -= ScrollX;
	pt.y -= ScrollY;

	Vector2D vTemp = { 0,0 };

	vTemp.fX = pt.x - m_tInfo.fX;
	vTemp.fY = pt.y - m_tInfo.fY;

	vTemp = vTemp.Normalliztion(vTemp);
	m_pPickedItem->Set_Pos(m_tInfo.fX, m_tInfo.fY);
	m_pPickedItem->Set_Vecter2D(vTemp.fX * 10, vTemp.fY * 10);
}

void CPlayer::Throw_Fire()
{
	if (!m_pUsingItem)
		return;

	if (0 != m_iAttackNum % 2)
		return;

	if ((CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON)) && (dynamic_cast<CItem*>(m_pUsingItem)->Get_ItemType() == ITEMTYPE::FIRETHROWER))
	{
		if (0 == m_iSoundCount)
		{
			CSoundMgr::Get_Instance()->StopSound(CHANNELID::SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Player_Fire_Thrower.wav", CHANNELID::SOUND_PLAYER, g_fVolume);
			m_iSoundCount = 130;
		}

		POINT	pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
		float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

		pt.x -= ScrollX;
		pt.y -= ScrollY;

		Vector2D vTemp = { 0,0 };

		vTemp.fX = pt.x - m_tInfo.fX;
		vTemp.fY = pt.y - m_tInfo.fY;

		vTemp = vTemp.Normalliztion(vTemp);

		CObjMgr::Get_Instance()->Add_Object(OBJID::FIRE, CAbstractFactory<CFire>::Create(m_tInfo.fX, m_tInfo.fY));
		CObjMgr::Get_Instance()->Get_LastObj(OBJID::FIRE)->Set_Player(true);
		CObjMgr::Get_Instance()->Get_LastObj(OBJID::FIRE)->Set_Vecter2D(vTemp.fX, vTemp.fY);
		CObjMgr::Get_Instance()->Get_LastObj(OBJID::FIRE)->Set_Speed(2.f + float(rand() % 500) / 100.f);
	}
}
