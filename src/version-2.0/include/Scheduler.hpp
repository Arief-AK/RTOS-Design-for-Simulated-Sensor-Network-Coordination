#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <vector>
#include <TaskControlBlock.hpp>

class Scheduler{
public:
    virtual TaskControlBlock* select_next_task(
        std::vector<TaskControlBlock*>& task_list,
        u_int8_t current_time
    ) = 0;

    virtual ~Scheduler() = default;
};

#endif // SCHEDULER_HPP