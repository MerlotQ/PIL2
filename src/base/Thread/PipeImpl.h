#ifndef PIL_PipeImpl_INCLUDED
#define PIL_PipeImpl_INCLUDED


#include "../Environment.h"


#if defined(PIL_OS_FAMILY_WINDOWS)
#if defined(_WIN32_WCE)
#include "PipeImpl_DUMMY.h"
#else
#include "PipeImpl_WIN32.h"
#endif
#elif defined(PIL_OS_FAMILY_UNIX)
#include "PipeImpl_POSIX.h"
#else
#include "PipeImpl_DUMMY.h"
#endif


#endif // 
