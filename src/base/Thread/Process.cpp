#include "Process.h"
#include "../Utils/Environment.h"
#include "../Debug/Assert.h"


namespace 
{
    std::vector<char> getEnvironmentVariablesBuffer(const pi::Process::Env& env)
	{   
		std::vector<char> envbuf;
		std::size_t pos = 0; 
		
        for (pi::Process::Env::const_iterator it = env.begin(); it != env.end(); ++it)
		{
			std::size_t envlen = it->first.length() + it->second.length() + 1;
	
			envbuf.resize(pos + envlen + 1);
			std::copy(it->first.begin(), it->first.end(), &envbuf[pos]);
			pos += it->first.length();
			envbuf[pos] = '=';
			++pos;
			std::copy(it->second.begin(), it->second.end(), &envbuf[pos]);
			pos += it->second.length();
		
			envbuf[pos] = '\0';
			++pos;
		}
	 
		envbuf.resize(pos + 1);
		envbuf[pos] = '\0';
	
		return envbuf;
	}
	
    void setEnvironmentVariables(const pi::Process::Env& env)
	{
        for (pi::Process::Env::const_iterator it = env.begin(); it != env.end(); ++it)
		{
            pi::Environment::set(it->first, it->second);
		}
	}
}

#define PLATEFORM_INCLUDE_SOURCE

#if defined(PIL_OS_FAMILY_WINDOWS) && defined(PIL_WIN32_UTF8)
#if defined(_WIN32_WCE)
//#include "Process_WINCE.cpp"
#else
//#include "Process_WIN32U.cpp"
#endif
#elif defined(PIL_OS_FAMILY_WINDOWS)
#include "Process_WIN32.cpp"
#elif defined(PIL_VXWORKS)
//#include "Process_VX.cpp"
#elif defined(PIL_OS_FAMILY_UNIX)
#include "Process_UNIX.cpp"
#else
//#include "Process_VMS.cpp"
#endif


namespace pi {


//
// ProcessHandle
//
ProcessHandle::ProcessHandle(const ProcessHandle& handle):
	_pImpl(handle._pImpl)
{
	_pImpl->duplicate();
}

	
ProcessHandle::~ProcessHandle()
{
	_pImpl->release();
}


ProcessHandle::ProcessHandle(ProcessHandleImpl* pImpl):
	_pImpl(pImpl)
{
    pi_check_ptr (_pImpl);
}


ProcessHandle& ProcessHandle::operator = (const ProcessHandle& handle)
{
	if (&handle != this)
	{
		_pImpl->release();
		_pImpl = handle._pImpl;
		_pImpl->duplicate();
	}
	return *this;
}

	
ProcessHandle::PID ProcessHandle::id() const
{
	return _pImpl->id();
}

	
int ProcessHandle::wait() const
{
	return _pImpl->wait();
}


//
// Process
//
ProcessHandle Process::launch(const std::string& command, const Args& args)
{
	std::string initialDirectory;
	Env env;
	return ProcessHandle(launchImpl(command, args, initialDirectory, 0, 0, 0, env));
}


ProcessHandle Process::launch(const std::string& command, const Args& args, const std::string& initialDirectory)
{
	Env env;
	return ProcessHandle(launchImpl(command, args, initialDirectory, 0, 0, 0, env));
}


ProcessHandle Process::launch(const std::string& command, const Args& args, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe)
{
    pi_assert (inPipe == 0 || (inPipe != outPipe && inPipe != errPipe));
	std::string initialDirectory;
	Env env;
	return ProcessHandle(launchImpl(command, args, initialDirectory, inPipe, outPipe, errPipe, env));
}


ProcessHandle Process::launch(const std::string& command, const Args& args, const std::string& initialDirectory, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe)
{
    pi_assert (inPipe == 0 || (inPipe != outPipe && inPipe != errPipe));
	Env env;
	return ProcessHandle(launchImpl(command, args, initialDirectory, inPipe, outPipe, errPipe, env));
}


ProcessHandle Process::launch(const std::string& command, const Args& args, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, const Env& env)
{
    pi_assert (inPipe == 0 || (inPipe != outPipe && inPipe != errPipe));
	std::string initialDirectory;
	return ProcessHandle(launchImpl(command, args, initialDirectory, inPipe, outPipe, errPipe, env));
}


ProcessHandle Process::launch(const std::string& command, const Args& args, const std::string& initialDirectory, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, const Env& env)
{
    pi_assert (inPipe == 0 || (inPipe != outPipe && inPipe != errPipe));
	return ProcessHandle(launchImpl(command, args, initialDirectory, inPipe, outPipe, errPipe, env));
}
	
	
int Process::wait(const ProcessHandle& handle)
{
	return handle.wait();
}


void Process::kill(ProcessHandle& handle)
{
	killImpl(*handle._pImpl);
}


void Process::kill(PID pid)
{
	killImpl(pid);
}

bool Process::isRunning(const ProcessHandle& handle)
{
	return isRunningImpl(*handle._pImpl);
}
bool Process::isRunning(PID pid)
{
	return isRunningImpl(pid);
}

void Process::requestTermination(PID pid)
{
	requestTerminationImpl(pid);
}


} // namespace pi
