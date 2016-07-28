
#ifndef __PIL_ENVIRONMENT_UNIX_H__
#define __PIL_ENVIRONMENT_UNIX_H__

#include <stdint.h>
#include <map>

#include "../Environment.h"
#include "../Thread/Mutex.h"


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

private:
	typedef std::map<std::string, std::string> StringMap;
	
	static StringMap _map;
	static FastMutex _mutex;
};


} // namespace pi


#endif // __PIL_ENVIRONMENT_UNIX_H__
