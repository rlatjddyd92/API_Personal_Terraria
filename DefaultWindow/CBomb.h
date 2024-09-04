#pragma once
#include "Define.h"
#include "Obj.h"

class CBomb : public CObj
{
public:
	CBomb();
	virtual ~CBomb();


public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual OBJRESULT Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;

	void		 Set_ExplosionCount(int Input) { m_iExplosionCount = Input; }
	int			 Get_ExplosionCount() { return m_iExplosionCount;  }

	void		 Set_Fire() { m_bFire = true; }
	bool		 Get_Fire() { return m_bFire; }

	void		 Set_Bouncy() { m_bBouncy = true; }
	bool		 Get_Bouncy() { return m_bBouncy; }

	void		 Set_Chain() { m_bChainExp = true; }
	bool		 Get_Chain() { return m_bChainExp; }

	void		 Set_Mine() { m_bMine = true; }
	bool		 Get_Mine() { return m_bMine; }

	void		 Set_FireSpray() { m_bFIreSpray = true; m_tInfo.fCX = 16.f; m_tInfo.fCY = 8.f; }
	bool		 Get_FireSpray() { return m_bFIreSpray; }

	void		 Set_High() { m_bHighExp = true; m_tInfo.fCX = 16.f; m_tInfo.fCY = 8.f; }
	bool		 Get_High() { return m_bHighExp; }

private:
	void		 Mine_Explosion();

	int			 m_iExplosionCount;
	bool		 m_bFire;
	bool		 m_bBouncy;
	bool	     m_bChainExp;


	// Áö·Ú Àü¿ë 
	bool		 m_bMine;

	int			 m_iMineCount;

	bool		 m_bFIreSpray; // È­¿°À» ¸¹ÀÌ ¸¹µë
	bool		 m_bHighExp;  // À§·Î Æ¢¾î¿Ã¶ó Æø¹ß

	bool		 m_bMineActive;

	bool		 m_bJump;
};

