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

#ifndef __MD5_H__
#define __MD5_H__

#include <stdint.h>

#include <string>
#include <vector>

#include "Crypto_utils.h"

namespace pi {
namespace crypto {

int md5_file(const std::string& fname, std::vector<uint8_t>& md5_hex);

int md5_data(const std::vector<uint8_t>& data, std::vector<uint8_t>& md5_hex);
int md5_data(const std::string& msg, std::vector<uint8_t>& md5_hex);
int md5_data(const void *dat, int len, std::vector<uint8_t>& md5_hex);


}} // end of namespace pi::crypto

#endif // end of __MD5_H__
