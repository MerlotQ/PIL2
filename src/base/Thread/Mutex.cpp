#include "Mutex.h"

#define PLATEFORM_INCLUDE_SOURCE

#if defined(PI_OS_FAMILY_WINDOWS)
#include "Mutex_Win32.cpp"
#elif defined(PI_VXWORKS)
#include "Mutex_VX.cpp"
#else
#include "Mutex_POSIX.cpp"
#endif
