#ifndef PATH_UNIX_H
#define PATH_UNIX_H


#include <string>
#include <vector>

#include "../Environment.h"


namespace pi {

class PathImpl
{
public:
    static std::string currentImpl();
    static std::string homeImpl();
    static std::string tempImpl();
    static std::string nullImpl();
    static std::string expandImpl(const std::string& path);
    static void listRootsImpl(std::vector<std::string>& roots);

    static int  cmd(const std::string& commands);

    static bool pathExist(const std::string& path);

    static bool mkdir(const std::string& path);

    static bool rm(const std::string& path);

    static bool rename(const std::string& pOld, const std::string& pNew);

    static bool lsdir(const std::string& path,std::vector<std::string>& dl);

    static int is_file(const std::string& path);
    static int is_dir(const std::string& path);
};


} // namespace pi


#endif // PATH_UNIX_H
