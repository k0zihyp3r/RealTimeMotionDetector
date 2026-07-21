#include "threadPool.hpp"

threadPool::threadPool(size_t numThreads) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&threadPool::workerLoop, this);
    }
}

void threadPool::submit(std::unique_ptr<Task> task) {
    if (!stop) {
        taskQueue.push(std::move(task));
    }
}

void threadPool::workerLoop() {
    while (!stop) {
        std::unique_ptr<Task> task;
        
        taskQueue.waitAndPop(task);

        if (task && !stop) {
            task->execute();
        }
    }
}

threadPool::~threadPool() {
    stop = true;
    for (size_t i = 0; i < workers.size(); ++i) {
        taskQueue.push(nullptr);
    }
    for (std::thread& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}