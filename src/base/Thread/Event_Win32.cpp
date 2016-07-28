
#ifdef PLATEFORM_INCLUDE_SOURCE

#include "Event_Win32.h"


namespace pi {


EventImpl::EventImpl(bool autoReset)
{
	_event = CreateEventW(NULL, autoReset ? FALSE : TRUE, FALSE, NULL);
	if (!_event)
		throw SystemException("cannot create event");
}


EventImpl::~EventImpl()
{
	CloseHandle(_event);
}


void EventImpl::waitImpl()
{
	switch (WaitForSingleObject(_event, INFINITE))
	{
	case WAIT_OBJECT_0:
		return;
	default:
		throw SystemException("wait for event failed");
	}
}


bool EventImpl::waitImpl(long milliseconds)
{
	switch (WaitForSingleObject(_event, milliseconds + 1))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		return true;
	default:
		throw SystemException("wait for event failed");		
	}
}


} // namespace pi

#endif // end of PLATEFORM_INCLUDE_SOURCE
