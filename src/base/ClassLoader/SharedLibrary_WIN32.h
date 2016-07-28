

#ifndef Foundation_SharedLibrary_WIN32_INCLUDED
#define Foundation_SharedLibrary_WIN32_INCLUDED

#include "../Environment.h"
#include "../Thread/Mutex.h"

namespace pi {


class PIL_API SharedLibraryImpl
{
protected:
	SharedLibraryImpl();
	~SharedLibraryImpl();
	void loadImpl(const std::string& path, int flags);
	void unloadImpl();
	bool isLoadedImpl() const;
	void* findSymbolImpl(const std::string& name);
	const std::string& getPathImpl() const;
	static std::string suffixImpl();

private:
	std::string _path;
	void* _handle;
	static FastMutex _mutex;
};


} // namespace pi


#endif // Foundation_SharedLibrary_WIN32_INCLUDED
