#ifndef THREAD_QUEUE_HPP
#define THREAD_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class threadQueue {
    public:
        threadQueue() = default;
        ~threadQueue() = default;

        threadQueue(const threadQueue&) = delete;
        threadQueue& operator=(const threadQueue&) = delete;

        void push(T item) {
            {
                std::lock_guard<std::mutex> lock(mut);
                dataQueue.push(std::move(item));
            }
            dataCond.notify_one();
            
        }

        void waitAndPop(T& item) {
            std::unique_lock<std::mutex> lock(mut);
            dataCond.wait(lock, [this] { return !dataQueue.empty(); });

            item = std::move(dataQueue.front());
            dataQueue.pop();
        }

        bool empty() const {
            std::lock_guard<std::mutex> lock(mut);
            return dataQueue.empty();
        }

    private:
        std::queue<T> dataQueue;
        mutable std::mutex mut;
        std::condition_variable dataCond;
};

#endif