#pragma once

#include "define.h"
#include "Obj.h"

class CFire : public CObj
{
public:
	CFire() : m_tFireInfo({ 0,0,0,0 }), m_bFixed(false), m_iImage(0), m_pLink(nullptr), m_fAdjustX(0), m_fAdjustY(0), m_bBurnTree(false) {}
	virtual ~CFire() { Release(); }

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual OBJRESULT Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;

	void		 Burn_Tree();

private:
	INFO		 m_tFireInfo;
	bool		 m_bFixed;
	int			 m_iImage;
	CObj*		 m_pLink;
	bool		 m_bBurnTree;

	float		 m_fAdjustX;
	float		 m_fAdjustY;

	int	         m_iBurnTime;
};

