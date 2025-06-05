#!/usr/bin/env python3
import os
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
    # Create both standard and enhanced plotters
    standard_plotter = Plotter(f"{SCRIPT_DIR}/results/standard")
    enhanced_plotter = EnhancedPlotter(f"{SCRIPT_DIR}/results/enhanced")

    # Load data from JSON files
    loader.read_json_data(f"{JSON_DIRECTORY}/{KERNEL_TASK_STATISTICS}.json", KERNEL_TASK_STATISTICS)
    loader.load_json_data(f"{JSON_DIRECTORY}/{METRICS_SUMMARY}.json", METRICS_SUMMARY)
    data = loader.get_data()

    if KERNEL_TASK_STATISTICS in data and METRICS_SUMMARY in data:
        kernel_data = data[KERNEL_TASK_STATISTICS]
        metrics_data = data[METRICS_SUMMARY]

        print("\nGenerating standard visualizations...")
        # Basic visualizations (existing)
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

        print("\nGenerating enhanced visualizations...")
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

        print("All visualizations have been generated successfully!")
