#pragma once

#include "define.h"
#include "Obj.h"

class CExplosion : public CObj
{
public:
	CExplosion () : m_fPushPower(100), m_bDestroyTile(true) {}
	virtual ~CExplosion() {}

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual OBJRESULT Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;

	bool		 Get_Nuke() { return m_bNuke; }
	void		 Set_NuKe() { m_bNuke = true; }

	void		 Set_TileMake() { m_bTileMake = true; }
	bool		 Get_TileMake() { return m_bTileMake; }

	void		 Set_FireBomb() { m_FireBomb = true; }
	void		 Set_FireBomb(bool Input) { m_FireBomb = Input; }
	bool		 Get_FireBomb() { return m_FireBomb; }

	void		 Set_DestroyTile(bool Input) { m_bDestroyTile = Input; }
	bool		 Get_DestroyTile() { return m_bDestroyTile; }

private:
	float		 m_fPushPower;
	bool		 m_bNuke;
	int			 m_ImageNum;
	bool		 m_bTileMake;
	bool		 m_FireBomb;
	bool		 m_bDestroyTile;

	CHANNELID	 m_ExpSoundNum; // 지정된 폭발 전용 사운드 채널 번호 
};

