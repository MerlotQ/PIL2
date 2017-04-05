#ifndef PIL_PipeImpl_POSIX_INCLUDED
#define PIL_PipeImpl_POSIX_INCLUDED


#include "../Environment.h"
#include "RefCountedObject.h"


namespace pi {


class PIL_API PipeImpl: public RefCountedObject
	/// A dummy implementation of PipeImpl for platforms
	/// that do not support pipes.
{
public:
	typedef int Handle;

	PipeImpl();
	~PipeImpl();
	int writeBytes(const void* buffer, int length);
	int readBytes(void* buffer, int length);
	Handle readHandle() const;
	Handle writeHandle() const;
	void closeRead();
	void closeWrite();
	
private:
	int _readfd;
	int _writefd;
};


} // namespace pi


#endif // PIL_PipeImpl_POSIX_INCLUDED
