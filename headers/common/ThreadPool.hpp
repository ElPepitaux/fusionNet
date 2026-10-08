#pragma once

#include "common/Platform.hpp"
#include "common/Logger.hpp"

#include <condition_variable>
#include <queue>

namespace fus::common
{
    class ThreadPool {
    public:
        ThreadPool(size_t numThreads);
        ~ThreadPool();

        void submit(std::function<void()> task);
        void stop();

    private:
        void _worker();

        std::vector<std::thread> _workers;
        std::queue<std::function<void()>> _tasks;
        std::mutex _queueMutex;
        std::condition_variable _condition;
        bool _stop = false;
    };
} // namespace fus::common