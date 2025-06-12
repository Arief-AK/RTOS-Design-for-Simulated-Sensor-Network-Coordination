#ifndef METRICS_HPP
#define METRICS_HPP

struct Metrics
{
    int total_tasks;
    int completed_tasks;
    int incomplete_tasks;
    int deadline_miss_count;
    int total_response_time;
    int total_turnaround_time;
    int context_switch_count;
    int cpu_idle_time;
    double cpu_utilisation;
};

#endif // METRICS_HPP