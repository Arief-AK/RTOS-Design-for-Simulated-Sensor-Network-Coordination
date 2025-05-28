#include <MetricsCollector.hpp>

MetricsCollector::MetricsCollector(std::unique_ptr<Logger> logger)
    :m_task_data{}, m_avg_response_time{}, m_avg_tardiness{}, m_deadline_miss_rate{},
    m_logger(logger ? std::move(logger) : std::make_unique<ConsoleLogger>("MetricsCollector", true)),
    m_json_logger(std::make_unique<JSONLogger>("MetricsCollector_JSON")) {}

MetricsCollector::~MetricsCollector() {}

void MetricsCollector::recordTaskMetrics(const TaskControlBlock &task){
    TaskMetrics metrics{
        task.getTaskId(),
        task.getArrivalTime(),
        task.getComputationTime(),
        task.getAbsDeadline(),
        task.getRelDeadline(),
        task.getStartTime(),
        task.getFinishTime(),
        task.getResponseTime(),
        task.getValue(),
        task.getTardiness(),
        task.getLateness(),
        task.getLaxity(),
        (task.getCriticality() == TaskCriticality::HARD ? "HARD" :
            task.getCriticality() == TaskCriticality::FIRM ? "FIRM" : 
            task.getCriticality() == TaskCriticality::SOFT ? "SOFT" : ""),
        (task.getStatus() == TaskStatus::READY ? "READY" :
            task.getStatus() == TaskStatus::RUNNING ? "RUNNING" : 
            task.getStatus() == TaskStatus::COMPLETED ? "COMPLETED" : "")
    };

    m_task_data.push_back(metrics);
}

void MetricsCollector::generateSummaryReport() const{
    for (const auto &task : m_task_data){
        std::stringstream ss;
        ss << "Task ID: " << static_cast<int>(task.task_id) << "\n"
           << "\tArrival Time: " << static_cast<int>(task.arrival_time) << "\n"
           << "\tComputation Time: " << static_cast<int>(task.computation_time) << "\n"
           << "\tAbsolute Deadline: " << static_cast<int>(task.abs_deadline) << "\n"
           << "\tRelative Deadline: " << static_cast<int>(task.rel_deadline) << "\n"
           << "\tStart Time: " << static_cast<int>(task.start_time) << "\n"
           << "\tFinish Time: " << static_cast<int>(task.finish_time) << "\n"
           << "\tResponse Time: " << static_cast<int>(task.response_time) << "\n"
           << "\tValue: " << static_cast<int>(task.value) << "\n"
           << "\tTardiness: " << static_cast<int>(task.tardiness) << "\n"
           << "\tLateness: " << static_cast<int>(task.lateness) << "\n"
           << "\tLaxity: " << static_cast<int>(task.laxity) << "\n"
           << "\tCriticality: " << task.criticality << "\n"
           << "\tStatus: " << task.status;

        m_logger->log(ss.str());
    }
}

void MetricsCollector::exportJSONSummary() const{
    auto root = nlohmann::json::array();

    for (const auto &task : m_task_data){
        root.push_back({
            {"task_id", task.task_id},
            {"arrival_time", task.arrival_time},
            {"computation_time", task.computation_time},
            {"abs_deadline", task.abs_deadline},
            {"rel_deadline", task.rel_deadline},
            {"start_time", task.start_time},
            {"finish_time", task.finish_time},
            {"response_time", task.response_time},
            {"value", task.value},
            {"tardiness", task.tardiness},
            {"lateness", task.lateness},
            {"laxity", task.laxity},
            {"criticality", task.criticality},
            {"status", task.status}
        });
    }

    m_json_logger->logStructure(root);
    m_json_logger->flushToFile("metrics_summary");
    m_json_logger->log("Metrics summary exported to JSON file.");
}