from dataclasses import dataclass

@dataclass
class Metrics:
    avg_response_time: int
    avg_turnaround_time: int
    cpu_idle_time: int
    cpu_utilisation: float
    completed_tasks: int
    context_switch: int
    deadline_miss: int
    incomplete_tasks: int
    total_tasks: int

@dataclass
class SimData:
    name: str
    scheduler: str
    metrics: Metrics
    task_count: int
    tasks: list
    timestamp: str