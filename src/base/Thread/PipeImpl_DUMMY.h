
#ifndef Foundation_PipeImpl_DUMMY_INCLUDED
#define Foundation_PipeImpl_DUMMY_INCLUDED


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
};


} // namespace pi


#endif 