#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <vector>
#include <TaskControlBlock.hpp>

class Scheduler{
public:
    // Select the next task to run based on the scheduling algorithm
    virtual TaskControlBlock* select_next_task(
        std::vector<TaskControlBlock*>& task_list,
        uint8_t current_time
    ) = 0;
    virtual ~Scheduler() = default;

    // Get the name of the scheduler
    virtual const std::string& getName() const = 0;

    // Clone the scheduler
    virtual std::unique_ptr<Scheduler> clone() const = 0;

private:
    std::string m_name;
};

#endif // SCHEDULER_HPP