
#pragma once
#ifndef _MULTITHREAD_H_
#define _MULTITHREAD_H_

#include <Windows.h>

class CKey
{
public:
	CKey();
	~CKey();

	void Lock();
	void UnLock();
private:
	CKey(const CKey&);
	void operator = (const CKey&);

	CRITICAL_SECTION m_criSection;
};

class CLocker
{
public:
	CLocker(CKey* pKey);
	~CLocker();

private:
	CLocker(const CLocker& );
	void operator = (const CLocker&);

	CKey* m_pKey;
};
#endif