#include "stdafx.h"
#include "CSoundMgr.h"
#include "MainGame.h"

CSoundMgr* CSoundMgr::m_pInstance = nullptr;
CSoundMgr::CSoundMgr() : m_iMonster(0)
{
    m_pSystem = nullptr;
}


CSoundMgr::~CSoundMgr()
{
    Release();
}

void CSoundMgr::Initialize()
{
    FMOD::System_Create(&m_pSystem);
    m_pSystem->init(32, FMOD_INIT_NORMAL, NULL);

    LoadSoundFile("Ailen_Attack_Single.wav");
    LoadSoundFile("Archer_Attack.wav");
    LoadSoundFile("Bomb_Big.wav");
    LoadSoundFile("Bomb_Middle.wav");
    LoadSoundFile("Bomb_Small.wav");
    LoadSoundFile("Eilen_Attack.wav");
    LoadSoundFile("Eilen_Emerge.wav");
    LoadSoundFile("Fire.wav");
    LoadSoundFile("Fire_1.wav");
    LoadSoundFile("Fire_2.wav");
    LoadSoundFile("Fire_3.wav");
    LoadSoundFile("FIre_Burn.wav");
    LoadSoundFile("Gun.wav");
    LoadSoundFile("Hit_By_Melee.wav");
    LoadSoundFile("Player_Fire_Thrower.wav");
    LoadSoundFile("Skull_Emerge.wav");
    LoadSoundFile("Ghost_Die.wav");
    LoadSoundFile("Archer_Die.wav");
    LoadSoundFile("Peace_Mode.wav");
    LoadSoundFile("Servival_Mod.wav");
    LoadSoundFile("Boss_Mod.wav");
    LoadSoundFile("Dig.wav");
    LoadSoundFile("Fail.wav");

    CSoundMgr::PlayBGM(L"Peace_Mode.wav", 0.3f);
}

void CSoundMgr::Release()
{
    for (auto& Mypair : m_mapSound)
    {
        delete[] Mypair.first;
        Mypair.second->release();
    }
    m_mapSound.clear();

    m_pSystem->release();
    m_pSystem->close();
}




void CSoundMgr::PlaySound(TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
    map<TCHAR*, FMOD::Sound*>::iterator iter;

    iter = find_if(m_mapSound.begin(), m_mapSound.end(),
        [&](auto& iter)->bool
        {
            return !lstrcmp(pSoundKey, iter.first);
        });

    if (iter == m_mapSound.end())
        return;

    bool bPlay = false;
    m_pChannelArr[int(eID)]->isPlaying(&bPlay);
    if (!bPlay)
    {
        m_pSystem->playSound(iter->second, NULL, FALSE, &m_pChannelArr[int(eID)]);
    }

    m_pChannelArr[int(eID)]->setVolume(fVolume);
    m_pChannelArr[int(eID)]->setMode(FMOD_LOOP_OFF);
    m_pSystem->update();
}

void CSoundMgr::PlayBGM(const TCHAR* pSoundKey, float fVolume)
{
    map<TCHAR*, FMOD::Sound*>::iterator iter;

    iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
        {
            return !lstrcmp(pSoundKey, iter.first);
        });

    if (iter == m_mapSound.end())
        return;

    m_pSystem->playSound(iter->second, NULL, FALSE, &m_pChannelArr[int(CHANNELID::SOUND_BGM)]);
    m_pChannelArr[int(CHANNELID::SOUND_BGM)]->setMode(FMOD_LOOP_NORMAL);
    m_pChannelArr[int(CHANNELID::SOUND_BGM)]->setVolume(fVolume);
    m_pSystem->update();
}

void CSoundMgr::StopSound(CHANNELID eID)
{
    m_pChannelArr[int(eID)]->stop();
}

void CSoundMgr::StopAll()
{
    for (int i = 0; i < int(CHANNELID::MAXCHANNEL); ++i)
        m_pChannelArr[i]->stop();
}

void CSoundMgr::SetChannelVolume(CHANNELID eID, float fVolume)
{
    m_pChannelArr[int(eID)]->setVolume(fVolume);
    m_pSystem->update();
}

void CSoundMgr::LoadSoundFile(const char* soundName)
{
    FMOD::Sound* pSound = nullptr;

    char szCurPath[128] = "../Sound/";
    strcat_s(szCurPath, soundName);   // "../Sound/Success.wav"

    FMOD_RESULT eRes = m_pSystem->createSound(szCurPath, FMOD_LOOP_NORMAL | FMOD_DEFAULT, NULL, &pSound);
    if (eRes == FMOD_OK)
    {
        int iLength = strlen(soundName) + 1;

        TCHAR* pSoundKey = new TCHAR[iLength];
        ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

        // 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수
        MultiByteToWideChar(CP_ACP, 0, soundName, iLength, pSoundKey, iLength);

        m_mapSound.emplace(pSoundKey, pSound);
    }

    m_pSystem->update();
}

void CSoundMgr::LoadSoundFile()
{
    // _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
    _finddatai64_t fd;

    // _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
    long handle = _findfirsti64("../Sound/*.*", &fd);

    if (handle == -1)
        return;

    int iResult = 0;

    char szCurPath[128] = "../Sound/";
    char szFullPath[128] = "";

    while (iResult != -1)
    {
        strcpy_s(szFullPath, szCurPath);
        strcat_s(szFullPath, fd.name);   // "../Sound/Success.wav"

        FMOD::Sound* pSound = nullptr;

        FMOD_RESULT eRes = m_pSystem->createSound(szFullPath, FMOD_LOOP_NORMAL | FMOD_DEFAULT, NULL, &pSound);

        if (eRes == FMOD_OK)
        {
            int iLength = strlen(fd.name) + 1;

            TCHAR* pSoundKey = new TCHAR[iLength];
            ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

            // 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수
            MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

            m_mapSound.emplace(pSoundKey, pSound);
        }
        //_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 더이상 없다면 -1을 리턴
        iResult = _findnexti64(handle, &fd);
    }

    m_pSystem->update();

    _findclose(handle);
}

CHANNELID CSoundMgr::Set_MonChannel()
{
    ++m_iMonster;

    if (m_iMonster >= 6)
        m_iMonster = 0;

    return CHANNELID(int(CHANNELID::SOUND_MON_1) + m_iMonster);
}

CHANNELID CSoundMgr::Set_ExpChannel()
{
    ++m_iExp;

    if (m_iExp >= 5)
        m_iExp = 0;

    return CHANNELID(int(CHANNELID::SOUND_MON_1) + m_iExp);
}
