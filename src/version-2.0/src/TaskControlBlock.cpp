#include <TaskControlBlock.hpp>

TaskControlBlock::TaskControlBlock(
    u_int8_t id, u_int8_t arrival, u_int8_t computation, u_int8_t deadline,
    TaskCriticality critical, u_int8_t value
) : task_id(id), arrival_time(arrival), computation_time(computation),
    abs_deadline(deadline), rel_deadline(deadline - arrival), 
    start_time(0), finish_time(0), response_time(0), value(value),
    tardiness(0), lateness(0), laxity(0), criticality(critical),
    status(TaskStatus::READY) {}

TaskControlBlock::~TaskControlBlock() {
    // Destructor implementation (if needed)
}

void TaskControlBlock::updateMetrics(u_int8_t current_time){
    // Update the metrics based on the current time
    start_time = current_time;
    finish_time = current_time + computation_time;
    response_time = finish_time - arrival_time;

    laxity = abs_deadline - current_time - computation_time;
    lateness = static_cast<int8_t>(current_time + computation_time - abs_deadline);
    tardiness = (lateness > 0) ? lateness : 0;
}

std::stringstream TaskControlBlock::get_stats() const {
    std::stringstream stream;

    stream 
        << "\nTask ID: " << static_cast<int>(task_id) << "\n"
        << "\tArrival Time: " << static_cast<int>(arrival_time) << "\n"
        << "\tComputation Time: " << static_cast<int>(computation_time) << "\n"
        << "\tAbsolute Deadline: " << static_cast<int>(abs_deadline) << "\n"
        << "\tRelative Deadline: " << static_cast<int>(rel_deadline) << "\n"
        << "\tStart Time: " << static_cast<int>(start_time) << "\n"
        << "\tFinish Time: " << static_cast<int>(finish_time) << "\n"
        << "\tResponse Time: " << static_cast<int>(response_time) << "\n"
        << "\tValue: " << static_cast<int>(value) << "\n"
        << "\tTardiness: " << static_cast<int>(tardiness) << "\n"
        << "\tLateness: " << static_cast<int>(lateness) << "\n"
        << "\tLaxity: " << static_cast<int>(laxity) << "\n"
        << "\tCriticality: "
        << (criticality == TaskCriticality::HARD ? "HARD" :
            criticality == TaskCriticality::FIRM ? "FIRM" : 
            criticality == TaskCriticality::SOFT ? "SOFT" : "") 
        << "\n\tStatus: "
        << (status == TaskStatus::READY ? "READY" :
            status == TaskStatus::RUNNING ? "RUNNING" : 
            status == TaskStatus::COMPLETED ? "COMPLETED" : "")
        << "\n-----------------";

    return stream;
}

u_int8_t TaskControlBlock::getTaskId() const { return task_id; }
u_int8_t TaskControlBlock::getArrivalTime() const { return arrival_time; }
u_int8_t TaskControlBlock::getComputationTime() const { return computation_time; }
u_int8_t TaskControlBlock::getAbsDeadline() const { return abs_deadline; }
u_int8_t TaskControlBlock::getRelDeadline() const { return rel_deadline; }

TaskStatus TaskControlBlock::getStatus() const { return status; }
void TaskControlBlock::setStatus(TaskStatus new_status) { status = new_status; }
