from dataclasses import dataclass

@dataclass
class Metrics:
    avg_response_time: int = 0
    avg_turnaround_time: int = 0
    cpu_idle_time: int = 0
    cpu_utilisation: float = 0
    completed_tasks: int = 0
    context_switch: int = 0
    deadline_miss: int = 0
    incomplete_tasks: int = 0
    total_tasks: int = 0

    def _extract(self, raw_metrics: any):
        self.avg_response_time = raw_metrics["AverageResponseTime"]
        self.avg_turnaround_time = raw_metrics["AverageTurnaroundTime"]
        self.cpu_idle_time = raw_metrics["CPUIdleTime"]
        self.cpu_utilisation = raw_metrics["CPUUtilisation"]
        self.completed_tasks = raw_metrics["CompletedTasks"]
        self.context_switch = raw_metrics["ContextSwitchCount"]
        self.deadline_miss = raw_metrics["DeadlineMissCount"]
        self.incomplete_tasks = raw_metrics["IncompleteTasks"]
        self.total_tasks = raw_metrics["TotalTasks"]

@dataclass
class TaskMetrics:
    arrival_time: int = 0
    deadline: int = 0
    execution_time: int = 0
    finish_time: int = 0
    period: int = 0
    priority: int = 0
    start_time: int = 0
    status: int = 0
    task_id: int = 0

    def _extract(self, raw_metrics: any):
        self.arrival_time = raw_metrics["ArrivalTime"]
        self.deadline = raw_metrics["Deadline"]
        self.execution_time = raw_metrics["ExecutionTime"]
        self.finish_time = raw_metrics["FinishTime"]
        self.period = raw_metrics["Period"]
        self.priority = raw_metrics["Priority"]
        self.start_time = raw_metrics["StartTime"]
        self.status = raw_metrics["Status"]
        self.task_id = raw_metrics["TaskID"]

@dataclass
class SimData:
    name: str
    scheduler: str
    metrics: Metrics
    task_count: int
    tasks: list
    timestamp: str