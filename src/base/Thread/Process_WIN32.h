#ifndef Foundation_Process_WIN32_INCLUDED
#define Foundation_Process_WIN32_INCLUDED

#include <vector>
#include <map>

#include "../Environment.h"
#include "../Platform/UnWindows.h"

#include "RefCountedObject.h"


namespace pi {


class Pipe;


class PIL_API ProcessHandleImpl: public RefCountedObject
{
public:
	ProcessHandleImpl(HANDLE _hProcess, UInt32 pid);
	~ProcessHandleImpl();
	
	UInt32 id() const;
	HANDLE process() const;
	int wait() const;
	void closeHandle();

private:
	HANDLE _hProcess;
	UInt32 _pid;
	
	ProcessHandleImpl(const ProcessHandleImpl&);
	ProcessHandleImpl& operator = (const ProcessHandleImpl&);
};


class PIL_API ProcessImpl
{
public:
	typedef UInt32 PIDImpl;
	typedef std::vector<std::string> ArgsImpl;
	typedef std::map<std::string, std::string> EnvImpl;
	
	static PIDImpl idImpl();
	static void timesImpl(long& userTime, long& kernelTime);
	static ProcessHandleImpl* launchImpl(
		const std::string& command, 
		const ArgsImpl& args, 
		const std::string& initialDirectory,
		Pipe* inPipe, 
		Pipe* outPipe, 
		Pipe* errPipe,
		const EnvImpl& env);
	static void killImpl(ProcessHandleImpl& handle);
	static void killImpl(PIDImpl pid);
	static bool isRunningImpl(const ProcessHandleImpl& handle);
	static bool isRunningImpl(PIDImpl pid);
	static void requestTerminationImpl(PIDImpl pid);
	static std::string terminationEventName(PIDImpl pid);
};


} // namespace pi


#endif // Foundation_Process_WIN32_INCLUDED
