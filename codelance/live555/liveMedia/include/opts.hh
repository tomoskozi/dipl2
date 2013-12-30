#ifndef __OPTS_HH__
#define __OPTS_HH__

#include <cstdio>


struct cOpts
{
    const char* output;
    size_t dropRate;
    size_t repeatCnt;
    bool systematicOff;
    bool fbOff;
    bool repeatAfterOver;
    const char* srcAddr;
    const char* dstAddr;

    cOpts()
        : output("decoding.csv"), dropRate(10000000U), 
          repeatCnt(5U), systematicOff(false), fbOff(false),
          repeatAfterOver(true)
    {
    }
}; /* cOpts */

class Opts
{
public:
    static cOpts opts;
}; /* Opts */


#endif /* __OPTS_HH__ */

