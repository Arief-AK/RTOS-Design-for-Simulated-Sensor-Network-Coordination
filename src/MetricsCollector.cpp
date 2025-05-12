#include <MetricsCollector.hpp>

MetricsCollector::MetricsCollector(const std::string report_name, std::shared_ptr<Logger> logger, bool to_csv, bool to_json)
    : m_total_tasks{}, m_completed_tasks{}, m_incomplete_tasks{},
      m_deadline_miss_count{}, m_total_response_time{}, m_total_turnaround_time{},
      m_context_switch_count{}, m_cpu_idle_time{}, m_cpu_utilisation{},
      m_to_csv(to_csv), m_to_json(to_json), m_report_name(report_name),
      m_logger(std::move(logger)), m_loggers{}, m_task_list{}
{
    // Create CSV and JSON loggers if required
    if (m_to_csv) {
        m_loggers.push_back(std::make_shared<CSVLogger>());
    }

    if (m_to_json) {
        m_loggers.push_back(std::make_shared<JSONLogger>());
    }
}

MetricsCollector::~MetricsCollector(){
    m_loggers.clear();
    m_task_list.clear();
    m_logger.reset();
}

void MetricsCollector::analyseTaskCompletion(const std::vector<std::shared_ptr<TaskControlBlock>> &task_list, int current_time){
    m_total_tasks = task_list.size();

    auto cpu_utilisation = 0;

    for (const auto& task : task_list){
        if(task->status == TaskStatus::COMPLETED){
            m_completed_tasks++;

            auto response_time = task->start_time - task->arrival_time;
            auto turnaround_time = task->finish_time - task->arrival_time;

            m_total_response_time += response_time;
            m_total_turnaround_time += turnaround_time;
            cpu_utilisation += task->execution_time;

            if(task->finish_time > task->arrival_time + task->deadline){
                m_deadline_miss_count++;
            }

            // Append task to the list
            m_task_list.push_back(task);
        }
        else{
            m_incomplete_tasks++;
        }
    }

    // Calculate CPU Utilisation
    m_cpu_utilisation = static_cast<double>(cpu_utilisation) / current_time * 100.0;
}

void MetricsCollector::reset(){
    m_total_tasks = 0;
    m_completed_tasks = 0;
    m_deadline_miss_count = 0;
    m_total_response_time = 0;
    m_total_turnaround_time = 0;
    m_context_switch_count = 0;
    m_cpu_idle_time = 0;
    m_cpu_utilisation = 0;

    m_logger->log("MetricsCollector reset");
}

void MetricsCollector::printReport(bool to_file){
    if (m_completed_tasks > 0){
        std::ostringstream report;

        report << "\n\n************** Metrics Report: **************\n\n";

        report << "Timings:\n";
        report << "\tTotal Tasks: " << m_total_tasks << "\n";
        report << "\tCompleted Tasks: " << m_completed_tasks << "\n";
        report << "\tDeadline Miss Count: " << m_deadline_miss_count << "\n";
        report << "\tAverage Response Time: " << (m_total_response_time / m_completed_tasks) << "\n";
        report << "\tAverage Turnaround Time: " << (m_total_turnaround_time / m_completed_tasks) << "\n";

        report << "\nCPU Utilisation:\n";
        report << "\tContext Switch Count: " << m_context_switch_count << "\n";
        report << "\tCPU Idle Time: " << m_cpu_idle_time << "\n";
        report << "\tCPU Utilisation: " << m_cpu_utilisation << "%\n\n";

        report << "Incomplete Tasks: " << m_incomplete_tasks << "\n\n";

        report << "********************************************\n";

        // Output to the logger
        m_logger->log(report.str());

        if (to_file){
            m_logger->logToFile(report.str());
            m_logger->logToFile(report.str(), m_report_name);
        }

        if (m_to_csv){
            auto csv_logger = std::dynamic_pointer_cast<CSVLogger>(m_loggers[0]);
            if (csv_logger) {
                csv_logger->setTasks(m_task_list);
                csv_logger->exportToCSV(m_report_name);
            }
        }

        if (m_to_json){
            auto json_logger = std::dynamic_pointer_cast<JSONLogger>(m_loggers[1]);
            if (json_logger) {
                json_logger->setTasks(m_task_list);
                json_logger->exportToJSON(m_report_name);
            }
        }
        
    } else {
        m_logger->log("No tasks completed.");
        return;
    }    
}

std::vector<std::shared_ptr<TaskControlBlock>> MetricsCollector::getTaskList() const{
    return m_task_list;
}

void MetricsCollector::incrementContextSwitchCount(){
    m_context_switch_count++;
}

void MetricsCollector::incrementCpuIdleTime(){
    m_cpu_idle_time++;
}

int MetricsCollector::getCPUIdleTime() const{
    return m_cpu_idle_time;
}

int MetricsCollector::getContextSwitchCount() const{
    return m_context_switch_count;
}