
#ifndef PIL_PipeImpl_WIN32_INCLUDED
#define PIL_PipeImpl_WIN32_INCLUDED

#include "../Environment.h"
#include "../Platform/UnWindows.h"
#include "RefCountedObject.h"


namespace pi {


class PIL_API PipeImpl: public RefCountedObject
	/// A dummy implementation of PipeImpl for platforms
	/// that do not support pipes.
{
public:
	typedef HANDLE Handle;

	PipeImpl();
	~PipeImpl();
	int writeBytes(const void* buffer, int length);
	int readBytes(void* buffer, int length);
	Handle readHandle() const;
	Handle writeHandle() const;
	void closeRead();
	void closeWrite();
	
private:
	HANDLE _readHandle;
	HANDLE _writeHandle;
};


} // namespace pi


#endif // PIL_PipeImpl_WIN32_INCLUDED
