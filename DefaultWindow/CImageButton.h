#pragma once
#include "Obj.h"

class CImageButton : public CObj
{
public: 
	CImageButton();
	virtual ~CImageButton();

public:
	virtual void Initialize()		override;
	virtual void Update()			override;
	virtual OBJRESULT Late_Update()		override;
	virtual void Render(HDC hDC)	override;
	virtual void Release()			override;
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;

public:
	void Set_Function(BUTTONFUNCTION input);

private:
	BUTTONFUNCTION m_Function;
	bool m_bCursor;
};

