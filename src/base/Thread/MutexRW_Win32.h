
#ifndef __MUTEXRW_WIN32_H__
#define __MUTEXRW_WIN32_H__


#include "../Environment.h"
#include "../Platform/UnWindows.h"
#include "../Debug/Exception.h"

namespace pi {


class PIL_API MutexRWImpl
{
protected:
    MutexRWImpl();
    ~MutexRWImpl();
	void readLockImpl();
	bool tryReadLockImpl();
	void writeLockImpl();
	bool tryWriteLockImpl();
	void unlockImpl();
	
private:
	void addWriter();
	void removeWriter();
	DWORD tryReadLockOnce();

	HANDLE   _mutex;
	HANDLE   _readEvent;
	HANDLE   _writeEvent;
	unsigned _readers;
	unsigned _writersWaiting;
	unsigned _writers;
};


} // end of namespace pi


#endif // end of __MUTEXRW_WIN32_H__

