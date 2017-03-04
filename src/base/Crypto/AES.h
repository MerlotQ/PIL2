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

#ifndef __AES_H__
#define __AES_H__

#include <stdio.h>
#include <stdint.h>

#include <string>
#include <vector>


namespace pi {
namespace crypto {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef size_t                  AES_CONTEX;
typedef std::vector<uint8_t>    AES_KEY;

AES_CONTEX AES_setKey(AES_KEY& key);
AES_CONTEX AES_setKey(const std::string& key);
int AES_freeKey(AES_CONTEX ctx);

int AES_encodeMsg(AES_CONTEX ctx, const std::vector<uint8_t>& msgIn, std::vector<uint8_t>& msgOut);
int AES_decodeMsg(AES_CONTEX ctx, const std::vector<uint8_t>& msgIn, std::vector<uint8_t>& msgOut);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class AES
{
public:
    AES() { m_key = 0; }
    AES(AES_KEY& key) { m_key = AES_setKey(key); }
    virtual ~AES() { if( m_key ) AES_freeKey(m_key); m_key = 0; }

    int setKey(AES_KEY& key) {
        if( m_key ) AES_freeKey(m_key);

        m_key = AES_setKey(key);

        return 0;
    }

    int setKey(const std::string& key) {
        if( m_key ) AES_freeKey(m_key);

        m_key = AES_setKey(key);
    }

    int isKeySet(void) {
        return m_key;
    }

    int encode(const std::vector<uint8_t>& msgIn, std::vector<uint8_t>& msgOut) {
        if( m_key ) return AES_encodeMsg(m_key, msgIn, msgOut);
        else return -1;
    }

    int decode(const std::vector<uint8_t>& msgIn, std::vector<uint8_t>& msgOut) {
        if( m_key ) return AES_decodeMsg(m_key, msgIn, msgOut);
        else return -1;
    }

private:
    AES_CONTEX  m_key;
};

}} // end of namespace pi::crypto


#endif // end of __AES_H__
