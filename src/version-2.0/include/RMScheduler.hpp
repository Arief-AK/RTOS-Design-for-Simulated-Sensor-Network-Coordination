#ifndef RMSCHEDULER_HPP
#define RMSCHEDULER_HPP

#include <cstdint>
#include <Scheduler.hpp>

class RMScheduler : public Scheduler{
public:
    TaskControlBlock* select_next_task(
        std::vector<TaskControlBlock*>& task_list,
        uint8_t current_time
    ) override;
};

#endif // RMSCHEDULER_HPP