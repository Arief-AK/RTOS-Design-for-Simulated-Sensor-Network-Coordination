import os
import glob
import pandas as pd
import matplotlib.pyplot as plt
from Loader import Loader

# Directory containing JSON metrics summaries
dir_path = os.path.join(os.path.dirname(__file__), '../../json/version-2.0')

# Find all *_metrics_summary.json files
def find_metrics_files():
    pattern = os.path.join(dir_path, '*_metrics_summary.json')
    return glob.glob(pattern)

# Load kernel statistics for a given scheduler (context switches, total ticks, etc).
def load_kernel_stats(scheduler):
    kernel_file = os.path.join(dir_path, f'Kernel_{scheduler}_task_statistics.json')
    if not os.path.exists(kernel_file):
        return None
    import json
    with open(kernel_file, 'r') as f:
        data = json.load(f)
    # The last element is the summary
    if isinstance(data, list) and data and isinstance(data[-1], dict):
        return data[-1]
    return None

# Load all metrics into a DataFrame, and compute extra metrics
def load_all_metrics(files):
    loader = Loader()
    records = []
    for file in files:
        scheduler = os.path.basename(file).replace('_metrics_summary.json', '')
        if loader.load_json_data(file, scheduler):
            data = loader.get_data()[scheduler]
            # Compute extra metrics
            df = pd.DataFrame(data)
            # Deadline miss count
            deadline_miss_count = (df['finish_time'] > df['abs_deadline']).sum()
            # Average response time
            average_response_time = df['response_time'].mean()
            # Sum computation_time for CPU utilisation
            total_computation_time = df['computation_time'].sum()
            # Load kernel stats
            kernel_stats = load_kernel_stats(scheduler)
            if kernel_stats:
                context_switches = kernel_stats.get('context_switch_count', None)
                total_ticks = kernel_stats.get('total_ticks', None)
                cpu_utilisation = (total_computation_time / total_ticks) if total_ticks else None
            else:
                context_switches = None
                cpu_utilisation = None
            # Add metrics to a summary row
            summary = {
                'scheduler': scheduler,
                'deadline_miss_count': deadline_miss_count,
                'average_response_time': average_response_time,
                'context_switches': context_switches,
                'cpu_utilisation': cpu_utilisation
            }
            # Add means of other metrics
            for col in ['response_time', 'tardiness', 'lateness', 'laxity', 'value', 'finish_time', 'start_time']:
                if col in df.columns:
                    summary[col] = df[col].mean()
            records.append(summary)
    if records:
        return pd.DataFrame(records)
    else:
        return pd.DataFrame()

# Plot comparison bar chart for selected metrics
def plot_comparison(df, metrics, outdir):
    os.makedirs(outdir, exist_ok=True)
    for metric in metrics:
        plt.figure(figsize=(8,5))
        ax = df.groupby('scheduler')[metric].mean().plot(kind='bar', title=f'Scheduler Comparison: {metric}')
        plt.ylabel(metric)
        plt.tight_layout()
        outpath = os.path.join(outdir, f'scheduler_comparison_{metric}.png')
        plt.savefig(outpath)
        print(f"Saved: {outpath}")
        plt.close()

if __name__ == "__main__":
    files = find_metrics_files()
    if not files:
        print("No metrics summary files found.")
        exit(1)
    df = load_all_metrics(files)
    if df.empty:
        print("No data loaded from metrics summary files.")
        exit(1)
    print("Loaded metrics for schedulers:", df['scheduler'].unique())
    # Select metrics to compare (must match JSON keys)
    metrics = [
        'response_time', 'tardiness', 'lateness', 'laxity',
        'value', 'finish_time', 'start_time',
        'deadline_miss_count', 'average_response_time', 'context_switches', 'cpu_utilisation'
    ]
    metrics = [m for m in metrics if m in df.columns]
    summary = df.set_index('scheduler')[metrics]
    print("\n=== Scheduler Comparison Table (mean values) ===\n")
    print(summary)
    plot_dir = os.path.join(os.path.dirname(__file__), 'comparison_plots')
    plot_comparison(df, metrics, plot_dir)
    print(f"\nComparison plots saved in: {plot_dir}\n")
