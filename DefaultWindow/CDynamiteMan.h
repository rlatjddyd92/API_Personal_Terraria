#pragma once
#include "define.h"
#include "CBoss.h"

class CDynamiteMan : public CBoss
{
public:
	CDynamiteMan () : m_iMachine_Gun(0) {}
	virtual ~CDynamiteMan() {}

public:
	virtual void Initialize()		override;
	virtual void Update()	    override;
	virtual OBJRESULT Late_Update()		override;
	virtual void Render(HDC hDC)	override;
	virtual void Release()			override;
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;

	

private:
	

	// �̵�
	void		 Jump();
	void		 Move_to_Player();

	// Phase 1
	void		 Slow_Explosion_Missile(); // <- ���� ��ź �߻� (�浹 ��� ����)
	void		 Bomb_Rain(); // �ϴÿ��� ����ź ����߸��� 

	// Phase 2
	void		 Dirt_Prison(); // <- ���� ���α� 
	void		 Fast_Explosion_Missile(); // <- ��� ��ź �߻� (�浹 ��� ����), ���α�� ����
	void		 Fire_Bomb(); // ȭ��ź
	void		 Spray_Fast_Explosion_Missile(); // <- ������� ��� ��ź �߻� 
	
	// ��� ��
	void		 Dead_Scene();
	void		 Nuke(); // ������ 
	void		 Drop_Item();

	// Utill
	int			 m_iMachine_Gun;

};

