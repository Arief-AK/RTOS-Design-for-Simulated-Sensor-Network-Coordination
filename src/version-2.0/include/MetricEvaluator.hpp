#ifndef TASK_PERFORMANCE_EVALUATOR_HPP
#define TASK_PERFORMANCE_EVALUATOR_HPP

#include <iostream>

class TaskPerformanceEvaluator
{
public:
    TaskPerformanceEvaluator(const std::string& name, const std::string& description);
    ~TaskPerformanceEvaluator();

private:
    std::string m_name;             // Name of the metric evaluator
    std::string m_description;      // Description of the metric evaluator

    // Metric evaluation methods
    void evaluateResponseTime();
    void evaluateLateness();
    void evaluateTardiness();
    void evaluateLaxity();
    void evaluateCriticality();
    void evaluateValue();
    void evaluateTaskStatus();
};

#endif  // TASK_PERFORMANCE_EVALUATOR_HPP