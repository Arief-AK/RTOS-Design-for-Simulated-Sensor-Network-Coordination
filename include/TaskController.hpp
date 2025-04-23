#ifndef TASKCONTROLLER_HPP
#define TASKCONTROLLER_HPP

#include <iostream>
#include <memory>
#include <vector>

#include <Scheduler.hpp>
#include <TaskControlBlock.hpp>

class TaskController{
public:
    TaskController(/* args */);
    ~TaskController();

    void addTask(std::shared_ptr<TaskControlBlock> task);
    void organiseTasks(int current_time);
    void removeTask(int task_id);
    void displayTasks();

    std::vector<std::shared_ptr<TaskControlBlock>> getPendingTasks();
    std::vector<std::shared_ptr<TaskControlBlock>> getReadyTasks();
    std::vector<std::shared_ptr<TaskControlBlock>> getCompletedTasks();

private:
    std::vector<std::shared_ptr<TaskControlBlock>> m_pending_tasks;
    std::vector<std::shared_ptr<TaskControlBlock>> m_ready_tasks;
    std::vector<std::shared_ptr<TaskControlBlock>> m_completed_tasks;
};

#endif // TASKCONTROLLER_HPP