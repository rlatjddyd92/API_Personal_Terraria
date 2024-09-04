#pragma once

#include "define.h"
#include "Obj.h"

class CMissile : public CObj
{
public:
	CMissile();
	virtual ~CMissile();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual OBJRESULT Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;

	void		 Set_Arrow(bool Input) {m_bArrow = Input;}
	bool		 Get_Arrow() { return m_bArrow; }

	void		 Set_Explosion(bool Input, float Size) { m_bExplosion = Input; m_fExplosionSize = Size; }
	bool		 Get_Explosion() { return m_bExplosion; }

	void		 Set_Drop(bool Input) { m_bDrop = Input; }
	bool		 Get_Drop() { return m_bDrop; }

	void		 Set_Fire(bool Input) { m_bFire = Input; }
	bool		 Get_Fire() { return m_bFire; }

	void		 Set_Magic(bool Input) { m_bMagic = Input; }
	bool		 Get_Magic() { return m_bMagic; }

	void		 Set_Energy(bool Input) { m_bEnergy = Input; }
	bool		 Get_Energy() { return m_bEnergy; }

private:
	void	Special_Dead();
	bool	m_bArrow;
	bool	m_bExplosion;
	bool	m_bDrop;
	bool	m_bFire;
	float   m_fExplosionSize;
	bool	m_bMagic; // <- 마법효과 필요한 지 
	bool	m_bEnergy; // <- 에너지 구 효과 필요한지 

	int		m_iFlyCount;
};

