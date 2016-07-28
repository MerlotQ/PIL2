
#ifdef PLATEFORM_INCLUDE_SOURCE

#include <stdlib.h>

#include "../Utils/Environment_WIN32.h"
#include "../Utils/utils_str.h"
#include "../Platform/UnWindows.h"
#include "../Debug/Exception.h"

#include "Path_WIN32.h"


namespace pi {

#ifndef PATH_MAX
#define PATH_MAX 1024 // fallback
#endif

std::string PathImpl::currentImpl()
{
	char buffer[MAX_PATH];
	DWORD n = GetCurrentDirectoryA(sizeof(buffer), buffer);
	if (n > 0 && n < sizeof(buffer))
	{
		std::string result(buffer, n);
		if (result[n - 1] != '\\')
			result.append("\\");
		return result;
	}
	else throw SystemException("Cannot get current directory");
}


std::string PathImpl::systemImpl()
{
	char buffer[MAX_PATH];
	DWORD n = GetSystemDirectoryA(buffer, sizeof(buffer));
	if (n > 0 && n < sizeof(buffer))
	{
		std::string result(buffer, n);
		if (result[n - 1] != '\\')
			result.append("\\");
		return result;
	}
	else throw SystemException("Cannot get system directory");
}


std::string PathImpl::homeImpl()
{
	std::string result;
	if (EnvironmentImpl::hasImpl("USERPROFILE"))
	{
		result = EnvironmentImpl::getImpl("USERPROFILE");
	}
	else if (EnvironmentImpl::hasImpl("HOMEDRIVE") && EnvironmentImpl::hasImpl("HOMEPATH"))
	{
		result = EnvironmentImpl::getImpl("HOMEDRIVE");
		result.append(EnvironmentImpl::getImpl("HOMEPATH"));
	}
	else
	{
		result = systemImpl();
	}

	std::string::size_type n = result.size();
	if (n > 0 && result[n - 1] != '\\')
		result.append("\\");
	return result;
}


std::string PathImpl::tempImpl()
{
	char buffer[MAX_PATH];
	DWORD n = GetTempPathA(sizeof(buffer), buffer);
	if (n > 0 && n < sizeof(buffer))
	{
		n = GetLongPathNameA(buffer, buffer, static_cast<DWORD>(sizeof buffer));
		if (n <= 0) throw SystemException("Cannot get temporary directory long path name");
		std::string result(buffer, n);
		if (result[n - 1] != '\\')
			result.append("\\");
		return result;
	}
	else throw SystemException("Cannot get temporary directory");
}


std::string PathImpl::nullImpl()
{
	return "NUL:";
}


std::string PathImpl::expandImpl(const std::string& path)
{
	char buffer[MAX_PATH];
	DWORD n = ExpandEnvironmentStringsA(path.c_str(), buffer, sizeof(buffer));
	if (n > 0 && n < sizeof(buffer))
		return std::string(buffer, n - 1);
	else
		return path;
}


void PathImpl::listRootsImpl(std::vector<std::string>& roots)
{
	roots.clear();
	char buffer[128];
	DWORD n = GetLogicalDriveStrings(sizeof(buffer) - 1, buffer);
	char* it = buffer;
	char* end = buffer + (n > sizeof(buffer) ? sizeof(buffer) : n);
	while (it < end)
	{
		std::string dev;
		while (it < end && *it) dev += *it++;
		roots.push_back(dev);
		++it;
	}
}

bool PathImpl::pathExist(const std::string& path)
{
    if( path.size() == 0 ) return false;

    DWORD attr = GetFileAttributes(_path.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        switch (GetLastError())
        {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
        case ERROR_NOT_READY:
        case ERROR_INVALID_DRIVE:
            return false;
        default:
            //handleLastErrorImpl(_path);
            return false;
        }
    }

    return true;
}


int PathImpl::cmd(const std::string& commands)
{
    return system(commands.c_str());
}

bool PathImpl::pathExist(const std::string& path)
{
    if( path.size() == 0 ) return false;

    DWORD attr = GetFileAttributes(_path.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        switch (GetLastError())
        {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
        case ERROR_NOT_READY:
        case ERROR_INVALID_DRIVE:
            return false;
        default:
            //handleLastErrorImpl(_path);
            return false;
        }
    }

    return true;
}


int _mkdir(const std::string& p)
{
    if ( CreateDirectoryA(p.c_str(), 0) )
        return 1;
    else
        return 0;
}

bool PathImpl::mkdir(const std::string& path)
{
    if( path.size() == 0 ) return false;

    StringArray sa = split_text(path, "/\\");
    int n = sa.size();
    if( n < 1 ) return false;

    // try to create dir
    std::string p = sa[0];
    for(int i=1; i<n; i++) {
        p = p + "\\" + sa[i];
        if( pathExist(p) )
            continue;
        else {
            if( !_mkdir(p) ) {
                return false;
            }
        }
    }

    return true;
}

bool PathImpl::rm(const std::string& path)
{
    if( path.size() == 0 ) return false;

    if ( RemoveDirectoryA(path.c_str()) == 0 ) return false;
    return true;
}

bool PathImpl::rename(const std::string& pOld, const std::string& pNew)
{
    if( pOld.size() == 0 || pNew.size() == 0 ) return false;

    if( MoveFileA(pOld.c_str(), pNew.c_str()) == 0 ) return false;
    return true;
}


bool PathImpl::lsdir(const std::string& path,std::vector<std::string>& dl)
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[2048];

    //Specify a file mask. *.* = We want everything!
    sprintf(sPath, "%s\\*.*", path.c_str());

    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) return false;

    do
    {
        //Find first file will always return "."
        //    and ".." as the first two directories.
        if(strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0)
        {
            //Build up our file path using the passed in
            //  [sDir] and the file/foldername we just found:
            dl.push_back(fdFile.cFileName);
        }
    }
    while(FindNextFile(hFind, &fdFile)); //Find the next file.

    FindClose(hFind); //Always, Always, clean things up!

    return true;
}

int PathImpl::is_file(const std::string& path)
{
    int r = is_dir(path);

    if( r < 0 )         return r;
    else if( r == 0 )   return 1;
    else                return 0;
}

int PathImpl::is_dir(const std::string& path)
{
    DWORD attr = GetFileAttributes(path.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
        return -1;

    if (attr & FILE_ATTRIBUTE_DIRECTORY)
        return 1;

    return 0;
}

} // namespace pi


#endif // end of PLATEFORM_INCLUDE_SOURCE
