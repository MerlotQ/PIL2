

#ifndef __PIL_ENVIRONMENT_WIN32_H__
#define __PIL_ENVIRONMENT_WIN32_H__

#include <stdint.h>
#include <string>

#include "../Environment.h"


namespace pi {


class PIL_API EnvironmentImpl
{
public:
    typedef uint8_t NodeId[6]; /// Ethernet address.

	static std::string getImpl(const std::string& name);	
	static bool hasImpl(const std::string& name);	
	static void setImpl(const std::string& name, const std::string& value);
	static std::string osNameImpl();
	static std::string osDisplayNameImpl();
	static std::string osVersionImpl();
	static std::string osArchitectureImpl();
	static std::string nodeNameImpl();
	static void nodeIdImpl(NodeId& id);
	static unsigned processorCountImpl();
};


} // namespace pi


#endif // __PIL_ENVIRONMENT_WIN32_H__
