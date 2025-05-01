#ifndef PRIORITY_SCHEDULER_HPP
#define PRIORITY_SCHEDULER_HPP

#include <Scheduler.hpp>

class PriorityScheduler : public Scheduler{
public:
    PriorityScheduler();
    ~PriorityScheduler();

    // Scheduler methods
    TaskControlBlock* selectTask(std::vector<std::shared_ptr<TaskControlBlock>> &ready_queue) override;
    std::string getName() const override;

private:
    size_t m_last_index;
};

#endif // PRIORITY_SCHEDULER_HPP