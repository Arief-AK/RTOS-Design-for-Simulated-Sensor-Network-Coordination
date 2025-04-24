#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <memory>
#include <queue>
#include <TaskControlBlock.hpp>

class Resource{
public:
    Resource(int id);
    ~Resource();

    bool lock(std::shared_ptr<TaskControlBlock> task);
    void unlock(std::shared_ptr<TaskControlBlock> task);
    bool isLockedBy(std::shared_ptr<TaskControlBlock> task) const;
    bool isFree() const;

private:
    std::shared_ptr<TaskControlBlock> m_owner;
    std::queue<std::shared_ptr<TaskControlBlock>> m_waiting_queue;

    int m_resource_id;
};

#endif // RESOURCE_HPP