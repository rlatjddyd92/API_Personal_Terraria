#pragma once

#include "define.h"
#include "Obj.h"

class CMouse : public CObj
{
public:
	CMouse() { Release(); };
	virtual ~CMouse() {};

public:
	virtual void Initialize()		override;
	virtual void Update()	    override;
	virtual OBJRESULT Late_Update()		override;
	virtual void Render(HDC hDC)	override;
	virtual void Release()			override;
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;


};

