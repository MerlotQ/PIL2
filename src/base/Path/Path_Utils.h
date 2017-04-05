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


#ifndef __FILE_PATH_H__
#define __FILE_PATH_H__

#include <string>
#include <vector>

#include "../Utils/utils_str.h"

namespace pi {


////////////////////////////////////////////////////////////////////////////////
/// file & path functions
////////////////////////////////////////////////////////////////////////////////

///
/// \brief check wheter given path is exist
///
/// \param p        - path
///
/// \return
///     0           - do not exist
///     1           - exist
///
int path_exist(const std::string& p);

///
/// \brief make a directory
///
/// \param p        - path
///
/// \return
///     0           - success
///     other       - failed
///
int path_mkdir(const std::string& p);

///
/// \brief path_delfile
///
/// \param p        - path
///
/// \return
///     0           - success
///     other       - failed
///
int path_remove(const std::string &p);

///
/// \brief rename a given path to new pathname
///
/// \param pOld     - old name
/// \param pNew     - new name
///
/// \return
///     0           - success
///     other       - failed
///
int path_rename(const std::string& pOld, const std::string& pNew);

///
/// \brief list contents in a given directory
///
/// \param dir_name - directory path
/// \param dl       - file list
///
/// \return
///     0           - success
///     other       - failed
///
int path_lsdir(const std::string& dir_name, pi::StringArray &dl);


int path_isdir(const std::string &p);
int path_isfile(const std::string &p);

int path_isAbsolute(const std::string& p);
int path_isRelative(const std::string& p);
std::string path_getAbsPath(const std::string& p);

std::string path_getHome(void);
std::string path_getPWD(void);
std::string path_getTemp(void);
std::string path_getAppSettings(void);


char path_getSeparator(void);


////////////////////////////////////////////////////////////////////////////////
/// split path & file name
////////////////////////////////////////////////////////////////////////////////

///
/// \brief path_split - split path & file name
///             if path is empty then return "" in the first item
///
/// \param fname - input full path/file name
///
/// \return two items string array, [0] is path, [1] is filename
///
///
StringArray path_split(const std::string &fname);

///
/// \brief path_splitext - split path/file base name and ext name
///
/// \param fname    - input path/file name
///
/// \return two items string array, [0] path/file base name, [1] ext name (include '.')
///
/// example:
///     input pathname: /home/aa/test.txt
///     output [0]: /home/aa/test
///            [1]: .txt
///
///
StringArray path_splitext(const std::string &fname);

///
/// \brief path_extname - get file ext name
///
/// \param fname - input file name
///
/// \return file ext name
///
StringArray path_name_ext(const std::string &fname);

std::string path_extname(std::string &fname);


std::string path_join(const std::string &p1, const std::string &p2);
std::string path_join(const std::string &p1, const std::string &p2, const std::string &p3);
std::string path_join(const StringArray &p);

} // end of namespace pi


#endif // __FILE_PATH_H__
