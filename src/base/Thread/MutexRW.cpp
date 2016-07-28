#define PLATEFORM_INCLUDE_SOURCE

#if defined(PI_OS_FAMILY_WINDOWS)
#include "MutexRW_Win32.cpp"
#elif defined(PI_VXWORKS)
#include "MutexRW_VX.cpp"
#else
#include "MutexRW_POSIX.cpp"
#endif
