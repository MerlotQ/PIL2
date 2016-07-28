

#ifndef Foundation_Path_WIN32_INCLUDED
#define Foundation_Path_WIN32_INCLUDED


#include "../Environment.h"
#include <vector>


namespace pi {


class PIL_API PathImpl
{
public:
	static std::string currentImpl();
	static std::string homeImpl();
	static std::string tempImpl();
	static std::string nullImpl();
	static std::string systemImpl();
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


#endif // Foundation_Path_WIN32_INCLUDED
