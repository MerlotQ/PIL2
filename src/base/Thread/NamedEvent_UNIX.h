
#ifndef Foundation_NamedEvent_UNIX_INCLUDED
#define Foundation_NamedEvent_UNIX_INCLUDED

#include <string>

#include "../Environment.h"

#if defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
#include <semaphore.h>
#endif


namespace pi {


class PIL_API NamedEventImpl
{
protected:
	NamedEventImpl(const std::string& name);	
	~NamedEventImpl();
	void setImpl();
	void waitImpl();
	
private:
	std::string getFileName();

	std::string _name;
#if defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX)
	sem_t* _sem;
#else
	int _lockfd; // lock file descriptor
	int _semfd;  // file used to identify semaphore
	int _semid;  // semaphore id
#endif
};


} // namespace pi


#endif // Foundation_NamedEvent_UNIX_INCLUDED
