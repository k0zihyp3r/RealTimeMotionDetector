#ifndef TASK_HPP
#define TASK_HPP

#include <cstdint>
#include <string>

enum class Priority{
    low,
    medium,
    high,
    critical
};

class Task {
    public:
        Task(int taskId, Priority priority) : taskId(taskId), priority(priority){}

        virtual ~Task() = default;  

        Task(const Task& other) = delete;
        Task& operator=(const Task& other) = delete;

        virtual void execute() = 0;

        Priority getPriority() const { return priority; }
        int getId() const { return taskId; }
        
        bool operator<(const Task& other) const{
            return static_cast<int>(this->priority) < static_cast<int>(other.priority);
        }
    
    private:
        int taskId;
        Priority priority;
};

#endif