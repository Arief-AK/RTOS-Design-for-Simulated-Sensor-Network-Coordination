import json

from SimData import Metrics, SimData

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
        tasks = self.raw_data["Tasks"]
        task_count = self.raw_data["TaskCount"]
        timestamp = self.raw_data["Timestamp"]

