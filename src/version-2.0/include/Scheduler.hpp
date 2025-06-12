#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <vector>
#include <TaskControlBlock.hpp>

class Scheduler{
public:
    virtual TaskControlBlock* select_next_task(
        std::vector<TaskControlBlock*>& task_list,
        uint8_t current_time
    ) = 0;

    virtual ~Scheduler() = default;
    virtual const std::string& getName() const = 0;

private:
    std::string m_name;
};

#endif // SCHEDULER_HPP