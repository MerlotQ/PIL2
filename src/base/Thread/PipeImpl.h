//
// PipeImpl.h
//
// $Id: //poco/1.4/Foundation/include/Poco/PipeImpl.h#1 $
//
// Library: Foundation
// Package: Processes
// Module:  PipeImpl
//
// Definition of the PipeImpl class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_PipeImpl_INCLUDED
#define Foundation_PipeImpl_INCLUDED


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


#endif // Foundation_PipeImpl_INCLUDED
