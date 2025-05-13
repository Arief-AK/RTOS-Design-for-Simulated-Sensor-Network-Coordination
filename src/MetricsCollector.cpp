#include <MetricsCollector.hpp>

MetricsCollector::MetricsCollector(const std::string report_name, const std::string scheduler_name, std::shared_ptr<Logger> logger, bool to_csv, bool to_json)
    : m_to_csv(to_csv), m_to_json(to_json), m_metrics{},
      m_report_name(report_name), m_scheduler_name(scheduler_name), m_logger(std::move(logger)),
      m_loggers{}, m_task_list{}
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
    m_metrics.total_tasks = task_list.size();

    // Initialise cpu utilisation
    auto cpu_utilisation = 0;

    for (const auto& task : task_list){
        if(task->status == TaskStatus::COMPLETED){
            m_metrics.completed_tasks++;

            auto response_time = task->start_time - task->arrival_time;
            auto turnaround_time = task->finish_time - task->arrival_time;

            m_metrics.total_response_time += response_time;
            m_metrics.total_turnaround_time += turnaround_time;
            cpu_utilisation += task->execution_time;

            if(task->finish_time > task->arrival_time + task->deadline){
                m_metrics.deadline_miss_count++;
            }

            // Append task to the list
            m_task_list.push_back(task);
        }
        else{
            m_metrics.incomplete_tasks++;
        }
    }

    // Calculate CPU Utilisation
    m_metrics.cpu_utilisation = static_cast<double>(cpu_utilisation) / current_time * 100.0;
}

void MetricsCollector::reset(){
    m_metrics = Metrics{};
    m_logger->log("MetricsCollector reset");
}

void MetricsCollector::printReport(bool to_file){
    if (m_metrics.completed_tasks > 0){
        std::ostringstream report;

        report << "\n\n************** Metrics Report: **************\n\n";

        report << "Timings:\n";
        report << "\tTotal Tasks: " << m_metrics.total_tasks << "\n";
        report << "\tCompleted Tasks: " << m_metrics.completed_tasks << "\n";
        report << "\tDeadline Miss Count: " << m_metrics.deadline_miss_count << "\n";
        report << "\tAverage Response Time: " << (m_metrics.total_response_time / m_metrics.completed_tasks) << "\n";
        report << "\tAverage Turnaround Time: " << (m_metrics.total_turnaround_time / m_metrics.completed_tasks) << "\n";

        report << "\nCPU Utilisation:\n";
        report << "\tContext Switch Count: " << m_metrics.context_switch_count << "\n";
        report << "\tCPU Idle Time: " << m_metrics.cpu_idle_time << "\n";
        report << "\tCPU Utilisation: " << m_metrics.cpu_utilisation << "%\n\n";

        report << "Incomplete Tasks: " << m_metrics.incomplete_tasks << "\n\n";

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
                json_logger->exportToJSON(m_report_name, m_report_name, m_scheduler_name);
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
    m_metrics.context_switch_count++;
}

void MetricsCollector::incrementCpuIdleTime(){
    m_metrics.cpu_idle_time++;
}

int MetricsCollector::getCPUIdleTime() const{
    return m_metrics.cpu_idle_time;
}

int MetricsCollector::getContextSwitchCount() const{
    return m_metrics.context_switch_count;
}