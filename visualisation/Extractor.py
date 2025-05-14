from SimData import Metrics, TaskMetrics, SimData

class Extractor:
    def __init__(self, raw_data: any):
        self.name = "Data Extractor"
        self.raw_data = raw_data
        self.extracted_data : SimData

        self._extract()

    def _extract(self):
        sim_name = self.raw_data["SimulationName"]
        scheduler_name = self.raw_data["SchedulerName"]
        raw_metrics = self.raw_data["Metrics"]
        raw_tasks = self.raw_data["Tasks"]
        task_count = self.raw_data["TaskCount"]
        timestamp = self.raw_data["Timestamp"]

        # Initialise with Metrics data class
        metrics = Metrics()
        metrics._extract(raw_metrics)

        # Iterate through tasks
        task_list = []
        raw_tasks: list
        task: dict
        for task in raw_tasks:
            # Initialise with Tasks data class
            tasks_metric = TaskMetrics()
            tasks_metric._extract(task)
            task_list.append(tasks_metric)

        # Initialise with SimData class
        self.extracted_data = SimData(sim_name, scheduler_name, metrics, task_count, task_list, timestamp)
