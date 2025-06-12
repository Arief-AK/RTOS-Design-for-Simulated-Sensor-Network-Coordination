#ifndef EDF_SCHEDULER_HPP
#define EDF_SCHEDULER_HPP

#include <Scheduler.hpp>
#include <limits>

class EDFScheduler : public Scheduler
{
public:
    EDFScheduler(/* args */);
    ~EDFScheduler();

    // Scheduler methods
    TaskControlBlock* selectTask(std::vector<std::shared_ptr<TaskControlBlock>> &ready_queue) override;
    std::string getName() const override;
};

#endif // EDF_SCHEDULER_HPP