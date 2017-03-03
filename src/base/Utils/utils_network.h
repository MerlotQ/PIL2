#ifndef __UTILS_NETWORK_H__
#define __UTILS_NETWORK_H__

#include <string.h>
#include <string>
#include <vector>

namespace pi {

/**
 * @brief The MAC address class
 */
class MACAddress
{
public:
    MACAddress() {}
    virtual ~MACAddress() {}

    std::string toString(void) {
        int n = address.size();

        char addr[64];
        memset(addr, 0, 64);

        for(int i=0; i<n; i++) {
            if( i != 0 )
                sprintf(addr, "%s:%02X", addr, address[i]);
            else
                sprintf(addr, "%s%02X",  addr, address[i]);
        }

        return addr;
    }

    std::string toStringCompact(void) {
        int n = address.size();

        char addr[64];
        memset(addr, 0, 64);

        for(int i=0; i<n; i++) {
            sprintf(addr, "%s%02X", addr, address[i]);
        }

        return addr;
    }

    int fromString(const std::string& add) {
        int n = add.size();
        unsigned char u, ua[8];
        int j = 0, k;

        memset(ua, 0, 8);

        for(int i=0; i<n; i++) {
            char c = add[i];

            if( c >= 'a' && c <= 'f' ) {
                u = c - 'a' + 10;
            } else if ( c >= 'A' && c <= 'F' ) {
                u = c - 'A' + 10;
            } else if ( c >= '0' && c <= '9' ) {
                u = c - '0';
            } else {
                continue;
            }

            k = j / 2;

            if( j % 2 == 0 )
                ua[k] = ua[k] | u<<4;
            else
                ua[k] = ua[k] | u;

            j++;

            if( j >= 12 ) break;
        }

        address.clear();
        for(int i=0; i<6; i++) address.push_back(ua[i]);

        return 0;
    }

    std::vector<unsigned char> getBinAddress(void) {
        return address;
    }

    int isValid(void) {
        if( address.size() < 6 ) return 0;

        int nz = 0;
        for(int i=0; i<address.size(); i++) {
            if( address[i] ) nz++;
        }
        if( nz < 2 ) return 0;

        return 1;
    }

    MACAddress& operator = (const std::vector<unsigned char>& a) {
        address = a;
        return *this;
    }


private:
    std::vector<unsigned char>      address;
};


/**
 * @brief list MAC address
 *
 * @param macAddress            - array of MAC addresses
 *
 * @return
 */
int listMACs(std::vector<MACAddress>& macAddress);


} // end of namespace pi

#endif // end of __UTILS_NETWORK_H__



