
#include "MutexRW.h"

#define PLATEFORM_INCLUDE_SOURCE

#if defined(PIL_OS_FAMILY_WINDOWS)
#include "MutexRW_Win32.cpp"
#elif defined(PIL_VXWORKS)
#include "MutexRW_VX.cpp"
#else
#include "MutexRW_POSIX.cpp"
#endif
