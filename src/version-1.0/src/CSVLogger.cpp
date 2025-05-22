#include <CSVLogger.hpp>

CSVLogger::CSVLogger(const std::string &name)
    : m_logger_name(name), m_file_path("../../../csv/"), m_task_list{}
{
    std::filesystem::create_directories(m_file_path);
}

CSVLogger::~CSVLogger()
{
    if (m_task_file_stream.is_open()) {
        m_task_file_stream.close();
    }

    if (m_metrics_file_stream.is_open()) {
        m_metrics_file_stream.close();
    }
}

void CSVLogger::setTasks(const std::vector<std::shared_ptr<TaskControlBlock>> &task_list){
    m_task_list = task_list;
}

void CSVLogger::log(const std::string& message){
    std::cout << m_logger_name << ": " << message << std::endl;
}

void CSVLogger::logToFile(const std::string& message){
    std::string full_path = m_file_path + m_logger_name + ".log";

    m_task_file_stream.open(full_path, std::ios::app);
    if (!m_task_file_stream.is_open()) {
        std::cerr << "Error opening file: " << full_path << std::endl;
        return;
    }

    m_task_file_stream << m_logger_name << ": " << message << std::endl;
    m_task_file_stream.close();
}

void CSVLogger::logToFile(const std::string& message, const std::string& filename){
    std::string full_path = filename + ".log";

    m_task_file_stream.open(full_path, std::ios::app);
    if (!m_task_file_stream.is_open()) {
        std::cerr << "Error opening file: " << full_path << std::endl;
        return;
    }

    m_task_file_stream << m_logger_name << ": " << message << std::endl;
    m_task_file_stream.close();
}

void CSVLogger::exportToCSV(const std::string& filename, const std::string& sim_name, const std::string& scheduler_name, const Metrics& metrics){
    // Export task and metrics information separately
    _exportTasksToCSV(filename, sim_name, scheduler_name);
    _exportMetricsToCSV(filename, sim_name, scheduler_name, metrics);
}

std::string CSVLogger::getLoggerName() const{
    return m_logger_name;
}

void CSVLogger::_exportTasksToCSV(const std::string &filename, const std::string &sim_name, const std::string &scheduler_name){
    std::string task_file_full_path = m_file_path + filename + "_" + sim_name + "_" + scheduler_name + "_tasks.csv";

    m_task_file_stream.open(task_file_full_path, std::ios::app);
    if (!m_task_file_stream.is_open()){
        std::cerr << "Error opening file: " << task_file_full_path << std::endl;
        return;
    }
  
    // Write headers
    m_task_file_stream << "TaskID,ArrivalTime,StartTime,FinishTime,ExecutionTime,Priority,Period,Deadline,Status,Timestamp"  << std::endl;

    // Write task details
    for (const auto& task : m_task_list){
        m_task_file_stream << task->task_id << ","
                      << task->arrival_time << ","
                      << task->start_time << ","
                      << task->finish_time << ","
                      << task->execution_time << ","
                      << task->priority << ","
                      << task->period << ","
                      << task->deadline << ","
                      << static_cast<int>(task->status) << std::endl;
    }

    // Write timestamp
    m_task_file_stream << _getCurrentSystemTime() << std::endl;
    m_task_file_stream.close();
    log("Exported tasks to CSV: " + task_file_full_path);
}

void CSVLogger::_exportMetricsToCSV(const std::string &filename, const std::string &sim_name, const std::string &scheduler_name, const Metrics &metrics){
    std::string metrics_file_full_path = m_file_path + filename + "_" + sim_name + "_" + scheduler_name + "_metrics.csv";

    m_metrics_file_stream.open(metrics_file_full_path, std::ios::app);
    if (!m_metrics_file_stream.is_open()){
        std::cerr << "Error opening file: " << metrics_file_full_path << std::endl;
        return;
    }

    // Write headers
    m_metrics_file_stream << "Metric,Value" << std::endl;
    m_metrics_file_stream << "TotalTasks" << "," << metrics.total_tasks << std::endl;
    m_metrics_file_stream << "CompletedTasks" << "," << metrics.completed_tasks << std::endl;
    m_metrics_file_stream << "IncompleteTasks" << "," << metrics.incomplete_tasks << std::endl;
    m_metrics_file_stream << "DeadlineMissCount" << "," << metrics.deadline_miss_count << std::endl;
    m_metrics_file_stream << "AverageResponseTime" << "," << ((metrics.completed_tasks > 0) ? (metrics.total_response_time / metrics.completed_tasks) : 0) << std::endl;
    m_metrics_file_stream << "AverageTurnaroundTime" << "," << ((metrics.completed_tasks > 0) ? (metrics.total_turnaround_time / metrics.completed_tasks) : 0) << std::endl;
    m_metrics_file_stream << "ContextSwitchCount" << "," << metrics.context_switch_count << std::endl;
    m_metrics_file_stream << "CPUIdleTime" << "," << metrics.cpu_idle_time << std::endl;
    m_metrics_file_stream << "CPUUtilisation" << "," << metrics.cpu_utilisation << std::endl;
    m_metrics_file_stream << "Timestamp" << "," << _getCurrentSystemTime() << std::endl;

    // Close the file
    m_metrics_file_stream.close();
    log("Exported metrics to CSV: " + metrics_file_full_path);
}

std::string CSVLogger::_getCurrentSystemTime(){
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buffer;

    #ifdef _WIN32
        localtime_s(&tm_buffer, &time);
    #else
        localtime_r(&time, &tm_buffer);
    #endif

    char time_buffer_char[32];
    std::strftime(time_buffer_char, sizeof(time_buffer_char), "%Y-%m-%d %H:%M:%S", &tm_buffer);

    return std::string(time_buffer_char);
}