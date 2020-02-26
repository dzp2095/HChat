//
// Created by 邓志鹏 on 26/02/2020.
//

#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <future>
#include <atomic>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(size_t pool_size);
    ~ThreadPool();

    template <class F,class... Args>
    auto submit(F&& f,Args&&... args)
    -> std::future<typename std::invoke_result_t <F(Args...)>>;

private:
    void setMaxPoolSize(){
        size_t sz = std::thread::hardware_concurrency();
        max_pool_size = sz == 0 ? 5 : sz + 1;
    }
    const size_t pool_size;
    size_t max_pool_size;
    std::mutex queue_mutex;
    std::condition_variable cond;
    std::queue<std::function<void ()>> tasks;
    std::atomic<bool>   stop;
    std::vector<std::thread> threads;

};


#endif //_THREADPOOL_H
