
#ifndef Foundation_NamedEvent_WIN32_INCLUDED
#define Foundation_NamedEvent_WIN32_INCLUDED

#include <string>

#include "../Environment.h"
#include "../Platform/UnWindows.h"


namespace pi {


class PIL_API NamedEventImpl
{
protected:
	NamedEventImpl(const std::string& name);	
	~NamedEventImpl();
	void setImpl();
	void waitImpl();
	
private:
	std::string _name;
	HANDLE      _event;	
};


} // namespace pi


#endif //
