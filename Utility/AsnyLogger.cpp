//
//  AsnyLogger.cpp
//  HChat
//
//  Created by 邓志鹏 on 12/02/2020.
//  Copyright © 2020 邓志鹏. All rights reserved.
//

#include "AsnyLogger.hpp"
#include <chrono>
#include "LogFile.h"
namespace HChat {

AsnyLogger::AsnyLogger(std::string  basename,int flush_interval=2)
    : log_file_name(std::move(basename)),
      flush_interval(flush_interval),
      running(false),
      latch(1){
    assert(!log_file_name.empty());
    current_buffer->setzero();
    next_buffer->setzero();
    buffers.reserve(16);
}

void AsnyLogger::append(const char *logline, size_t len) {
    std::lock_guard<std::mutex> lock(mutex_);
    if(current_buffer->avail()>len){
        current_buffer->append(logline,len);
    } else{
        //current_buffer release the management after move
        buffers.push_back(std::move(current_buffer));
        if (next_buffer)
            current_buffer=std::move(next_buffer);
        else
            //use make_unique rather than new
            current_buffer=std::make_unique<Buffer>(Buffer());
        current_buffer->append(logline,len);
        cond.notify_all();
    }
}

void  AsnyLogger::ThreadFunc(){
    latch.countDown(1);
    LogFile output(log_file_name);
    //move the time-consuming operation outside the Critical section
    BufferPtr new_buffer1 = std::make_unique<Buffer>(Buffer{});
    BufferPtr new_buffer2 = std::make_unique<Buffer>(Buffer{});
    new_buffer1->setzero();
    new_buffer2->setzero();
    BufferVector buffers_to_write;
    buffers_to_write.reserve(16);
    auto sec = std::chrono::seconds(1);

    while (running){
        {
            assert(new_buffer1 && new_buffer1->length() == 0);
            assert(new_buffer1 && new_buffer1->length() == 0);
            assert(buffers_to_write.empty());

            std::unique_lock<std::mutex> lock(mutex_);
            if (buffers.empty())
                cond.wait_for(lock,flush_interval*sec);
            buffers.push_back(std::move(current_buffer));
            current_buffer.swap(new_buffer1);
            buffers_to_write.swap(buffers);
            if(!next_buffer)
                next_buffer=std::move(new_buffer2);
        }

        //drop these logs to avoid trashing
        if (buffers_to_write.size()>25){
            buffers_to_write.erase(buffers_to_write.begin()+2,buffers.end());
        }

        assert(!buffers_to_write.empty());
        for (const auto& buffer:buffers_to_write){
            output.append(buffer->get_data(),buffer->length());
        }

        if(buffers_to_write.size()>2){
            buffers_to_write.resize(2);
        }

        if(!new_buffer1){
            assert(!buffers_to_write.empty());
            new_buffer1 = std::move(buffers_to_write.back());
            buffers_to_write.pop_back();
            new_buffer1->reset();
        }

        if(!new_buffer2){
            assert(!buffers_to_write.empty());
            new_buffer2 = std::move(buffers_to_write.back());
            buffers_to_write.pop_back();
            new_buffer2->reset();
        }


        buffers_to_write.clear();
        output.flush();
    }
    output.flush();
};

}