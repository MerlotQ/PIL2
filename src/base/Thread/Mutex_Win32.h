

#ifndef __MUTEX_WIN32_H__
#define __MUTEX_WIN32_H__

#include "../Environment.h"
#include "../Platform/UnWindows.h"
#include "../Debug/Exception.h"

namespace pi {


class PIL_API MutexImpl
{
protected:
	MutexImpl();
	~MutexImpl();
	void lockImpl();
	bool tryLockImpl();
	bool tryLockImpl(long milliseconds);
	void unlockImpl();
	
private:
	CRITICAL_SECTION _cs;
};


typedef MutexImpl FastMutexImpl;


//
// inlines
//
inline void MutexImpl::lockImpl()
{
	try
	{
		EnterCriticalSection(&_cs);
	}
	catch (...)
	{
		throw SystemException("cannot lock mutex");
	}
}


inline bool MutexImpl::tryLockImpl()
{
	try
	{
		return TryEnterCriticalSection(&_cs) != 0;
	}
	catch (...)
	{
	}
	throw SystemException("cannot lock mutex");
}


inline void MutexImpl::unlockImpl()
{
	LeaveCriticalSection(&_cs);
}


} // namespace pi


#endif // __MUTEX_WIN32_H__
