#include <CSVLogger.hpp>

CSVLogger::CSVLogger(std::shared_ptr<MetricsCollector> collector, const std::string& name)
    : m_collector(collector), m_logger_name(name), m_file_path("../csv/")
{
    std::filesystem::create_directories(m_file_path);
}

CSVLogger::~CSVLogger()
{
    if (m_file_stream.is_open()) {
        m_file_stream.close();
    }
}

void CSVLogger::log(const std::string& message){
    std::cout << m_logger_name << ": " << message << std::endl;
}

void CSVLogger::logToFile(const std::string& message){
    std::string full_path = m_file_path + m_logger_name + ".log";

    m_file_stream.open(full_path, std::ios::app);
    if (!m_file_stream.is_open()) {
        std::cerr << "Error opening file: " << full_path << std::endl;
        return;
    }

    m_file_stream << m_logger_name << ": " << message << std::endl;
    m_file_stream.close();
}

void CSVLogger::logToFile(const std::string& message, const std::string& filename){
    std::string full_path = filename + ".log";

    m_file_stream.open(full_path, std::ios::app);
    if (!m_file_stream.is_open()) {
        std::cerr << "Error opening file: " << full_path << std::endl;
        return;
    }

    m_file_stream << m_logger_name << ": " << message << std::endl;
    m_file_stream.close();
}

void CSVLogger::exportToCSV(const std::string &filename){
    std::string full_path = m_file_path + filename + ".csv";

    m_file_stream.open(full_path, std::ios::app);
    if (!m_file_stream.is_open()){
        std::cerr << "Error opening file: " << full_path << std::endl;
        return;
    }

    // Get metrics from collector
    auto task_list = m_collector->getTaskList();
    
    // Write headers
    m_file_stream << "TaskID,ArrivalTime,StartTime,FinishTime,ExecutionTime,Priority,Period,Deadline,Status"  << std::endl;

    // Write task details
    for (const auto& task : task_list){
        m_file_stream << task->task_id << ","
                      << task->arrival_time << ","
                      << task->start_time << ","
                      << task->finish_time << ","
                      << task->execution_time << ","
                      << task->priority << ","
                      << task->period << ","
                      << task->deadline << ","
                      << static_cast<int>(task->status) << std::endl;
    }

    m_file_stream.close();
    log("Exported metrics to CSV: " + full_path);
}

std::string CSVLogger::getLoggerName() const{
    return m_logger_name;
}