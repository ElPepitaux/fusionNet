#include "common/ThreadPool.hpp"


fus::common::ThreadPool::ThreadPool(size_t threads)
{
    if (threads == 0)
        threads = 1;
    _workers.reserve(threads);
    for (size_t i = 0; i < threads; ++i)
        _workers.emplace_back(&ThreadPool::_worker, this);
}

fus::common::ThreadPool::~ThreadPool()
{
    stop();
}

void fus::common::ThreadPool::submit(std::function<void()> task)
{
    {
        std::lock_guard<std::mutex> lock(_queueMutex);
        if (_stop)
            return;
        _tasks.push(std::move(task));
    }
    _condition.notify_one();
}

void fus::common::ThreadPool::stop()
{
    {
        std::lock_guard<std::mutex> lock(_queueMutex);
        if (_stop)
            return;
        _stop = true;
    }
    _condition.notify_all();
    for (auto& worker : _workers) {
        if (worker.joinable())
            worker.join();
    }
}

void fus::common::ThreadPool::_worker()
{
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(_queueMutex);
            _condition.wait(lock, [this] { return _stop || !_tasks.empty(); });
            if (_stop && _tasks.empty())
                return;
            task = std::move(_tasks.front());
            _tasks.pop();
        }
        try {
            task();
        } catch (const std::exception& e) {
            fus::logging::StandardLogger::error("[ThreadPool] Task threw: " + std::string(e.what()));
        } catch (...) {
            fus::logging::StandardLogger::error("[ThreadPool] Task threw unknown exception");
        }
    }
}