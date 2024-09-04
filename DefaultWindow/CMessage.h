#pragma once
#include "Define.h"
#include "Obj.h"

class CMessage : public CObj
{
public:
	CMessage() : m_iOpenCount(200), m_bItem(false), m_bItemOpen(false){}
	virtual ~CMessage() { Release(); }

public:
	virtual void Initialize()		override;
	virtual void Update()	    override;
	virtual OBJRESULT Late_Update()		override;
	virtual void Render(HDC hDC)	override;
	virtual void Release()			override;
	virtual void Collision_Rect_Obj(CObj* opponent)	override;
	virtual void Collision_Eillpse_Obj(CObj* opponent)	override;
	void		 Set_ItemToolTip() { m_bItem = true; }
	void		 Set_Message(const TCHAR* Input) 
	{ 
		if (!m_bItem)
			m_tMessage = Input;
		else
		{
			map<const TCHAR*, const TCHAR*>::iterator iter = m_mapItem.find(Input);
			if (iter != m_mapItem.end())
				m_tMessage = iter->second;
		}
			
	}
	void		 Render_Item(HDC hDC);
	void		 Set_ItemOpen() { m_bItemOpen = true; }
	void		 Set_OpenCount(int Input) { m_iOpenCount = Input; }

private:
	map<const TCHAR*, const TCHAR*> m_mapItem; // ������ ���� ���� 
	int			m_iOpenCount; // �޽����� �����ϴ� �ð� ī��Ʈ 
	bool		m_bItem; // ������ �������� �ƴ��� ǥ�� 
	const TCHAR* m_tMessage;
	bool		m_bItemOpen; 

};

