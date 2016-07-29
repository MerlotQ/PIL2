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

#ifdef __cplusplus
extern "C" {
#include "lzfse.h"
}
#endif

#include "lzfse++.h"

namespace pi {
namespace compress {


int lzfse_encode(const std::vector<uint8_t>& inBuf, std::vector<uint8_t>& outBuf)
{
    size_t aux_allocated = lzfse_encode_scratch_size();
    size_t in_size = inBuf.size();
    size_t out_allocated = in_size;
    size_t out_size = 0;

    void *aux = aux_allocated ? malloc(aux_allocated) : 0;
    if( outBuf.size() < out_allocated ) outBuf.resize(out_allocated);

    const uint8_t *in = inBuf.data();
    uint8_t *out = outBuf.data();

    while(1) {
        out_size = lzfse_encode_buffer(out, out_allocated, in, in_size, aux);

        if( out_size == 0 ) {
            out_allocated <<= 1;
            outBuf.resize(out_allocated);
            out = outBuf.data();

            continue;
        }

        break;
    }

    outBuf.resize(out_size);

    if( aux ) free(aux);

    return 0;
}

int lzfse_decode(const std::vector<uint8_t>& inBuf, std::vector<uint8_t>& outBuf)
{
    size_t aux_allocated = lzfse_decode_scratch_size();
    size_t in_size = inBuf.size();
    size_t out_allocated = in_size*4;
    size_t out_size = 0;

    void *aux = aux_allocated ? malloc(aux_allocated) : 0;
    if( outBuf.size() < out_allocated ) outBuf.resize(out_allocated);

    const uint8_t *in = inBuf.data();
    uint8_t *out = outBuf.data();

    while(1) {
        out_size = lzfse_decode_buffer(out, out_allocated, in, in_size, aux);

        if( out_size == 0 || out_size == out_allocated ) {
            out_allocated <<= 1;
            outBuf.resize(out_allocated);
            out = outBuf.data();

            continue;
        }

        break;
    }

    outBuf.resize(out_size);

    if( aux ) free(aux);

    return 0;
}


int lzfse_encode(const uint8_t* inBuf, const size_t inSize, uint8_t** outBuf, size_t &outSize)
{
    size_t aux_allocated = lzfse_encode_scratch_size();
    size_t in_size = inSize;
    size_t out_allocated = in_size;
    size_t out_size = 0;

    const uint8_t *in = inBuf;
    uint8_t *out = *outBuf;

    void *aux = aux_allocated ? malloc(aux_allocated) : 0;
    if( outSize < out_allocated ) {
        out = (uint8_t*) realloc(out, out_allocated);
        if( out == NULL ) return -1;
    }

    while(1) {
        out_size = lzfse_encode_buffer(out, out_allocated, in, in_size, aux);

        if( out_size == 0 ) {
            out_allocated <<= 1;
            out = (uint8_t*) realloc(out, out_allocated);
            if( out == NULL ) return -1;

            continue;
        }

        break;
    }

    *outBuf = out;
    outSize =  out_size;

    if( aux ) free(aux);

    return 0;
}

int lzfse_decode(const uint8_t* inBuf, const size_t inSize, uint8_t** outBuf, size_t& outSize)
{
    size_t aux_allocated = lzfse_decode_scratch_size();
    size_t in_size = inSize;
    size_t out_allocated = in_size*4;
    size_t out_size = 0;

    const uint8_t *in = inBuf;
    uint8_t *out = *outBuf;

    void *aux = aux_allocated ? malloc(aux_allocated) : 0;
    if( outSize < out_allocated ) {
        out = (uint8_t*) realloc(out, out_allocated);
        if( out == NULL ) return -1;
    }

    while(1) {
        out_size = lzfse_decode_buffer(out, out_allocated, in, in_size, aux);

        if( out_size == 0 || out_size == out_allocated ) {
            out_allocated <<= 1;
            out = (uint8_t*) realloc(out, out_allocated);
            if( out == NULL ) return -1;

            continue;
        }

        break;
    }

    *outBuf = out;
    outSize = out_size;

    if( aux ) free(aux);

    return 0;
}


}} // end of namespace pi::compress


