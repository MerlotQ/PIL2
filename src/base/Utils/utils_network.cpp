
#include "base/Thread/Pipe.h"
#include "base/Thread/Process.h"
#include "base/Regexp/RegularExpression.h"

#include "../Environment.h"
#include "utils_network.h"
#include "utils_str.h"

namespace pi {

int listMACs(std::vector<MACAddress>& macAddress)
{
    std::string res;
    char buffer[2048];
    int nBuf = 2047;

    macAddress.clear();

    // call system utils to get all configurations
    {
        std::string cmd;
        std::vector<std::string> args;

    #if PIL_OS_FAMILY_UNIX
        cmd = "ifconfig";
    #endif

    #if defined(PIL_OS_FAMILY_WINDOWS) || defined(PIL_MINGW)
        cmd = "ipconfig";
        args.push_back("/all");
    #endif

        pi::Pipe outPipe;
        pi::ProcessHandle ph = pi::Process::launch(cmd, args,
                                                   0, &outPipe, 0);
        ph.wait();

        while(1) {
            int n = outPipe.readBytes(buffer, nBuf);
            if( n == 0 ) break;

            buffer[n] = 0;
            res = res + buffer;
        }
    }

    // parse MAC address
    {
        RegularExpression re_mac("([0-9a-fA-F]{2}[:-]){5}[0-9a-fA-F]{2}");
        RegularExpression re_macFake("([0-9a-fA-F]{2}[:-]){6}[0-9a-fA-F]{2}");

        RegularExpression::Match m;
        int r;

        StringArray sa;
        sa = split_line(res);

        for(int i=0; i<sa.size(); i++) {
            std::string& s = sa[i];

            // determin fake MAC address
            r = re_macFake.match(s, 0, m);
            if( m.offset != std::string::npos && m.length > 0 ) continue;

            // find real MAC address
            r = re_mac.match(s, 0, m);
            if( m.offset != std::string::npos && m.length > 0 ) {
                memcpy(buffer, s.c_str() + m.offset, m.length);
                buffer[m.length] = 0;

                MACAddress mac;
                mac.fromString(buffer);
                macAddress.push_back(mac);
            }
        }
    }

    return 0;
}

} // end of namespace pi


