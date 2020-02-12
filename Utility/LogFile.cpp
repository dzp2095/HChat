// @Author Lin Ya
// @Email xxbbb@vip.qq.com
#include "LogFile.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "FileUtil.h"
#include <mutex>


namespace HChat{

LogFile::LogFile(const std::string& basename, int flush_interval)
    : basename_(basename),
      flush_interval(flush_interval),
      count_(0){
  // assert(basename.find('/') >= 0);
  file_.reset(new AppendFile(basename));
}

LogFile::~LogFile() {}

void LogFile::append(const char* logline, int len) {
  
  std::lock_guard<std::mutex> lock(mutex_);
  append_unlocked(logline, len);
}

void LogFile::flush() {
  std::lock_guard<std::mutex> lock(mutex_);
  file_-> flush();
}

void LogFile::append_unlocked(const char* logline, int len) {
  file_-> append(logline, len);
  ++count_;
  if (count_ >= flush_interval) {
    count_ = 0;
    file_->flush();
  }
}
}


