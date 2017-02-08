
#ifndef __UTILS_STR_H__
#define __UTILS_STR_H__

#include <stdint.h>
#include <string>
#include <vector>
#include <ostream>
#include <sstream>


namespace pi {


////////////////////////////////////////////////////////////////////////////////
/// string array type
////////////////////////////////////////////////////////////////////////////////
typedef std::vector<std::string> StringArray;


////////////////////////////////////////////////////////////////////////////////
/// string functions
////////////////////////////////////////////////////////////////////////////////

///
/// \brief split_text
///     split given string by delims
///
/// \param intext - input text
/// \param delims - delims
///
/// \return string array
///
/// \note
///
///
StringArray split_text(const std::string& intext, const std::string &delims);

///
/// \brief join_text
///     Join all string items to a single string
///
/// \param sa           - string array
/// \param delims       - seperator
///
/// \return joined string
///
/// \note
///     sa = ["hello world", "Test file"]
///     delims = " - "
///
///     otuput = "hello world - Test file"
///
std::string join_text(const StringArray& sa, const std::string& delims);

///
/// \brief merge two string together
///
/// \param s1           - string 1
/// \param s2           - string 2
///
/// \return
///     merged string
///
std::string join_text(const std::string& s1, const std::string& s2);

///
/// \brief split_line
///     split given string by line
///
/// \param intext - input text
/// \return string array
///
/// FIXME: blank line is not supported!
///
StringArray split_line(const std::string &intext);


///
/// \brief read_lines from file
///
/// \param fn           - file name
/// \param lns          - lines
/// \param buf_len      - default line buffer size
///
/// \return
///         0           - success
///
int read_lines(const std::string& fn, StringArray &lns, int buf_len=8196);


// string trim functions
std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);

// string lower & upper
std::string str_tolower(std::string &s);
std::string str_toupper(std::string &s);


std::string itos(const int &i);
std::string ftos(const float &f, int precision=6);
std::string dtos(const double &d, int precision=6);



int     str_to_int(const std::string &s);
float   str_to_float(const std::string &s);
double  str_to_double(const std::string &s);

inline std::string int_to_str(const int i) { return itos(i); }
inline std::string float_to_str(const float f) { return ftos(f); }
inline std::string double_to_str(const double d) { return dtos(d); }


template <typename T>
std::string to_str(const T& i)
{
    std::ostringstream ost;

    ost<<i;
    return ost.str();
}

std::string buf2HexStr(uint8_t* buf, size_t size);

} // end of namespace pi

#endif // end of __UTILS_STR_H__

