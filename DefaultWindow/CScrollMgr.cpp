#include "stdafx.h"
#include "CScrollMgr.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr()
	: m_fScrollX(0.f), m_fScrollY(0.f)
{
}


CScrollMgr::~CScrollMgr()
{
}