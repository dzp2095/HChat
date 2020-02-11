//
//  LogStream.hpp
//  HChat
//
//  Created by 邓志鹏 on 10/02/2020.
//  Copyright © 2020 邓志鹏. All rights reserved.
//

#ifndef LogStream_hpp
#define LogStream_hpp

#include <stdio.h>

#include "Noncopyable.hpp"
#include "FixedSizeBuffer.hpp"
#include <fmt/format.h>

namespace HChat{

class LogStream:Noncopyable<LogStream>{
    
public:
    using Buffer = detail::FixedSizeBuffer<detail::kSmallBuffer>;

    LogStream(){};
    ~LogStream()=default;
    
    LogStream& operator<<(const unsigned char* str) {
    return operator<<(reinterpret_cast<const char*>(str));
    }

    LogStream& operator<<(const std::string& v) {
    buffer.append(v.c_str(), v.size());
    return *this;
    }
    
    LogStream& operator<<(const char* str) {
      if (str)
        buffer.append(str, strlen(str));
      else
        buffer.append("(null)", 6);
      return *this;
    }
    
    LogStream& operator<<(bool a){
        buffer.append(a?"1":"0", 1);
        return *this;
    };
    
    LogStream& operator<<(short n)
    {
        return operator<<(fmt::format_int(n).c_str());
    }
    
    LogStream& operator<<(unsigned short n) {
        return operator<<(fmt::format_int(n).c_str());
    }
    
    LogStream& operator<<(int n)  {
        return operator<<(fmt::format_int(n).c_str());
    }
    
    LogStream& operator<<(unsigned int n)  {
        return operator<<(fmt::format_int(n).c_str());
    }
    
    LogStream& operator<<(long n)  {
        return operator<<(fmt::format_int(n).c_str());
    }
    
    LogStream& operator<<(unsigned long n) {
        return operator<<(fmt::format_int(n).c_str());
    }
    
    LogStream& operator<<(long long n) {
        return operator<<(fmt::format_int(n).c_str());
    }
    
    LogStream& operator<<(unsigned long long n) {
        return operator<<(fmt::format_int(n).c_str());
    }
    
    LogStream& operator<<(const void* p){
        return operator<<(fmt::format("{}", fmt::ptr(p)).c_str());
        return *this;
    }

    LogStream& operator<<(float n) {
    *this << static_cast<double>(n);
    return *this;
    }
    LogStream& operator<<(double n){
        return operator<<(fmt::format("{:-f}", n).c_str());
    }
    
    LogStream& operator<<(long double n){
        return operator<<(fmt::format("{:-f}", n).c_str());
    }
    
    LogStream& operator<<(char v)
    {
      buffer.append(&v, 1);
      return *this;
    }
    
    const Buffer& get_buffer() const { return buffer; }
    void resetBuffer() { buffer.reset(); }


private:
    Buffer buffer;

};

}
#endif /* LogStream_hpp */
