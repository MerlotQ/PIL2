#ifndef LOGGER_H
#define LOGGER_H

#include "Assert.h"

#include <iostream>
#include <fstream>

#ifdef ENABLE_LOGGER
#define LOG(C) Logger::instance()<<__FUNCTION__<<" "<<C<<std::endl;\
    Logger::instance().flush();
#else
#define LOG(C)
#endif


class Logger
{
public:
    Logger();
    static std::ofstream& instance()
    {
        static std::ofstream ofs("log.txt");
        return ofs;
    }
};

#endif // LOGGER_H

