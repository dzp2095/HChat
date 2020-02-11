//
//  Logging.cpp
//  HChat
//
//  Created by 邓志鹏 on 08/02/2020.
//  Copyright © 2020 邓志鹏. All rights reserved.
//

#include "Logging.hpp"
#include <fmt/format.h>
#include <time.h>
#include <sys/time.h>
#include <thread>

namespace HChat
{

void defaultOutput(const char* msg, size_t len)
{
  size_t n = fwrite(msg, 1, len, stdout);
  //FIXME check n
  (void)n;
}

void defaultFlush()
{
  fflush(stdout);
}


Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush = defaultFlush;

Logger::Logger(const char* filename,int line):
    impl(filename, line)
{
    
}
Logger::~Logger()
{
    impl.stream << " -- " << impl.basename<<" : "<<impl.line<<"\n";
    const LogStream::Buffer& buf(stream().get_buffer());
    g_output(buf.get_data(), buf.length());
}


void Logger::setOutput(OutputFunc output){
    g_output=output;
}
void Logger::setFlush(FlushFunc flush){
    g_flush=flush;
}

void Logger::Impl::formatTime()
{
    struct timeval tv;
    time_t time;
    char str_t[26] = {0};
    gettimeofday (&tv, NULL);
    time = tv.tv_sec;
    struct tm* p_time = localtime(&time);
    strftime(str_t, 26, "%Y-%m-%d %H:%M:%S\t", p_time);
    stream << str_t;
}
Logger::Impl::Impl(const char *fileName, int line)
    : basename(fileName),
      line(line),
      stream()
{
    formatTime();
//    std::this_thread::get_id();
    
}

}
