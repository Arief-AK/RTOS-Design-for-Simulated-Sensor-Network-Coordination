#!/usr/bin/env python3
import os
import glob
from Loader import Loader
from Plotter import Plotter
from EnhancedPlotter import EnhancedPlotter

# Get the absolute path to the directory containing this script
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
JSON_DIRECTORY = os.path.join(SCRIPT_DIR, '../../json/version-2.0')

KERNEL_TASK_STATISTICS = "Kernel_task_statistics"
METRICS_SUMMARY = "metrics_summary"

if __name__ == "__main__":
    loader = Loader()
    # Find all kernel statistics files for all schedulers
    kernel_stat_files = glob.glob(os.path.join(JSON_DIRECTORY, "Kernel_*_task_statistics.json"))
    schedulers = []
    for file in kernel_stat_files:
        # Extract scheduler name from filename
        basename = os.path.basename(file)
        # Format: Kernel_SCHEDULERNAME_task_statistics.json
        parts = basename.split("_")
        if len(parts) >= 3:
            scheduler_name = parts[1]
            schedulers.append((scheduler_name, file))

    if not schedulers:
        print("No kernel statistics files found for any scheduler.")
        exit(1)

    for scheduler_name, kernel_file in schedulers:
        metrics_file = os.path.join(JSON_DIRECTORY, f"{scheduler_name}_metrics_summary.json")
        if not os.path.exists(metrics_file):
            print(f"Warning: Metrics summary file not found for scheduler {scheduler_name}.")
            continue

        # Use unique keys for each scheduler's data
        kernel_key = f"Kernel_{scheduler_name}_task_statistics"
        metrics_key = f"{scheduler_name}_metrics_summary"
        loader.read_json_data(kernel_file, kernel_key)
        loader.load_json_data(metrics_file, metrics_key)
        data = loader.get_data()

        if kernel_key in data and metrics_key in data:
            kernel_data = data[kernel_key]
            metrics_data = data[metrics_key]

            print(f"\nGenerating standard visualizations for {scheduler_name}...")
            # Basic visualizations (existing)
            standard_plotter = Plotter(f"{SCRIPT_DIR}/results/standard/{scheduler_name}")
            enhanced_plotter = EnhancedPlotter(f"{SCRIPT_DIR}/results/enhanced/{scheduler_name}")
            standard_plotter.plot_execution_timeline(kernel_data)
            standard_plotter.plot_response_time(metrics_data)
            standard_plotter.plot_deadline_violations(metrics_data)
            standard_plotter.plot_laxity(metrics_data)
            standard_plotter.plot_criticality_aggregation(metrics_data)

            # Additional standard visualizations
            standard_plotter.plot_value_vs_criticality(metrics_data)
            standard_plotter.plot_computation_vs_response(metrics_data)
            standard_plotter.plot_task_utilization(kernel_data)
            standard_plotter.plot_waiting_time(metrics_data)
            standard_plotter.plot_cumulative_completion(metrics_data)
            standard_plotter.plot_arrival_pattern(metrics_data)
            standard_plotter.plot_task_status_distribution(metrics_data)

            # This plot may not work if context switch data is not available in the format expected
            try:
                context_switch_data = kernel_data.iloc[-1:] if 'context_switch_count' in kernel_data.columns else None
                if context_switch_data is not None:
                    standard_plotter.plot_context_switch_timeline(context_switch_data, kernel_data[:-1] if len(kernel_data) > 1 else None)
            except Exception as e:
                print(f"Warning: Could not generate context switch timeline: {e}")

            print(f"\nGenerating enhanced visualizations for {scheduler_name}...")
            # Generate enhanced visualizations
            try:
                enhanced_plotter.plot_priority_distribution(metrics_data)
            except Exception as e:
                print(f"Warning: Could not generate priority distribution plot: {e}")
                
            try:
                enhanced_plotter.plot_cpu_utilization_over_time(kernel_data)
            except Exception as e:
                print(f"Warning: Could not generate CPU utilization plot: {e}")
                
            try:
                enhanced_plotter.plot_task_completion_rate(metrics_data)
            except Exception as e:
                print(f"Warning: Could not generate task completion rate plot: {e}")
                
            try:
                enhanced_plotter.plot_periodicity_analysis(metrics_data)
            except Exception as e:
                print(f"Warning: Could not generate periodicity analysis plot: {e}")
                
            try:
                enhanced_plotter.plot_resource_blocking_analysis(metrics_data)
            except Exception as e:
                print(f"Warning: Could not generate resource blocking analysis plot: {e}")
                
            try:
                enhanced_plotter.plot_scheduler_efficiency(metrics_data)
            except Exception as e:
                print(f"Warning: Could not generate scheduler efficiency plot: {e}")
                
            try:
                enhanced_plotter.plot_task_makespan(kernel_data)
            except Exception as e:
                print(f"Warning: Could not generate task makespan plot: {e}")
                
            try:
                # Generate comprehensive report
                enhanced_plotter.generate_comprehensive_report(metrics_data, kernel_data)
            except Exception as e:
                print(f"Warning: Could not generate comprehensive report: {e}")

            print(f"All visualizations for {scheduler_name} have been generated successfully!")
        else:
            print(f"Warning: Data missing for scheduler {scheduler_name}.")
