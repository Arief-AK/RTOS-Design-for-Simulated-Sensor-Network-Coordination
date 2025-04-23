#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <iostream>
#include <vector>
#include <memory>

#include <TaskControlBlock.hpp>

class Scheduler{
public:
    virtual ~Scheduler() = default;

    // Method to select a task to the scheduler
    virtual TaskControlBlock* selectTask(std::vector<std::shared_ptr<TaskControlBlock>> &ready_queue) = 0;

    virtual std::string getName() const = 0;
};

#endif // SCHEDULER_HPP