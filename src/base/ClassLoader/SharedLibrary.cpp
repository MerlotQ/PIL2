#include "SharedLibrary.h"
#include "../Debug/Exception.h"


#define PLATEFORM_INCLUDE_SOURCE

#if defined(hpux) || defined(_hpux)
#include "SharedLibrary_HPUX.cpp"
#elif defined(PIL_VXWORKS)
#include "SharedLibrary_VX.cpp"
#elif defined(PIL_OS_FAMILY_UNIX)
#include "SharedLibrary_UNIX.cpp"
#elif defined(PIL_OS_FAMILY_WINDOWS) && defined(PIL_WIN32_UTF8)
#include "SharedLibrary_WIN32.cpp"
#elif defined(PIL_OS_FAMILY_WINDOWS)
#include "SharedLibrary_WIN32.cpp"
#elif defined(PIL_OS_FAMILY_VMS)
#include "SharedLibrary_VMS.cpp"
#endif

namespace pi {

SharedLibrary::SharedLibrary()
{
}


SharedLibrary::SharedLibrary(const std::string& path)
{
    loadImpl(path, 0);
}

SharedLibrary::~SharedLibrary()
{
}

void SharedLibrary::load(const std::string& path, int flags)
{
    loadImpl(path, flags);
}


void SharedLibrary::unload()
{
    unloadImpl();
}


bool SharedLibrary::isLoaded() const
{
    return isLoadedImpl();
}


bool SharedLibrary::hasSymbol(const std::string& name)
{
    return findSymbolImpl(name) != 0;
}


void* SharedLibrary::getSymbol(const std::string& name)
{
    void* result = findSymbolImpl(name);
    if (result)
        return result;
    else
        throw NotFoundException(name);
}


const std::string& SharedLibrary::getPath() const
{
    return getPathImpl();
}


std::string SharedLibrary::suffix()
{
    return suffixImpl();
}


} // namespace pi
