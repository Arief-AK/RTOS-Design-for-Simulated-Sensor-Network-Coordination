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

    const std::string& getName() const override;

private:
    std::string m_name = "RMScheduler";
};

#endif // RMSCHEDULER_HPP