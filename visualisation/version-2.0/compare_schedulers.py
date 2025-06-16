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

# Load all metrics into a DataFrame
def load_all_metrics(files):
    loader = Loader()
    records = []
    for file in files:
        # Scheduler name from filename
        scheduler = os.path.basename(file).replace('_metrics_summary.json', '')
        if loader.load_json_data(file, scheduler):
            data = loader.get_data()[scheduler]
            data['scheduler'] = scheduler
            records.append(data)
    if records:
        return pd.concat(records, ignore_index=True)
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
        'value', 'finish_time', 'start_time'
    ]
    # Only keep columns that exist
    metrics = [m for m in metrics if m in df.columns]
    # Print summary table
    summary = df.groupby('scheduler')[metrics].mean()
    print("\n=== Scheduler Comparison Table (mean values) ===\n")
    print(summary)
    # Plot
    plot_dir = os.path.join(os.path.dirname(__file__), 'comparison_plots')
    plot_comparison(df, metrics, plot_dir)
    print(f"\nComparison plots saved in: {plot_dir}\n")
