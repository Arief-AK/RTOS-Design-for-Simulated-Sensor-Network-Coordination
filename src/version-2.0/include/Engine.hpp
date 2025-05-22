#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <memory>
#include <vector>
#include <iostream>

#include <Logger.hpp>
#include <TaskControlBlock.hpp>

class Engine{
public:
    Engine(
        std::string engine_name,
        std::unique_ptr<Logger> logger,
        int max_ticks = 100
    );
    ~Engine();

    void run();

private:
    std::string m_engine_name;
    std::unique_ptr<Logger> m_logger;
    std::vector<std::shared_ptr<TaskControlBlock>> m_task_list;
    
    int m_current_time;
    int m_max_ticks;
};

#endif // ENGINE_HPP