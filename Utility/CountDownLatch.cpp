//
//  CountDownLatch.cpp
//  HChat
//
//  Created by 邓志鹏 on 08/02/2020.
//  Copyright © 2020 邓志鹏. All rights reserved.
//

#include "CountDownLatch.hpp"

using namespace HChat;

CountDownLatch::CountDownLatch(int count):count_(count)
{
    
}

void CountDownLatch::wait()
{
    std::unique_lock<std::mutex> lock(mutex_);
    while (count_ > 0) { //prevent spurious awake
        condition_.wait(lock);
    }
}

void CountDownLatch::countDown(int n = 1)
{
    std::unique_lock<std::mutex> lock;
    count_-=n;
    if (count_==0)
    {
        condition_.notify_all();
    }
}

int CountDownLatch::getCount() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return count_;
}
