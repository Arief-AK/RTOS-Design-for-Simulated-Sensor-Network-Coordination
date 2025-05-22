#include <JSONLogger.hpp>

JSONLogger::JSONLogger(const std::string &name)
    : m_logger_name(name), m_file_path("../../../json/"), m_task_list{}
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

void JSONLogger::exportToJSON(const std::string &filename, const std::string& sim_name, const std::string& scheduler_name, const Metrics& metrics){
    std::string full_path = m_file_path + filename + ".json";

    // Create a JSON variables
    auto dataframe = nlohmann::json::object();
    auto json_array = nlohmann::json::array();
    nlohmann::json json_task;

    // Add simulation and scheduler name to the JSON object
    dataframe["SimulationName"] = sim_name;
    dataframe["SchedulerName"] = scheduler_name;

    // Iterate through the task list and create JSON objects
    for (const auto& task : m_task_list) {
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

    auto system_time = _getCurrentSystemTime();

    // Add the JSON array to the dataframe
    dataframe["Tasks"] = json_array;
    dataframe["TaskCount"] = json_array.size();

    // Add metric ditionary
    dataframe["Metrics"] = nlohmann::json::object();
    dataframe["Metrics"]["TotalTasks"] = metrics.total_tasks;
    dataframe["Metrics"]["CompletedTasks"] = metrics.completed_tasks;
    dataframe["Metrics"]["IncompleteTasks"] = metrics.incomplete_tasks;
    dataframe["Metrics"]["DeadlineMissCount"] = metrics.deadline_miss_count;
    dataframe["Metrics"]["AverageResponseTime"] = (metrics.completed_tasks > 0) ? (metrics.total_response_time / metrics.completed_tasks) : 0;
    dataframe["Metrics"]["AverageTurnaroundTime"] = (metrics.completed_tasks > 0) ? (metrics.total_turnaround_time / metrics.completed_tasks) : 0;
    dataframe["Metrics"]["ContextSwitchCount"] = metrics.context_switch_count;
    dataframe["Metrics"]["CPUIdleTime"] = metrics.cpu_idle_time;
    dataframe["Metrics"]["CPUUtilisation"] = metrics.cpu_utilisation;

    // Add the current system time to the dataframe
    dataframe["Timestamp"] = system_time;

    // Open the file and write the JSON data
    m_file_stream.open(full_path, std::ios::app);
    if (!m_file_stream.is_open()){
        std::cerr << "Error opening file: " << full_path << std::endl;
        return;
    }
    m_file_stream << std::setw(4) << dataframe << std::endl;
    m_file_stream.close();
    log("Exported metrics to JSON: " + full_path);
}

std::string JSONLogger::getLoggerName() const {
    return m_logger_name;
}

std::string JSONLogger::_getCurrentSystemTime(){
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
