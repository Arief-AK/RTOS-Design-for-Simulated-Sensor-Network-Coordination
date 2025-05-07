#ifndef ROUNDROBIN_SCHEDULER_HPP
#define ROUNDROBIN_SCHEDULER_HPP

#include <Scheduler.hpp>

class RoundRobinScheduler: public Scheduler{
public:
    explicit RoundRobinScheduler(int time_quantum);

    TaskControlBlock* selectTask(std::vector<std::shared_ptr<TaskControlBlock>> &ready_queue) override;
    std::string getName() const override;

private:
    int m_time_quantum; // fixed time-slice
    size_t m_last_index;
    int m_current_quantum;  // tracks how long current task has run
    TaskControlBlock* m_current_task;  // tracks current running task
};

#endif // ROUND_ROBIN_SCHEDULER_HPP