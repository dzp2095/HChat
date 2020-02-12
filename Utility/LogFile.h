// @Author Lin Ya
// @Email xxbbb@vip.qq.com
#pragma once
#include <memory>
#include <string>
#include "FileUtil.h"
#include "Noncopyable.hpp"
#include <mutex>

namespace HChat {

// TODO 自动归档功能
class LogFile : Noncopyable<LogFile> {
 public:
    
  LogFile(const std::string& basename, int flushEveryN = 1024);
  ~LogFile();

  void append(const char* logline, int len);
  void flush();
  bool rollFile();

 private:
  void append_unlocked(const char* logline, int len);

  const std::string basename_;
  const int flush_interval;

  int count_;
  std::mutex mutex_;
  std::unique_ptr<AppendFile> file_;
};

}

