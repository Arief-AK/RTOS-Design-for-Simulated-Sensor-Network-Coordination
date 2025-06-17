#include <TaskControlBlock.hpp>

TaskControlBlock::TaskControlBlock(
    uint8_t id, uint8_t arrival, uint8_t computation, uint8_t deadline,
    TaskCriticality critical, uint8_t value
) : task_id(id), arrival_time(arrival), computation_time(computation),
    abs_deadline(deadline), rel_deadline(deadline - arrival), 
    start_time(0), finish_time(0), response_time(0), value(value),
    tardiness(0), remaining_time(computation_time), priority(0),
    lateness(0), laxity(0), criticality(critical),
    status(TaskStatus::READY), behaviour(nullptr) {}

TaskControlBlock::~TaskControlBlock() {
    // Destructor implementation (if needed)
}

void TaskControlBlock::reset(){
    remaining_time = computation_time;
    start_time = 0;
    finish_time = 0;
    response_time = 0;
    lateness = 0;
    tardiness = 0;
    laxity = 0;
    status = TaskStatus::READY;
}

void TaskControlBlock::updateMetrics(uint8_t current_time){
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

uint8_t TaskControlBlock::getTaskId() const { return task_id; }
uint8_t TaskControlBlock::getArrivalTime() const { return arrival_time; }
uint8_t TaskControlBlock::getComputationTime() const { return computation_time; }
uint8_t TaskControlBlock::getAbsDeadline() const { return abs_deadline; }
uint8_t TaskControlBlock::getRelDeadline() const { return rel_deadline; }
uint8_t TaskControlBlock::getStartTime() const { return start_time; }
uint8_t TaskControlBlock::getFinishTime() const{ return finish_time; }
uint8_t TaskControlBlock::getResponseTime() const { return response_time; }
uint8_t TaskControlBlock::getValue() const { return value; }
uint8_t TaskControlBlock::getTardiness() const { return tardiness; }
uint8_t TaskControlBlock::getRemainingTime() const { return remaining_time; }
uint8_t TaskControlBlock::getPriority() const { return priority; }

int8_t TaskControlBlock::getLateness() const { return lateness; }
int8_t TaskControlBlock::getLaxity() const { return laxity; }
TaskStatus TaskControlBlock::getStatus() const { return status; }
TaskCriticality TaskControlBlock::getCriticality() const { return criticality; }

void TaskControlBlock::setPriority(uint8_t new_priority) { priority = new_priority; }
void TaskControlBlock::setStatus(TaskStatus new_status) { status = new_status; }
void TaskControlBlock::setStartTime(uint8_t time) { start_time = time; }
void TaskControlBlock::setResponseTime(uint8_t time) { response_time = time; }
void TaskControlBlock::setFinishTime(uint8_t time) { finish_time = time; }

bool TaskControlBlock::isCompleted() const{
    return remaining_time == 0;
}

bool TaskControlBlock::isRunning() const{
    return status == TaskStatus::RUNNING;
}

bool TaskControlBlock::isReady() const{
    return status == TaskStatus::READY;
}

void TaskControlBlock::bindBehaviour(std::shared_ptr<TaskBehaviour> behaviour_fn){
    behaviour = behaviour_fn;
}

void TaskControlBlock::execute(uint8_t current_time){
    if(behaviour){
        behaviour->run(this, current_time);
    } else {
        std::cerr << "No behaviour bound to TaskControlBlock with ID: " << static_cast<int>(task_id) << std::endl;
    }
}

void TaskControlBlock::run_tick(uint8_t current_time){
    if(remaining_time == 0) return;

    if(status == TaskStatus::READY){
        status = TaskStatus::RUNNING;

        start_time = current_time;
        response_time = current_time - arrival_time;
    }

    // Decrement the remaining time
    --remaining_time;

    if(remaining_time == 0){
        finish_time = current_time + 1;
        updateMetrics(finish_time);
        status = TaskStatus::COMPLETED;
    }
}

void TaskControlBlock::decrementRemainingTime(){
    if(remaining_time > 0)
        --remaining_time;
}

std::unique_ptr<TaskControlBlock> TaskControlBlock::clone() const {
    return std::make_unique<TaskControlBlock>(*this);
}
