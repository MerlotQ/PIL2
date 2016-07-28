
#ifndef __EVENT_WIN32_H__
#define __EVENT_WIN32_H__

#include <windows.h>

#include "../Environment.h"
#include "../Platform/UnWindows.h"
#include "../Debug/Exception.h"


namespace pi {


class PIL_API EventImpl
{
protected:
	EventImpl(bool autoReset);		
	~EventImpl();
	void setImpl();
	void waitImpl();
	bool waitImpl(long milliseconds);
	void resetImpl();
	
private:
	HANDLE _event;
};


//
// inlines
//
inline void EventImpl::setImpl()
{
	if (!SetEvent(_event))
	{
		throw SystemException("cannot signal event");
	}
}


inline void EventImpl::resetImpl()
{
	if (!ResetEvent(_event))
	{
		throw SystemException("cannot reset event");
	}
}


} // namespace pi


#endif // end of __EVENT_WIN32_H__
