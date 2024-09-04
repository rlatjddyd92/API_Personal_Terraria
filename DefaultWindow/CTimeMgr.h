#pragma once
class CTimeMgr
{
public:
	CTimeMgr() {};
	~CTimeMgr() {  };

public:




public:
	static CTimeMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTimeMgr;

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CTimeMgr* m_pInstance;
};

