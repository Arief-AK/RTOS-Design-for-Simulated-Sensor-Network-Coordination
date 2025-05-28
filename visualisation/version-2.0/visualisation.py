import os
from Loader import Loader
from Plotter import Plotter

# Get the absolute path to the directory containing this script
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
JSON_DIRECTORY = os.path.join(SCRIPT_DIR, '../../json/version-2.0')

KERNEL_TASK_STATISTICS = "Kernel_task_statistics"
METRICS_SUMMARY = "metrics_summary"

if __name__ == "__main__":
    loader = Loader()
    plotter = Plotter(f"{SCRIPT_DIR}/results")

    loader.read_json_data(f"{JSON_DIRECTORY}/{KERNEL_TASK_STATISTICS}.json", KERNEL_TASK_STATISTICS)
    loader.load_json_data(f"{JSON_DIRECTORY}/{METRICS_SUMMARY}.json", METRICS_SUMMARY)
    data = loader.get_data()

    if KERNEL_TASK_STATISTICS in data and METRICS_SUMMARY in data:
        kernel_data = data[KERNEL_TASK_STATISTICS]
        metrics_data = data[METRICS_SUMMARY]

        plotter.plot_execution_timeline(kernel_data)
        plotter.plot_response_time(metrics_data)
        plotter.plot_deadline_violations(metrics_data)
        plotter.plot_laxity(metrics_data)
        plotter.plot_criticality_aggregation(metrics_data)
