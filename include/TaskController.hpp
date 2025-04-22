#include <iostream>
#include <memory>
#include <vector>

#include <Scheduler.hpp>
#include <TaskControlBlock.hpp>

class TaskController{
public:
    TaskController(/* args */);
    ~TaskController();

    void setScheduler(std::shared_ptr<Scheduler> scheduler);

    void addTask(std::shared_ptr<TaskControlBlock> task);
    void removeTask(int task_id);
    void displayTasks();

    std::vector<std::shared_ptr<TaskControlBlock>> getTasks();

private:
    std::vector<std::shared_ptr<TaskControlBlock>> m_tasks;
    std::shared_ptr<Scheduler> m_scheduler;
};
