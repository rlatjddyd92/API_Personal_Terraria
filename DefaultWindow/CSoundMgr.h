#pragma once
#include "Define.h"

class CSoundMgr
{
public:
    static CSoundMgr* Get_Instance()
    {
        if (nullptr == m_pInstance)
            m_pInstance = new CSoundMgr;

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
    CSoundMgr();
    ~CSoundMgr();

public:
    void Initialize();
    void Release();

public:
    void PlaySound(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
    void PlayBGM(const TCHAR* pSoundKey, float fVolume);
    void StopSound(CHANNELID eID);
    void StopAll();
    void SetChannelVolume(CHANNELID eID, float fVolume);

public:
    void LoadSoundFile(const char* soundName);
    void LoadSoundFile();
    CHANNELID Set_MonChannel();
    CHANNELID Set_ExpChannel();

private:
    static CSoundMgr* m_pInstance;

    // 사운드 리소스 정보를 갖는 객체 
    map<TCHAR*, FMOD::Sound*> m_mapSound;

    // FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
    FMOD::Channel* m_pChannelArr[int(CHANNELID::MAXCHANNEL)];

    // 사운드 ,채널 객체 및 장치를 관리하는 객체 
    FMOD::System* m_pSystem;

private:
    int         m_iMonster;
    int         m_iExp;
};

