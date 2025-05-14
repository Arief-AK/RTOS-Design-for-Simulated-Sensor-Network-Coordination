import json
import pandas as pd
import matplotlib.pyplot as plt
import glob
import os

from Extractor import Extractor, SimData

JSON_FILE = "json/main_controller.json"

# Extract the information
def extract_info(data) -> SimData:
    extractor = Extractor(data)
    return extractor.extracted_data

# Produce chart
def produce_chart(sim_data_list):
    """
    Accepts a list of SimData objects and produces bar charts comparing key metrics for each simulation.
    """

    # Prepare data for DataFrame
    records = []
    for sim in sim_data_list:
        records.append({
            'Simulation': sim.name,
            'Scheduler': sim.scheduler,
            'Avg Response Time': sim.metrics.avg_response_time,
            'Avg Turnaround Time': sim.metrics.avg_turnaround_time,
            'CPU Idle Time': sim.metrics.cpu_idle_time,
            'CPU Utilisation': sim.metrics.cpu_utilisation,
            'Completed Tasks': sim.metrics.completed_tasks,
            'Context Switch': sim.metrics.context_switch,
            'Deadline Miss': sim.metrics.deadline_miss,
            'Incomplete Tasks': sim.metrics.incomplete_tasks,
            'Total Tasks': sim.metrics.total_tasks
        })
    df = pd.DataFrame(records)
    df.set_index(['Simulation', 'Scheduler'], inplace=True)

    # Plot and save each metric separately (per scheduler)
    metrics_to_plot = [
        'Avg Response Time',
        'Avg Turnaround Time',
        'CPU Idle Time',
        'CPU Utilisation',
        'Completed Tasks',
        'Context Switch',
        'Deadline Miss',
        'Incomplete Tasks',
        'Total Tasks'
    ]
    for metric in metrics_to_plot:
        # For each unique scheduler, save plots in its own folder
        for scheduler in df.index.get_level_values('Scheduler').unique():
            scheduler_dir = os.path.join(os.path.dirname(__file__), 'visualisation', scheduler.replace(' ', ''))
            os.makedirs(scheduler_dir, exist_ok=True)
            # Filter for this scheduler
            df_sched = df.xs(scheduler, level='Scheduler')
            ax = df_sched[[metric]].plot(kind='bar', figsize=(10, 5), legend=False)
            plt.title(f'{metric} Comparison ({scheduler})')
            plt.ylabel(metric)
            plt.xlabel('Simulation')
            plt.xticks(rotation=45, ha='right')
            plt.tight_layout()
            # Add horizontal grid lines for y-axis intervals
            ax.yaxis.grid(True, linestyle='--', which='major', color='grey', alpha=0.7)
            save_path = os.path.join(scheduler_dir, f'{metric.replace(" ", "_").lower()}_comparison.png')
            plt.savefig(save_path)
            plt.close()

    # Additional: Comparison plot of schedulers for selected metrics
    comparison_metrics = [
        'Avg Response Time',
        'Avg Turnaround Time',
        'CPU Idle Time',
        'CPU Utilisation',
        'Total Tasks'
    ]
    # Group by Scheduler and calculate mean for each metric
    df_reset = df.reset_index()
    df_grouped = df_reset.groupby('Scheduler')[comparison_metrics].mean()
    comparison_dir = os.path.join(os.path.dirname(__file__), 'visualisation', 'comparison')
    os.makedirs(comparison_dir, exist_ok=True)
    ax = df_grouped.plot(kind='bar', figsize=(12, 6))
    plt.title('Scheduler Comparison on Key Metrics (Mean Values)')
    plt.ylabel('Mean Value')
    plt.xlabel('Scheduler')
    plt.xticks(rotation=45, ha='right')
    plt.tight_layout()
    save_path = os.path.join(comparison_dir, 'scheduler_comparison.png')
    plt.savefig(save_path)
    plt.close()

if __name__ == "__main__":
    print("\nVisualising results...")

    # Find all JSON files in the json/ directory
    json_dir = os.path.join(os.path.dirname(__file__), '../json')
    json_files = glob.glob(os.path.join(json_dir, '*.json'))

    sim_data_list = []
    for json_file in json_files:
        with open(json_file) as f:
            content = f.read().strip()
            # Try to parse as a single object or multiple objects
            try:
                # Try as a single JSON object
                data = json.loads(content)
                sim_data = extract_info(data)
                sim_data_list.append(sim_data)
            except json.JSONDecodeError:
                # Try to split into multiple objects
                objs = []
                # Try to split by '}{' and fix braces
                for obj in content.split('}\n{'):
                    if not obj.startswith('{'): obj = '{' + obj
                    if not obj.endswith('}'): obj = obj + '}'
                    try:
                        data = json.loads(obj)
                        sim_data = extract_info(data)
                        sim_data_list.append(sim_data)
                    except Exception as e:
                        print(f"Failed to parse object in {json_file}: {e}")
    print(f"Parsed {len(sim_data_list)} simulations.")
    if sim_data_list:
        produce_chart(sim_data_list)
    else:
        print("No simulation data found.")

    print("All visualisations are saved to visualisation directory!")