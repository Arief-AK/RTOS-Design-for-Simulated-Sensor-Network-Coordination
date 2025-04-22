#include <iostream>
#include <memory>
#include <vector>

#include <TaskControlBlock.hpp>

class TaskController{
public:
    TaskController(/* args */);
    ~TaskController();

    void addTask(std::shared_ptr<TaskControlBlock> task);
    void removeTask(int task_id);

    void displayTasks();

private:
    std::vector<std::shared_ptr<TaskControlBlock>> m_tasks;
};
