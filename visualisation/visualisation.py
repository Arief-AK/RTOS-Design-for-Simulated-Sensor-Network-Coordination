import json
# import pandas as pd
# import matplotlib as plt

from SimData import Metrics, SimData

JSON_FILE = "../json/main_controller.json"

# Extract the information
def extract_info(data) -> SimData:
    sim_name = data["SimulationName"]
    scheduler_name = data["SchedulerName"]
    raw_metrics = data["Metrics"]
    tasks = data["Tasks"]
    task_count = data["TaskCount"]
    timestamp = data["Timestamp"]

# Produce chart
def produce_chart():
    pass

# Plot metrics
def plot_metrics():
    pass

if __name__ == "__main__":
    print("\nVisualising results...")

    # Open the file
    with open(JSON_FILE) as json_file:
        data = json.load(json_file)
    
    # Extract data
    

    # Create the tasks chart

    # Create the metrics chart

    # Display