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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>
#include <algorithm>

#include "Path.h"
#include "Path_Utils.h"

using namespace std;

namespace pi {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int path_exist(const std::string& p)
{
    return Path::pathExist(p);
}

int path_mkdir(const std::string& p)
{
    return Path::mkdir(p);
}

int path_remove(const std::string &p)
{
    return Path::rm(p);
}

int path_rename(const std::string& pOld, const std::string& pNew)
{
    return Path::rename(pOld, pNew);
}

int path_lsdir(const std::string& dir_name, pi::StringArray &dl)
{
    return Path::lsdir(dir_name, dl);
}

int path_isdir(const std::string &p)
{
    return Path::is_dir(p);
}

int path_isfile(const std::string &p)
{
    return Path::is_file(p);
}


int path_isAbsolute(const std::string& p)
{
    Path pa(p);

    return pa.isAbsolute();
}

int path_isRelative(const std::string& p)
{
    Path pa(p);

    return pa.isRelative();
}


std::string path_getAbsPath(const std::string& p)
{
    Path pa(p);

    return pa.absolute().toString();
}


std::string path_getHome(void)
{
    return Path::home();
}

std::string path_getPWD(void)
{
    return Path::current();
}

std::string path_getTemp(void)
{
    return Path::temp();
}

char path_getSeparator(void)
{
    return Path::separator();
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

StringArray path_split(const string &fname)
{
    size_t      found = -1;
    StringArray r;

    r.clear();

    /* find / or \ */
    found = fname.find_last_of("/\\");

    if( found == string::npos ) {
        r.push_back("");
        r.push_back(fname);
        return r;
    }

    // folder
    r.push_back(fname.substr(0, found));
    // file
    r.push_back(fname.substr(found+1));

    return r;
}

StringArray path_splitext(const string &fname)
{
    size_t      found;
    StringArray r, ff;

    r.clear();

    // get folder & file name
    ff = path_split(fname);

    // find .
    found = ff[1].find_last_of(".");
    if( found == string::npos ) {
        r.push_back(path_join(ff[0],ff[1]));
        r.push_back("");
        return r;
    }

    // filename
    r.push_back(path_join(ff[0],ff[1].substr(0, found)));
    // extname
    r.push_back(ff[1].substr(found));

    return r;
}

std::string path_extname(std::string &fname)
{
    size_t      found;
    string      ext_name = "";


    // find .
    found = fname.find_last_of(".");
    if( found != string::npos ) {
        ext_name = fname.substr(found);
    }

    return ext_name;
}

StringArray path_name_ext(const std::string &fname)
{
    StringArray path_file=path_split(fname);
    StringArray name_ext=path_splitext(path_file[1]);
    path_file[1]=name_ext[0];
    path_file.push_back(name_ext[1]);
    return path_file;
}

std::string path_join(const std::string &p1, const std::string &p2)
{
    string      p;
    int         l;

    p = p1;

    l = p.size();
    if( p[l-1] == '/' || p[l-1] == '\\' )
        p = p.substr(0, l-1);

    p = p + "/" + p2;
    return p;
}

std::string path_join(const std::string &p1, const std::string &p2, const std::string &p3)
{
    string      p;

    p = path_join(p1, p2);
    return path_join(p, p3);
}


std::string path_join(const StringArray &p)
{
    int     i, l;
    string  p_all;

    p_all = "";
    for(i=0; i<p.size(); i++) {
        l = p_all.size();
        if( l>0 ) {
            if( p_all[l-1] == '/' || p_all[l-1] == '\\' )
                p_all = p_all.substr(0, l-1);
        }

        p_all = p_all + "/" + p[i];
    }

    return p_all;
}

}//end of namespace pi

