#include <Engine.hpp>

Engine::Engine(
    std::string engine_name,
    std::unique_ptr<Logger> logger,
    int max_ticks
): m_engine_name(engine_name),
    m_logger(std::move(logger)),
    m_task_list{},
    m_current_time(0),
    m_max_ticks(max_ticks)
{}

Engine::~Engine(){
    m_task_list.clear();
}

void Engine::run(){
    m_logger->log("\n\n********* " + m_engine_name + " Engine Started *********");

    while (m_current_time < m_max_ticks){
        m_logger->log("Tick: " + std::to_string(m_current_time));

        // Assign a scheduling algorithm to the task list
        // This is a placeholder for the actual scheduling algorithm
        // In a real scenario, you would have a scheduler that selects tasks based on their properties

        // Simulate task execution
        for (auto &task : m_task_list) {
            if (task->arrival_time <= m_current_time && task->computation_time > 0) {
                
                // Perform the task
                task->computation_time--;

                // Update finish time to current time
                task->finish_time = m_current_time;

                m_logger->log("Task ID: " + std::to_string(task->task_id) + " executed at tick: " + std::to_string(m_current_time));
            }
        }

        // Simulate a tick
        m_current_time++;
    }

    m_logger->log("\n********* " + m_engine_name + " Engine Finished *********");
}