//
//  Mutex.hpp
//  HChat
//
//  Created by 邓志鹏 on 29/01/2020.
//  Copyright © 2020 邓志鹏. All rights reserved.
//

#ifndef HCHAT_MUTEX_HPP
#define HCHAT_MUTEX_HPP

#include <stdio.h>
#include <pthread.h>
#include <assert.h>

namespace HChat
{

class MutexLock
{
public:
    MutexLock(const MutexLock&)=delete;
    MutexLock& operator=(const MutexLock&)=delete;
    MutexLock(MutexLock&&)=delete;
    MutexLock& operator=(MutexLock&&)=delete;
    
    MutexLock()
    {
        pthread_mutex_init(&mutex, nullptr);
    }
    ~MutexLock()
    {
        
        pthread_mutex_destroy(&mutex);
    }
    
    void lock()
    {
        pthread_mutex_lock(&mutex);
    }
    
    void unlock()
    {
        pthread_mutex_unlock(&mutex);
    }
    
private:
    pthread_mutex_t mutex;
};

class MutexLockGuard
{
public:
    MutexLock(const MutexLock&)=delete;
    MutexLock& operator=(const MutexLock&)=delete;
    MutexLock(MutexLock&&)=delete;
    MutexLock& operator=(MutexLock&&)=delete;
    
    explicit MutexLockGuard(MutexLock &mutex):mutex(mutex)
    {
        mutex.lock();
    }
    ~MutexLockGuard()
    {
        mutex.unlock();
    }
private:
    MutexLock& mutex;
};
}

#define MutexLockGuard(x) static_assert(false,"Missing mutex variable name!")

#endif /* HCHAT_MUTEX_HPP */
