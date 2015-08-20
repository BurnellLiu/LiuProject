#include "MultiThread.h"


CKey::CKey()
{
	InitializeCriticalSection(&m_criSection);
}

CKey::~CKey()
{
	DeleteCriticalSection(&m_criSection);
}

void CKey::Lock()
{
	EnterCriticalSection(&m_criSection);
}

void CKey::UnLock()
{
	LeaveCriticalSection(&m_criSection);
}

CLocker::CLocker(CKey* pKey)
{
	m_pKey = pKey;
	m_pKey->Lock();
}

CLocker::~CLocker()
{
	m_pKey->UnLock();
}