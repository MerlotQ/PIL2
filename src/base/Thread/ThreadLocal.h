#ifndef PIL_ThreadLocal_INCLUDED
#define PIL_ThreadLocal_INCLUDED

#include <map>

#include "Thread.h"

namespace pi {

#if PIL_OS_FAMILY_UNIX

template<class C> class ThreadLocal
{
private:
    pthread_key_t key;

    static void deleter(void* v)
    {
        delete static_cast<C*>(v);
    }

public:
    ThreadLocal()
    {
        pthread_key_create(&key, deleter);
        pthread_setspecific(key, new C);
    }

    ~ThreadLocal()
    {
        deleter(pthread_getspecific(key));
        pthread_setspecific(key, 0);
        pthread_key_delete(key);
    }


    C& operator()()
    {
        return *static_cast<C*>(pthread_getspecific(key));
    }
};

#elif defined(PIL_OS_FAMILY_WINDOWS)

template<class C> class ThreadLocal
{
private:
    DWORD key;

    static void deleter(void* v)
    {
        delete static_cast<C*>(v);
    }

public:
    ThreadLocal()
    {
        key = TlsAlloc();
        TlsSetValue(key, (LPVOID) new C);
    }

    ~ThreadLocal()
    {
        TlsFree(key);
    }

    C& operator()()
    {
        return *static_cast<C*>(TlsGetValue(key));
    }
};

#endif

} // namespace pi


#endif // PIL_ThreadLocal_INCLUDED
