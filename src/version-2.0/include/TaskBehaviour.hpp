#ifndef TASK_BEHAVIOUR_HPP
#define TASK_BEHAVIOUR_HPP

#include <cstdint>

// Forward declaration to avoid circular dependency
class TaskControlBlock;

class TaskBehaviour{
public:
    virtual ~TaskBehaviour() = default;
    virtual void run(TaskControlBlock* task, uint8_t current_time) = 0;    
};

#endif // TASK_BEHAVIOUR_HPP