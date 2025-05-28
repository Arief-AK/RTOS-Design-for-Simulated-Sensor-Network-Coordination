#ifndef TASK_BEHAVIOUR_HPP
#define TASK_BEHAVIOUR_HPP

// Forward declaration to avoid circular dependency
class TaskControlBlock;

class TaskBehaviour{
public:
    virtual void run(TaskControlBlock* task, uint8_t current_time) = 0;
    virtual ~TaskBehaviour() = default;
};

#endif // TASK_BEHAVIOUR_HPP