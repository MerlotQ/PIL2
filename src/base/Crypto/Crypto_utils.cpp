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

#include "Crypto_utils.h"


namespace pi {
namespace crypto {

std::string hex2str(const std::vector<uint8_t>& hex)
{
    static const char hexTable[] = "0123456789abcdef";

    std::string buf;
    const uint8_t *digest = hex.data();
    int N = hex.size();

    buf.resize(N*2);

    for(int i=0; i<N; i++) {
        buf[i*2+0] = hexTable[digest[i] >> 4];
        buf[i*2+1] = hexTable[digest[i] & 0x0f];
    }

    return buf;
}

std::vector<uint8_t> str2hex(const std::string& str)
{
    std::vector<uint8_t> hex;

    int n = str.size();
    if( n < 1 ) return hex;

    int k = n / 2;
    if( n % 2 ) k++;
    hex.resize(k, 0);

    for(int i=0; i<n; i++) {
        k = i / 2;

        uint8_t c = str[i], v;
        if( c >= '0' && c <= '9' )
            v = c - '0';
        else if( c >= 'a' && c <= 'f' )
            v = c - 'a' + 10;
        else if( c >= 'A' && c <= 'F' )
            v = c - 'A' + 10;
        else
            v = 0;

        if( i % 2 == 0 ) v = v << 4;
        hex[k] = hex[k] | v;
    }

    return hex;
}


std::string vec2str(const std::vector<uint8_t>& vec)
{
    std::string s;

    s = (char*) vec.data();
    /*
    int n = vec.size();

    int nZ = 0, nn;
    for(int i=n-1; i>=0; i--) {
        if( vec[i] ) break;
        nZ ++;
    }

    nn = n - nZ;
    s.resize(nn);
    for(int i=0; i<nn; i++)
        s[i] = vec[i];
    */

    return s;
}

std::vector<uint8_t> str2vec(const std::string& str)
{
    std::vector<uint8_t> v;

    v.resize(str.size());
    for(int i=0; i<str.size(); i++)
        v[i] = str[i];

    return v;
}


}} // end of namesapce pi::crypto


