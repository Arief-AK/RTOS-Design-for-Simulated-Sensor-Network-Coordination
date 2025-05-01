#include <Resource.hpp>

Resource::Resource(int id) : m_owner(nullptr), m_waiting_queue{}, m_resource_id(id){}

Resource::~Resource(){}

bool Resource::lock(std::shared_ptr<TaskControlBlock> task){
    auto flag = false;
    
    // Check owner
    if(!m_owner){
        // Resource is free, assign task as owner
        m_owner = task;
        flag = true;
    } else if(m_owner == task){
        // Task is already the owner
        flag = true;
    }else{
        // Resource is busy, add task to waiting queue
        m_waiting_queue.push(task);
        task->status = TaskStatus::BLOCKED;

        if(task->dynamic_priority < m_owner->dynamic_priority){
            std::cout << "Priority Inheritance: " << m_owner->task_id
                      << " inherits priority from " << task->task_id
                      << " from task " << task->task_id
                      << std::endl;
            m_owner->dynamic_priority = task->dynamic_priority;
        }

        flag = false;
    }
    
    return flag;
}

void Resource::unlock(std::shared_ptr<TaskControlBlock> task){
    if(m_owner == task){

        // Restore owner originial priority
        m_owner->dynamic_priority = m_owner->base_priority;

        if(!m_waiting_queue.empty()){
            // Assign ownership to the next waiting task in queue
            m_owner = m_waiting_queue.front();
            m_owner->status = TaskStatus::PENDING;
            m_waiting_queue.pop();
        } else{
            // No more tasks waiting, release ownership
            m_owner = nullptr;
        }
    }
}

bool Resource::isLockedBy(std::shared_ptr<TaskControlBlock> task) const{
    return m_owner == task;
}

bool Resource::isFree() const{
    return m_owner == nullptr;
}
