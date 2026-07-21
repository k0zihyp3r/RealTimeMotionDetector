#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <vector>
#include <thread>
#include <memory>
#include <atomic>
#include "threadQueue.hpp"
#include "task.hpp"

class threadPool {
public:
    explicit threadPool(size_t numThreads = std::thread::hardware_concurrency());

    ~threadPool();

    threadPool(const threadPool&) = delete;
    threadPool& operator=(const threadPool&) = delete;

    void submit(std::unique_ptr<Task> task);

private:
    void workerLoop();

    std::vector<std::thread> workers;
    threadQueue<std::unique_ptr<Task>> taskQueue;
    std::atomic<bool> stop{false};
};

#endif