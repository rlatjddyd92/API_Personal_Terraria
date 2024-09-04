#pragma once
#include "Define.h"
#include "Obj.h"


class CMonster : public CObj
{
public:
	CMonster() : m_bAir(false), m_bRange(false), m_bActive(false), m_bAttack(false), m_ImageNum(0), m_bRight(false), m_fNext(0), m_fAttackY(0), m_tImage({0,0,0,0}), m_tImageRect({ 0,0,0,0 }) 
		, m_fAttackYAdjust(0), m_tBeforeInfo({-1,-1,0,0}) {}
	virtual ~CMonster() {}

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual OBJRESULT Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;

	bool		 Get_Range() { return m_bRange; }
	void		 Set_Range(bool Input) { m_bRange = Input; }

	bool		 Get_Air() { return m_bAir; }
	void		 Set_Air(bool Input) { m_bAir = Input; }

	bool		 Get_Exp() { return m_bExp; }
	void		 Set_Exp(bool Input) { m_bExp = Input; }

private:
	INFO		m_tBeforeInfo;

	void		 Move_to_Player();
	bool		 m_bAir;
	bool		 m_bRange;
	bool		 m_bExp;

	// Ground - Range - Exp = 폭발 화살 
	// Ground - Range - !Exp = 마법 
	// Ground - !Range - Exp = 자폭토끼 
	// Ground - !Range - !Exp = 좀비
	// Air - Range - Exp = 폭탄 떨구기 
	// Air - Range - !Exp = 공중에서 총알 발사 
	// Air - !Range - Exp = 폭발 해골 
	// Air - !Range - !Exp = 공중 몸통 박치기 -> 이후 좀비처럼 행동 

	bool		 m_bActive;

	// 몬스터 공격
	void		 Attack_Melee(); // Melee Obj 생성 공격
	void		 Attack_Gun(); // Missile Obj 생성 공격
	void		 Attack_Exp(); // Explosion Obj 생성 공격

	int			 m_ImageNum; // 이미지 관련 (위에서 몇 번째 이미지를 사용하는지)
	bool		 m_bRight; // 이미지 관련 (오른쪽을 바라보는지)
	float		 m_fNext; // 다음 이미지로 넘어가기 위해 필요한 픽셀
	bool		 m_bAttack; // 공격 이미지를 불러올 것인지 

	INFO		 m_tImage; // 이미지 출력 INFO 
	RECT		 m_tImageRect; // 이미지 출력 범위 

	void		 Make_Image_Rect();

	float		 m_fAttackY; // Air 몬스터 전용, 공격을 진행하는 위치 (유저보다 얼마나 위에서 공격하는지)
	float		 m_fAttackYAdjust;
	float		 m_fAttackXAdjust;

	CHANNELID	 m_ChaId;
};

