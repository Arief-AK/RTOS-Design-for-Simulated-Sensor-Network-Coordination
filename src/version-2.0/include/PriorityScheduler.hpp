#ifndef PRIORITYSCHEDULER_HPP
#define PRIORITYSCHEDULER_HPP

#include <cstdint>
#include <Scheduler.hpp>

class PriorityScheduler : public Scheduler{
public:
    TaskControlBlock* select_next_task(
        std::vector<TaskControlBlock*>& task_list,
        uint8_t current_time
    ) override;

    const std::string& getName() const override;
    std::unique_ptr<Scheduler> clone() const override;

private:
    std::string m_name = "PriorityScheduler";
};

#endif // PRIORITYSCHEDULER_HPP
