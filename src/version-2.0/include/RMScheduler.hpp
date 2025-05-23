#ifndef RMSCHEDULER_HPP
#define RMSCHEDULER_HPP

#include <Scheduler.hpp>

class RMScheduler : public Scheduler{
public:
    TaskControlBlock* select_next_task(
        std::vector<TaskControlBlock*>& task_list,
        u_int8_t current_time
    ) override;
};

#endif // RMSCHEDULER_HPP