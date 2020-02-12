//
//  Logging.hpp
//  HChat
//
//  Created by 邓志鹏 on 08/02/2020.
//  Copyright © 2020 邓志鹏. All rights reserved.
//

#ifndef Logging_hpp
#define Logging_hpp


#include <cstdio>
#include <string>
#include "LogStream.hpp"

namespace HChat{

class Logger{

public:
    
    Logger(const char* filename,int line);
    //output the stream buffer when deconstruct
    ~Logger();
    LogStream &stream(){ return impl.stream; };
    static void setLogFileName(std::string fileName) { logFileName_ = fileName; }
    static std::string getLogFileName() { return logFileName_; }
    
    using OutputFunc = void(*) (const char* msg,size_t len);
    using FlushFunc = void(*) ();
    
    static void setOutput(OutputFunc);
    static void setFlush(FlushFunc);
//    static void setTimeZone(const TimeZone& tz);
private:
    static std::string logFileName_;
    class Impl
    {
    public:
        Impl(const char *fileName, int line);
        void formatTime();
        std::string basename;
        int line;
        LogStream stream;
    };
    Impl impl;
};
}

#define LOG Logger(__FILE__, __LINE__).stream()

#endif /* Logging_hpp */
