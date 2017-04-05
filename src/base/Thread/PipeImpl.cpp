#include "PipeImpl.h"

#define PLATEFORM_INCLUDE_SOURCE

#if defined(PIL_OS_FAMILY_WINDOWS)
#if defined(_WIN32_WCE)
#include "PipeImpl_DUMMY.cpp"
#else
#include "PipeImpl_WIN32.cpp"
#endif
#elif defined(PIL_OS_FAMILY_UNIX)
#include "PipeImpl_POSIX.cpp"
#else
#include "PipeImpl_DUMMY.cpp"
#endif
