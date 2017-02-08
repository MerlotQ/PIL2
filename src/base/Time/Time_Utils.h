
/*******************************************************************************

  Pilot Intelligence Library
    http://www.pilotintelligence.com/

  ----------------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

#ifndef __TIME_UTILS_H__
#define __TIME_UTILS_H__


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>


#if defined(PIL_OS_FAMILY_WINDOWS) 

////////////////////////////////////////////////////////////////////////////////
/// Windows
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>


namespace pi {

inline uint64_t tm_get_millis(void)
{
    return GetTickCount();
}

inline uint64_t tm_get_ms(void)
{
    return GetTickCount();
}

inline uint64_t tm_get_us(void)
{
    FILETIME        t;
    uint64_t        t_ret;

    // get UTC time
    GetSystemTimeAsFileTime(&t);

    t_ret = 0;

    t_ret |= t.dwHighDateTime;
    t_ret <<= 32;
    t_ret |= t.dwLowDateTime;

    // convert 100 ns to [ms]
    return t_ret/10;
}

inline double tm_getTimeStamp(void)
{
    FILETIME        t;
    uint64_t        t_ret;
    double          ts;

    // get UTC time
    GetSystemTimeAsFileTime(&t);

    t_ret = 0;

    t_ret |= t.dwHighDateTime;
    t_ret <<= 32;
    t_ret |= t.dwLowDateTime;

    // convert 100 ns to second
    ts = 1.0 * t_ret / 1e7;

    return ts;
}


inline int64_t tm_getTimeStampUnix(void)
{
    FILETIME        t;
    uint64_t        t_ret;
    int64_t         ts;

    // get UTC time
    GetSystemTimeAsFileTime(&t);

    t_ret = 0;

    t_ret |= t.dwHighDateTime;
    t_ret <<= 32;
    t_ret |= t.dwLowDateTime;

    // convert 100 ns to second
    ts = t_ret / 10000000;

    return ts;
}

inline void tm_sleep(uint32_t t)
{
    Sleep(t);
}

inline void tm_sleep_us(uint64_t t)
{
    HANDLE timer;
    LARGE_INTEGER ft;

    // Convert to 100 nanosecond interval, negative value indicates relative time
    ft.QuadPart = -(10*t);

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

} // end of namespace pi

#else

////////////////////////////////////////////////////////////////////////////////
/// UNIX
////////////////////////////////////////////////////////////////////////////////


#include <sys/time.h>
#include <sys/timeb.h>

namespace pi {


inline uint64_t tm_get_millis(void)
{
    struct timeval  tm_val;
    uint64_t        v;
    int             ret;

    ret = gettimeofday(&tm_val, NULL);
    v = tm_val.tv_sec*1000 + tm_val.tv_usec/1000;

    return v;
}

inline uint64_t tm_get_ms(void)
{
    struct timeval  tm_val;
    uint64_t        v;
    int             ret;

    ret = gettimeofday(&tm_val, NULL);
    v = tm_val.tv_sec*1000 + tm_val.tv_usec/1000;

    return v;
}

inline uint64_t tm_get_us(void)
{
    struct timeval  tm_val;
    uint64_t        v;
    int             ret;

    ret = gettimeofday(&tm_val, NULL);
    v = tm_val.tv_sec*1000000 + tm_val.tv_usec;

    return v;
}

inline double tm_getTimeStamp(void)
{
    struct timeval  tm_val;
    double          v;
    int             ret;

    ret = gettimeofday(&tm_val, NULL);
    v = tm_val.tv_sec + 1.0*tm_val.tv_usec/1e6;

    return v;
}


inline int64_t tm_getTimeStampUnix(void)
{
    struct timeval  tm_val;
    int64_t         v;
    int             ret;

    ret = gettimeofday(&tm_val, NULL);
    v = tm_val.tv_sec;

    return v;
}

inline void tm_sleep(uint32_t t)
{
    struct timespec tp;

    tp.tv_sec  = t / 1000;
    tp.tv_nsec = ( t % 1000 ) * 1000000;

    while( nanosleep(&tp, &tp) );
}

inline void tm_sleep_us(uint64_t t)
{
    struct timespec tp;

    tp.tv_sec  = t / 1000000;
    tp.tv_nsec = ( t % 1000000 ) * 1000;

    while( nanosleep(&tp, &tp) );
}


} // end of namespace pi


#endif // end of PIL_OS_FAMILY_WINDOWS


#endif // end of __TIME_H__


