//
//  AsnyLogger.hpp
//  HChat
//
//  Created by 邓志鹏 on 12/02/2020.
//  Copyright © 2020 邓志鹏. All rights reserved.
//

#ifndef AsnyLogger_hpp
#define AsnyLogger_hpp

#include <cstdio>
#include <thread>
#include "Noncopyable.hpp"
#include <condition_variable>
#include "CountDownLatch.hpp"
#include <string>
#include <utility>
#include "Singleton.hpp"
#include "FixedSizeBuffer.hpp"
#include <memory>
#include <vector>
#include <mutex>

namespace HChat {

class AsnyLogger:public Noncopyable<AsnyLogger> {

public:
    static AsnyLogger& instance(std::string  basename,int flush_interval=2){
        //thread-safe in C++11
        static AsnyLogger logger{std::move(basename),flush_interval};
        return logger;
    }
    void append(const char * logline,size_t len);
    
    void start(){
        thread_= std::thread{&AsnyLogger::ThreadFunc,this};
        running= true;
        //block untill the thread real start
        latch.wait();
    }
    
    void stop(){
        running = false;
        cond.notify_all();
        thread_.join();
    }
    
private:
    AsnyLogger(std::string  basename,int flush_interval);
    ~AsnyLogger(){
        if(running)
            stop();
    }
    void ThreadFunc();

    const std::string log_file_name;
    const int flush_interval;
    bool running;
    std::condition_variable cond;
    std::thread thread_;
    CountDownLatch latch;
    std::mutex mutex_;

    using Buffer=detail::FixedSizeBuffer<detail::kLargeBuffer> ;
    using BufferPtr=std::unique_ptr<Buffer> ;
    using BufferVector =  std::vector<BufferPtr> ;
    BufferPtr current_buffer;
    BufferPtr next_buffer;
    BufferVector buffers;
};

}

#endif /* AsnyLogger_hpp */
