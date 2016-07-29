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


#ifndef __CRYPTO_UTILS_H__
#define __CRYPTO_UTILS_H__

#include <string>
#include <vector>

namespace pi {
namespace crypto {

std::string hex2str(const std::vector<uint8_t>& hex);
std::vector<uint8_t> str2hex(const std::string& str);

std::string vec2str(const std::vector<uint8_t>& vec);
std::vector<uint8_t> str2vec(const std::string& str);


}} // end of namesapce pi::crypto



#endif // end of __CRYPTO_UTILS_H__

