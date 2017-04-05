#ifdef PLATEFORM_INCLUDE_SOURCE


#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#if defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
#include <semaphore.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#endif

#include "NamedEvent_UNIX.h"
#include "../Utils/format/format.h"
#include "../Debug/Exception.h"

namespace pi {


#if (PIL_OS == PIL_OS_LINUX) || (PIL_OS == PIL_OS_CYGWIN) || (PIL_OS == PIL_OS_FREE_BSD)
	union semun
	{
		int                 val;
		struct semid_ds*    buf;
		unsigned short int* array;
		struct seminfo*     __buf;
	};
#elif (PIL_OS == PIL_OS_HPUX)
	union semun
	{
		int              val;
		struct semid_ds* buf;
		ushort*          array;
	};
#endif


NamedEventImpl::NamedEventImpl(const std::string& name):
	_name(name)
{
	std::string fileName = getFileName();
#if defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
	_sem = sem_open(fileName.c_str(), O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, 0);
	if ((long) _sem == (long) SEM_FAILED) 
        throw SystemException(fmt::sprintf("cannot create named mutex %s (sem_open() failed, errno=%d)", fileName, errno), _name);
#else
	int fd = open(fileName.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd != -1)
		close(fd);
	else 
        throw SystemException(fmt::sprintf("cannot create named event %s (lockfile)", fileName), _name);
	key_t key = ftok(fileName.c_str(), 0);
	if (key == -1)
        throw SystemException(fmt::sprintf("cannot create named mutex %s (ftok() failed, errno=%d)", fileName, errno), _name);
	_semid = semget(key, 1, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | IPC_CREAT | IPC_EXCL);
	if (_semid >= 0)
	{
		union semun arg;
		arg.val = 0;
		semctl(_semid, 0, SETVAL, arg);
	}
	else if (errno == EEXIST)
	{
		_semid = semget(key, 1, 0);
	}
    else throw SystemException(fmt::sprintf("cannot create named mutex %s (semget() failed, errno=%d)", fileName, errno), _name);
#endif // defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
}


NamedEventImpl::~NamedEventImpl()
{
#if defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
	sem_close(_sem);
#endif
}


void NamedEventImpl::setImpl()
{
#if defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
	if (sem_post(_sem) != 0)
	   	throw SystemException("cannot set named event", _name);
#else
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op  = 1;
	op.sem_flg = 0;
	if (semop(_semid, &op, 1) != 0)
	   	throw SystemException("cannot set named event", _name);
#endif
}


void NamedEventImpl::waitImpl()
{
#if defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
	int err;
	do
	{
		err = sem_wait(_sem);
	}
	while (err && errno == EINTR);
	if (err) throw SystemException("cannot wait for named event", _name);
#else
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op  = -1;
	op.sem_flg = 0;
	int err;
	do
	{
		err = semop(_semid, &op, 1);
	}
	while (err && errno == EINTR); 
	if (err) throw SystemException("cannot wait for named event", _name);
#endif
}


std::string NamedEventImpl::getFileName()
{
#if defined(sun) || defined(__APPLE__) || defined(__QNX__)
	std::string fn = "/";
#else
	std::string fn = "/tmp/";
#endif
	fn.append(_name);
	fn.append(".event");
	return fn;
}


} // namespace pi


#endif // end of PLATEFORM_INCLUDE_SOURCE
