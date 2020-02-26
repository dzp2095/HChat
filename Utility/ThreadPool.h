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
    -> std::future<std::invoke_result_t<F,Args...>>;

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

ThreadPool::ThreadPool(size_t pool_size):pool_size(pool_size),stop(false),max_pool_size(0) {

    setMaxPoolSize();
    pool_size=std::min(pool_size, max_pool_size);
    for (int i = 0; i < pool_size; ++i) {
        threads.emplace_back(
                [this](){
                    while (true){
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(this->queue_mutex);
                            cond.wait(lock,[this]{ return this->stop.load() || !tasks.empty();});
                            if (stop.load() && tasks.empty())
                                return;
                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                        task();
                    }
                });
    }
}

ThreadPool::~ThreadPool() {

    stop.store(true);
    cond.notify_all();
    for (auto & thread:threads) {
        thread.join();
    }
}

template<class F, class... Args>
auto ThreadPool::submit(F &&f, Args &&... args)
-> std::future<std::invoke_result_t<F,Args...>>{

    using return_type = std::invoke_result_t<F,Args ...>;
    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();

    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        if (stop.load())
            throw std::runtime_error("The ThreadPool has stopped!");
        tasks.emplace([task]{(*task)();});
    }
    cond.notify_one();
    return res;
}


#endif //_THREADPOOL_H
