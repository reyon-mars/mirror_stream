#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <thread>

class thread_pool
{
private:
    std::vector<std::jthread> workers_;
    std::queue<std::function<void()>> tasks_;

    std::mutex queue_mtx_;
    std::condition_variable cv_;
    bool stop_ = false;

public:
    thread_pool(size_t thread_count = (std::thread::hardware_concurrency() * 2))
    {
        workers_.reserve(thread_count);
        for (size_t i = 0; i < thread_count; i++)
        {
            workers_.emplace_back([this]
                                  {
                while( true ){
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mtx_ );
                        this->cv_.wait(lock, [this] {
                            return this->stop_ || !this->tasks_.empty();
                        });

                        if( this->stop_ && this->tasks_.empty()){
                            return;
                        }
                        task = std::move( this->tasks_.front() );
                        this->tasks_.pop();
                    }
                    task();
                } });
        }
    }
};