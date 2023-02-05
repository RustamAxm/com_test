#pragma once

#include <mutex>
#include <queue>
#include <optional>


template <typename Type>
class TSQueue {
public:
    TSQueue() = default;
    TSQueue(const TSQueue<Type> & other) = delete;
    TSQueue& operator =(const TSQueue<Type> & other) = delete;

    virtual ~TSQueue() {}

    void push(Type item) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(item);
    }

    std::optional<Type> pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return std::nullopt;
        }
        Type tmp = queue_.front();
        queue_.pop();
        return tmp;
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }
private:
    std::queue<Type> queue_;
    mutable std::mutex mutex_;
};