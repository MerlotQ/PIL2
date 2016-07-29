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

#ifndef __LZFSE_PP_H__
#define __LZFSE_PP_H__

#include <stdio.h>
#include <stdint.h>
#include <vector>

namespace pi {
namespace compress {

int lzfse_encode(const std::vector<uint8_t>& inBuf, std::vector<uint8_t>& outBuf);
int lzfse_decode(const std::vector<uint8_t>& inBuf, std::vector<uint8_t>& outBuf);

int lzfse_encode(const uint8_t* inBuf, const size_t inSize, uint8_t** outBuf, size_t& outSize);
int lzfse_decode(const uint8_t* inBuf, const size_t inSize, uint8_t** outBuf, size_t& outSize);


}} // end of namespace pi::compress

#endif // end of __LZFSE_PP_H__
