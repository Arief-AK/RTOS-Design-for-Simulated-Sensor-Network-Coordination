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
    void removeTask(int task_id);
    void displayTasks();

    std::vector<std::shared_ptr<TaskControlBlock>> getTasks();

private:
    std::vector<std::shared_ptr<TaskControlBlock>> m_tasks;
};

#endif // TASKCONTROLLER_HPP