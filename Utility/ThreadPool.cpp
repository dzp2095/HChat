//
// Created by 邓志鹏 on 26/02/2020.
//

#include "ThreadPool.h"

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
    -> std::future<typename std::invoke_result_t <F(Args...)>>{

    using return_type = std::invoke_result_t<F(Args ...)>;
    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    auto res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        if (stop.load())
            throw std::runtime_error("The ThreadPool has stopped!");
        tasks.emplace([&task = task]{(*task)();});
    }
    cond.notify_one();
    return res;
}
