
#ifdef PLATEFORM_INCLUDE_SOURCE

#include "../Debug/Assert.h"
#include "MutexRW_Win32.h"


namespace pi {


MutexRWImpl::MutexRWImpl(): _readers(0), _writersWaiting(0), _writers(0)
{
	_mutex = CreateMutexW(NULL, FALSE, NULL);
	if (_mutex == NULL)
		throw SystemException("cannot create reader/writer lock");

	_readEvent = CreateEventW(NULL, TRUE, TRUE, NULL);
	if (_readEvent == NULL)
		throw SystemException("cannot create reader/writer lock");

	_writeEvent = CreateEventW(NULL, TRUE, TRUE, NULL);
	if (_writeEvent == NULL)
		throw SystemException("cannot create reader/writer lock");
}


MutexRWImpl::~MutexRWImpl()
{
	CloseHandle(_mutex);
	CloseHandle(_readEvent);
	CloseHandle(_writeEvent);
}


inline void MutexRWImpl::addWriter()
{
	switch (WaitForSingleObject(_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		if (++_writersWaiting == 1) ResetEvent(_readEvent);
		ReleaseMutex(_mutex);
		break;
	default:
		throw SystemException("cannot lock reader/writer lock");
	}
}


inline void MutexRWImpl::removeWriter()
{
	switch (WaitForSingleObject(_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		if (--_writersWaiting == 0 && _writers == 0) SetEvent(_readEvent);
		ReleaseMutex(_mutex);
		break;
	default:
		throw SystemException("cannot lock reader/writer lock");
	}
}


void MutexRWImpl::readLockImpl()
{
	HANDLE h[2];
	h[0] = _mutex;
	h[1] = _readEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, INFINITE))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		++_readers;
		ResetEvent(_writeEvent);
		ReleaseMutex(_mutex);
        //poco_assert_dbg(_writers == 0);
		break;
	default:
		throw SystemException("cannot lock reader/writer lock");
	}
}


bool MutexRWImpl::tryReadLockImpl()
{
	for (;;)
	{
		if (_writers != 0 || _writersWaiting != 0)
			return false;

		DWORD result = tryReadLockOnce();
		switch (result)
		{
		case WAIT_OBJECT_0:
		case WAIT_OBJECT_0 + 1:
			return true;
		case WAIT_TIMEOUT:
			continue; // try again
		default:
			throw SystemException("cannot lock reader/writer lock");
		}
	}
}


void MutexRWImpl::writeLockImpl()
{
	addWriter();
	HANDLE h[2];
	h[0] = _mutex;
	h[1] = _writeEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, INFINITE))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		--_writersWaiting;
		++_readers;
		++_writers;
		ResetEvent(_readEvent);
		ResetEvent(_writeEvent);
		ReleaseMutex(_mutex);
        //poco_assert_dbg(_writers == 1);
		break;
	default:
		removeWriter();
		throw SystemException("cannot lock reader/writer lock");
	}
}


bool MutexRWImpl::tryWriteLockImpl()
{
	addWriter();
	HANDLE h[2];
	h[0] = _mutex;
	h[1] = _writeEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, 1))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		--_writersWaiting;
		++_readers;
		++_writers;
		ResetEvent(_readEvent);
		ResetEvent(_writeEvent);
		ReleaseMutex(_mutex);
        //poco_assert_dbg(_writers == 1);
		return true;
	case WAIT_TIMEOUT:
		removeWriter();
		return false;
	default:
		removeWriter();
		throw SystemException("cannot lock reader/writer lock");
	}
}


void MutexRWImpl::unlockImpl()
{
	switch (WaitForSingleObject(_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		_writers = 0;
		if (_writersWaiting == 0) SetEvent(_readEvent);
		if (--_readers == 0) SetEvent(_writeEvent);
		ReleaseMutex(_mutex);
		break;
	default:
		throw SystemException("cannot unlock reader/writer lock");
	}
}


DWORD MutexRWImpl::tryReadLockOnce()
{
	HANDLE h[2];
	h[0] = _mutex;
	h[1] = _readEvent;
	DWORD result = WaitForMultipleObjects(2, h, TRUE, 1); 
	switch (result)
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		++_readers;
		ResetEvent(_writeEvent);
		ReleaseMutex(_mutex);
        //poco_assert_dbg(_writers == 0);
		return result;
	case WAIT_TIMEOUT:
		return result;
	default:
		throw SystemException("cannot lock reader/writer lock");
	}
}


} // namespace pi

#endif // end of PLATEFORM_INCLUDE_SOURCE

