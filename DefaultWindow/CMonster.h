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

	// Ground - Range - Exp = ���� ȭ�� 
	// Ground - Range - !Exp = ���� 
	// Ground - !Range - Exp = �����䳢 
	// Ground - !Range - !Exp = ����
	// Air - Range - Exp = ��ź ������ 
	// Air - Range - !Exp = ���߿��� �Ѿ� �߻� 
	// Air - !Range - Exp = ���� �ذ� 
	// Air - !Range - !Exp = ���� ���� ��ġ�� -> ���� ����ó�� �ൿ 

	bool		 m_bActive;

	// ���� ����
	void		 Attack_Melee(); // Melee Obj ���� ����
	void		 Attack_Gun(); // Missile Obj ���� ����
	void		 Attack_Exp(); // Explosion Obj ���� ����

	int			 m_ImageNum; // �̹��� ���� (������ �� ��° �̹����� ����ϴ���)
	bool		 m_bRight; // �̹��� ���� (�������� �ٶ󺸴���)
	float		 m_fNext; // ���� �̹����� �Ѿ�� ���� �ʿ��� �ȼ�
	bool		 m_bAttack; // ���� �̹����� �ҷ��� ������ 

	INFO		 m_tImage; // �̹��� ��� INFO 
	RECT		 m_tImageRect; // �̹��� ��� ���� 

	void		 Make_Image_Rect();

	float		 m_fAttackY; // Air ���� ����, ������ �����ϴ� ��ġ (�������� �󸶳� ������ �����ϴ���)
	float		 m_fAttackYAdjust;
	float		 m_fAttackXAdjust;

	CHANNELID	 m_ChaId;
};

