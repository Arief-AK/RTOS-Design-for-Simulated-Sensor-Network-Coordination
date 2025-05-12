#include <JSONLogger.hpp>

JSONLogger::JSONLogger(const std::string &name)
    : m_logger_name(name), m_file_path("../json/"), m_task_list{}
{
    std::filesystem::create_directories(m_file_path);
}

JSONLogger::~JSONLogger()
{
    if (m_file_stream.is_open()) {
        m_file_stream.close();
    }
}

void JSONLogger::setTasks(const std::vector<std::shared_ptr<TaskControlBlock>> &task_list){
    m_task_list = task_list;
}

void JSONLogger::log(const std::string& message){
    std::cout << m_logger_name << ": " << message << std::endl;
}

void JSONLogger::logToFile(const std::string& message){
    std::string full_path = m_file_path + m_logger_name + ".log";

    m_file_stream.open(full_path, std::ios::app);
    if (!m_file_stream.is_open()) {
        std::cerr << "Error opening file: " << full_path << std::endl;
        return;
    }

    m_file_stream << m_logger_name << ": " << message << std::endl;
    m_file_stream.close();
}

void JSONLogger::logToFile(const std::string& message, const std::string& filename){
    std::string full_path = filename + ".log";

    m_file_stream.open(full_path, std::ios::app);
    if (!m_file_stream.is_open()) {
        std::cerr << "Error opening file: " << full_path << std::endl;
        return;
    }

    m_file_stream << m_logger_name << ": " << message << std::endl;
    m_file_stream.close();
}

void JSONLogger::exportToJSON(const std::string &filename){
    std::string full_path = m_file_path + filename + ".json";

    // Create a JSON array
    auto json_array = nlohmann::json::array();
    for (const auto& task : m_task_list) {
        nlohmann::json json_task;
        json_task["TaskID"] = task->task_id;
        json_task["ArrivalTime"] = task->arrival_time;
        json_task["StartTime"] = task->start_time;
        json_task["FinishTime"] = task->finish_time;
        json_task["ExecutionTime"] = task->execution_time;
        json_task["Priority"] = task->priority;
        json_task["Period"] = task->period;
        json_task["Deadline"] = task->deadline;
        json_task["Status"] = static_cast<int>(task->status);

        // Add the JSON object to the array
        json_array.push_back(json_task);
    }

    m_file_stream.open(full_path, std::ios::app);
    if (!m_file_stream.is_open()){
        std::cerr << "Error opening file: " << full_path << std::endl;
        return;
    }
    
    m_file_stream << std::setw(4) << json_array << std::endl;
    m_file_stream.close();
    log("Exported metrics to JSON: " + full_path);
}

std::string JSONLogger::getLoggerName() const {
    return m_logger_name;
}