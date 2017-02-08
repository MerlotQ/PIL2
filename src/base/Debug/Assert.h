#ifndef ASSERT_H
#define ASSERT_H

#include <iostream>
#include <tr1/functional>

namespace pi
{

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// main template
template<bool>
struct StaticAssert;

// specialization
template<>
struct StaticAssert<true>
{};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define PI_DEBUG_LEVEL_ERROR    1
#define PI_DEBUG_LEVEL_WARN     2
#define PI_DEBUG_LEVEL_INFO     3
#define PI_DEBUG_LEVEL_TRACE    4       // default
#define PI_DEBUG_LEVEL_NORM     5

// debug level functions
void dbg_set_level(int i);
int  dbg_get_level(void);
void dbg_push_level(int i);
int  dbg_pop_level(void);

// debug print functions
void dbg_printf(int level,
                const char *fname, int line, const char *func,
                const char *szFmtString, ...);


// log information call back function, regist, and unregist function
typedef std::tr1::function<int (std::string&)> LogInfo_Message_Handle;

int dbg_registMessageHandle(const std::string &handleName, LogInfo_Message_Handle &msgHandle);
int dbg_unregistMessageHandle(const std::string &handleName);


} // end of namespace pi

#define pi_static_assert(exp) \
{ pi::StaticAssert<((exp) != 0)> StaticAssertFailed; }

#ifndef pi_assert
#define pi_assert(f)\
    do { \
        if (!(f) ) { \
            fprintf(stderr, "ERROR (FILE: %s, LINE: %d, FUNC: %s)\n", \
                __FILE__, __LINE__, __FUNCTION__); \
            exit(1); \
        } \
    } while (0);

#define pi_assert2(f,s)\
    do { \
        if (!(f) ) { \
            fprintf(stderr, "ERROR: %s (FILE: %s, LINE: %d, FUNC: %s)\n", \
                s, __FILE__, __LINE__, __FUNCTION__); \
            exit(1); \
        } \
    } while (0);
#endif

#define pi_check_ptr(ptr) pi_assert(ptr)

#if defined(_DEBUG)
    #define pi_assert_dbg(f)    pi_assert(f)
    #define pi_assert2_dbg(f,s) pi_assert2(f,s)
#else
    #define pi_assert_dbg(f)
    #define pi_assert2_dbg(f,s)
#endif

#define pi_unexpected() pi_dbg_error()


// level 1: error message
#define pi_dbg_error(...)   pi::dbg_printf(1, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

// level 2: warning message
#define pi_dbg_warn(...)    pi::dbg_printf(2, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

// level 3: information message (default)
#define pi_dbg_info(...)    pi::dbg_printf(3, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

// level 4: trace message
#define pi_dbg_trace(...)   pi::dbg_printf(4, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

// level 5: normal message
#define pi_dbg_message(...) pi::dbg_printf(5, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)


#endif // ASSERT_H
