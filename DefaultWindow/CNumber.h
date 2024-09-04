#pragma once
#include "Define.h"
#include "Obj.h"

class CNumber : public CObj
{
public:
	CNumber() : m_fNumber(0) {}
	virtual ~CNumber() { Release(); }

public:
	virtual void Initialize()		override;
	virtual void Update()	    override;
	virtual OBJRESULT Late_Update()		override;
	virtual void Render(HDC hDC)	override;
	virtual void Release()			override;
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;
	void		 Set_NumBer(float Input) { m_fNumber = Input; }

private:
	float		 m_fNumber;

	float        m_fStartX;
	float        m_fStartY;
};

