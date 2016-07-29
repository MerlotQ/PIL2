#include "Event.h"

#define PLATEFORM_INCLUDE_SOURCE

#if defined(PIL_OS_FAMILY_WINDOWS)
#include "Event_Win32.cpp"
#elif defined(PIL_VXWORKS)
#include "Event_VX.cpp"
#else
#include "Event_POSIX.cpp"
#endif

namespace pi {


Event::Event(bool autoReset): EventImpl(autoReset)
{
}


Event::~Event()
{
}


} // namespace pi
