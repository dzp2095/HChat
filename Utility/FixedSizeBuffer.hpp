//
//  FixedSizeBuffer.hpp
//  HChat
//
//  Created by 邓志鹏 on 11/02/2020.
//  Copyright © 2020 邓志鹏. All rights reserved.
//

#ifndef FixedSizeBuffer_hpp
#define FixedSizeBuffer_hpp

#include <stdio.h>
#include <algorithm>
namespace HChat{

namespace detail{

const int kSmallBuffer = 4000; //memory block is usually 4kB
const int kLargeBuffer = 4000*1000;

template<int SIZE>
class FixedSizeBuffer
{
public:
    FixedSizeBuffer():cur(buffer){};
    
    ~FixedSizeBuffer() = default;
    
    size_t avail() const {return end()-cur;};
    
    const char* get_data() const{return buffer;};
    
    size_t length() const{return cur-buffer;};
    
    const char* current(){return cur;}
    
    void reset(){cur=buffer;}

    void setzero() { memset(buffer, 0, sizeof buffer); }

    void append(const char* buf, size_t len)
    {
        if (avail() > len)
        {
            //sometimes perform even faster than memcpy
            std::copy(buf, buf+len, cur);
            cur+=len;
        }
    }
private:
    const char * end() const{return buffer+sizeof buffer;};
    char buffer[SIZE]{};
    char *cur;
};

}

}
#endif /* FixedSizeBuffer_hpp */
