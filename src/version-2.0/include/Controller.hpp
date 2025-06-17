#ifndef V2_CONTROLLER_HPP
#define V2_CONTROLLER_HPP

#include <memory>
#include <Kernel.hpp>
#include <CABuffer.hpp>
#include <CABuffer.tpp>
#include <ProducerTask.hpp>
#include <ConsumerTask.hpp>
#include <config.hpp>

#include <RMScheduler.hpp>
#include <EDFScheduler.hpp>
#include <PriorityScheduler.hpp>

#include <ExperimentRunner.hpp>

// Shared buffer for sensor data
extern CABuffer<int, BUFFER_SIZE> sensor_data_buffer;

class Controller{
public:
    Controller(/* args */);
    ~Controller() = default;

    void RunRMScheduler();
    void RunEDFScheduler();
    void RunPriorityScheduler();
    void RunComparativeExperiment();

private:
    ConsoleLogger m_logger;

    std::vector<std::unique_ptr<TaskControlBlock>> _createExperimentSampleTaskSet() const;
};

#endif // V2_CONTROLLER_HPP