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
	

	// ¿Ãµø
	void		 Jump();
	void		 Move_to_Player();

	// Phase 1
	void		 Slow_Explosion_Missile(); // <- ¿˙º” ¿Ø≈∫ πﬂªÁ (√Êµπ ¡ÔΩ√ ∆¯πﬂ)
	void		 Bomb_Rain(); // «œ¥√ø°º≠ ¡˝º”≈∫ ∂≥æÓ∂ﬂ∏Æ±‚ 

	// Phase 2
	void		 Dirt_Prison(); // <- ¿Ø¿˙ ∞°µŒ±‚ 
	void		 Fast_Explosion_Missile(); // <- ∞Ìº” ¿Ø≈∫ πﬂªÁ (√Êµπ ¡ÔΩ√ ∆¯πﬂ), ∞°µŒ±‚øÕ ø¨∞Ë
	void		 Fire_Bomb(); // »≠ø∞≈∫
	void		 Spray_Fast_Explosion_Missile(); // <- ªÁπÊ¿∏∑Œ ∞Ìº” ¿Ø≈∫ πﬂªÁ 
	
	// ªÁ∏¡ Ω√
	void		 Dead_Scene();
	void		 Nuke(); // «Ÿ∆¯πﬂ 
	void		 Drop_Item();

	// Utill
	int			 m_iMachine_Gun;

};

