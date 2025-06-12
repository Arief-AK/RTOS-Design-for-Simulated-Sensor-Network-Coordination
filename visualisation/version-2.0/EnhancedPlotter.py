import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib.figure as fig
import numpy as np
from Plotter import Plotter

class EnhancedPlotter(Plotter):
    def __init__(self, plot_dir:str):
        super().__init__(plot_dir)
        # Additional color palettes for new visualizations
        self.extended_palette = {"HARD":"red", "FIRM":"orange", "SOFT":"green", 
                                "HIGH": "darkred", "MEDIUM": "darkorange", "LOW": "darkgreen"}
    
    def plot_priority_distribution(self, data:pd.DataFrame)-> fig.Figure:
        """
        Visualizes task priority distribution if available or infers priorities 
        based on criticality levels.
        """
        if "priority" in data.columns:
            # If priority is directly available in the data
            fig = plt.figure(figsize=(8, 6))
            sns.countplot(data=data, x="priority", hue="criticality", palette=self.palette)
            plt.title("Task Priority Distribution")
            plt.xlabel("Priority Level")
            plt.ylabel("Number of Tasks")
            self._save(fig, "priority_distribution")
        else:
            # Infer priority based on criticality (HARD > FIRM > SOFT)
            priority_map = {"HARD": "HIGH", "FIRM": "MEDIUM", "SOFT": "LOW"}
            data_copy = data.copy()
            data_copy["inferred_priority"] = data_copy["criticality"].map(priority_map)
            
            fig = plt.figure(figsize=(8, 6))
            sns.countplot(data=data_copy, x="inferred_priority", hue="criticality", palette=self.extended_palette)
            plt.title("Task Priority Distribution (Inferred from Criticality)")
            plt.xlabel("Inferred Priority Level")
            plt.ylabel("Number of Tasks")
            self._save(fig, "inferred_priority_distribution")
    
    def plot_cpu_utilization_over_time(self, data:pd.DataFrame)-> fig.Figure:
        """
        Visualizes CPU utilization over time based on task execution data.
        """
        # Filter out rows with NaN values in start_time or finish_time
        valid_data = data.dropna(subset=["start_time", "finish_time"])
        
        if valid_data.empty:
            print("Warning: No valid task execution data for CPU utilization plot")
            fig = plt.figure(figsize=(8, 6))
            plt.text(0.5, 0.5, "No valid task execution data available",
                    ha='center', va='center', fontsize=14)
            plt.title("CPU Utilization Over Time")
            self._save(fig, "cpu_utilization_over_time")
            return fig
        
        # Create a timeline of CPU utilization
        max_time = int(valid_data["finish_time"].max()) + 1
        timeline = np.zeros(max_time)
        
        # Mark each tick where CPU is utilized (a task is running)
        for _, task in valid_data.iterrows():
            if isinstance(task, pd.Series) and "start_time" in task and "finish_time" in task:
                # Check if values are valid numbers
                if pd.notna(task["start_time"]) and pd.notna(task["finish_time"]):
                    start = int(task["start_time"])
                    finish = int(task["finish_time"])
                    if start < finish:  # Ensure valid task execution times
                        for t in range(start, finish):
                            timeline[t] = 1
        
        # Calculate utilization percentage for each time window
        window_size = 5  # Adjust as needed
        utilization = []
        time_points = []
        
        for i in range(0, max_time, window_size):
            end = min(i + window_size, max_time)
            if end > i:
                window_util = np.mean(timeline[i:end]) * 100
                utilization.append(window_util)
                time_points.append(i)
        
        fig = plt.figure(figsize=(10, 6))
        plt.plot(time_points, utilization, marker='o', linestyle='-')
        plt.axhline(y=np.mean(timeline) * 100, color='r', linestyle='--', label=f'Average: {np.mean(timeline)*100:.1f}%')
        plt.title("CPU Utilization Over Time")
        plt.xlabel("Time (ticks)")
        plt.ylabel("Utilization (%)")
        plt.ylim(0, 105)  # Set y-axis limit to 0-105%
        plt.legend()
        plt.grid(True)
        self._save(fig, "cpu_utilization_over_time")
    
    def plot_task_completion_rate(self, data:pd.DataFrame)-> fig.Figure:
        """
        Visualizes task completion success rate by criticality.
        """
        # Calculate completion status
        completion_status = data.groupby(["criticality", "status"]).size().unstack(fill_value=0)
        
        # If 'COMPLETED' column doesn't exist, create it
        if 'COMPLETED' not in completion_status.columns:
            completion_status['COMPLETED'] = 0
            
        # Calculate total tasks per criticality
        total_tasks = completion_status.sum(axis=1)
        
        # Calculate completion rate
        completion_rates = pd.DataFrame({
            'completed': completion_status.get('COMPLETED', 0),
            'total': total_tasks,
            'rate': completion_status.get('COMPLETED', 0) / total_tasks * 100
        }).reset_index()
        
        fig = plt.figure(figsize=(8, 6))
        
        # Plot completion rate
        ax1 = plt.subplot(111)
        sns.barplot(data=completion_rates, x='criticality', y='rate', palette=self.palette, hue='criticality', legend=False, ax=ax1)
        ax1.set_ylabel('Completion Rate (%)')
        ax1.set_ylim(0, 105)
        
        # Add count labels
        for i, row in completion_rates.iterrows():
            ax1.text(i, row['rate'] + 2, f"{row['completed']}/{row['total']}", 
                    ha='center', va='bottom', color='black')
        
        plt.title("Task Completion Rate by Criticality")
        plt.tight_layout()
        self._save(fig, "task_completion_rate")
    
    def plot_periodicity_analysis(self, data:pd.DataFrame)-> fig.Figure:
        """
        Analyzes and visualizes task periodicity if present in the data.
        """
        # Check if we can identify periodic tasks
        if "task_id" in data.columns and "arrival_time" in data.columns:
            # Group by task_id to get all arrival times for each task
            task_arrivals = data.groupby("task_id")["arrival_time"].apply(list).reset_index()
            
            # Calculate inter-arrival times for tasks with multiple arrivals
            task_periods = []
            
            for _, row in task_arrivals.iterrows():
                arrivals = sorted(row["arrival_time"])
                if len(arrivals) > 1:
                    periods = [arrivals[i+1] - arrivals[i] for i in range(len(arrivals)-1)]
                    avg_period = np.mean(periods) if periods else 0
                    std_period = np.std(periods) if periods else 0
                    
                    task_periods.append({
                        "task_id": row["task_id"],
                        "avg_period": avg_period,
                        "std_period": std_period,
                        "is_periodic": std_period < 0.01 and avg_period > 0  # Simple heuristic
                    })
            
            if task_periods:
                periods_df = pd.DataFrame(task_periods)
                
                # Get task criticality
                task_criticality = data[["task_id", "criticality"]].drop_duplicates()
                periods_df = periods_df.merge(task_criticality, on="task_id", how="left")
                
                fig = plt.figure(figsize=(10, 6))
                
                # Plot average periods
                ax = sns.barplot(data=periods_df, x="task_id", y="avg_period", 
                                hue="criticality", palette=self.palette)
                
                # Mark periodic tasks
                for i, is_periodic in enumerate(periods_df["is_periodic"]):
                    color = 'green' if is_periodic else 'red'
                    marker = 'o' if is_periodic else 'x'
                    ax.plot(i, periods_df.iloc[i]["avg_period"], marker, color=color, 
                            markersize=10, markeredgewidth=2)
                
                plt.title("Task Periodicity Analysis")
                plt.xlabel("Task ID")
                plt.ylabel("Average Period (ticks)")
                plt.legend(title="Criticality")
                
                # Add a legend for periodicity markers using matplotlib
                plt.plot([], [], 'go', markersize=10, label='Periodic')
                plt.plot([], [], 'rx', markersize=10, label='Aperiodic/Sporadic')
                plt.legend(title="Task Type")
                
                self._save(fig, "task_periodicity")
    
    def plot_resource_blocking_analysis(self, data:pd.DataFrame)-> fig.Figure:
        """
        Visualizes resource blocking time if available in the data.
        """
        # Check if blocking time information is available
        if "blocking_time" in data.columns:
            fig = plt.figure(figsize=(8, 6))
            sns.barplot(data=data, x="task_id", y="blocking_time", hue="criticality", palette=self.palette)
            plt.title("Resource Blocking Time per Task")
            plt.xlabel("Task ID")
            plt.ylabel("Blocking Time (ticks)")
            self._save(fig, "resource_blocking_time")
        else:
            # Estimate blocking time from waiting time if available
            if "waiting_time" in data.columns and "computation_time" in data.columns and "response_time" in data.columns:
                # Blocking time can be estimated as: waiting_time - (response_time - computation_time - waiting_time)
                data_copy = data.copy()
                data_copy["estimated_blocking"] = data_copy["waiting_time"] - (
                    data_copy["response_time"] - data_copy["computation_time"] - data_copy["waiting_time"]
                )
                data_copy["estimated_blocking"] = data_copy["estimated_blocking"].clip(lower=0)  # Ensure non-negative
                
                fig = plt.figure(figsize=(8, 6))
                sns.barplot(data=data_copy, x="task_id", y="estimated_blocking", hue="criticality", palette=self.palette)
                plt.title("Estimated Resource Blocking Time per Task")
                plt.xlabel("Task ID")
                plt.ylabel("Estimated Blocking Time (ticks)")
                self._save(fig, "estimated_resource_blocking")
    
    def plot_scheduler_efficiency(self, data:pd.DataFrame)-> fig.Figure:
        """
        Visualizes scheduler efficiency metrics.
        """
        # Create metrics for scheduler efficiency
        metrics = []
        
        # Calculate average response time ratio (response_time / computation_time)
        if "response_time" in data.columns and "computation_time" in data.columns:
            avg_response_ratio = (data["response_time"] / data["computation_time"]).mean()
            metrics.append({"metric": "Avg Response Ratio", "value": avg_response_ratio})
        
        # Calculate deadline miss rate
        if "lateness" in data.columns:
            deadline_miss_rate = (data["lateness"] > 0).mean() * 100
            metrics.append({"metric": "Deadline Miss Rate (%)", "value": deadline_miss_rate})
        
        # Calculate average CPU utilization if we have execution data
        if "start_time" in data.columns and "finish_time" in data.columns:
            total_execution_time = sum(data["finish_time"] - data["start_time"])
            max_time = data["finish_time"].max()
            if max_time > 0:
                cpu_utilization = (total_execution_time / max_time) * 100
                metrics.append({"metric": "CPU Utilization (%)", "value": cpu_utilization})
        
        # Calculate context switch rate if available
        if "context_switch_count" in data.columns:
            # This is tricky since context_switch_count might be a single value
            # or it might be in a separate row/column
            if isinstance(data["context_switch_count"], pd.Series):
                context_switch_count = data["context_switch_count"].iloc[0]
            else:
                context_switch_count = data["context_switch_count"]
            
            if max_time > 0:
                context_switch_rate = context_switch_count / max_time
                metrics.append({"metric": "Context Switch Rate", "value": context_switch_rate})
        
        # Create the visualization if we have metrics
        if metrics:
            metrics_df = pd.DataFrame(metrics)
            
            fig = plt.figure(figsize=(10, 6))
            
            # Create a horizontal bar chart with log scale for better visualization
            # of metrics with different scales
            bars = plt.barh(metrics_df["metric"], metrics_df["value"], color='skyblue')
            
            # Add values to the end of each bar
            for i, bar in enumerate(bars):
                plt.text(bar.get_width() + 0.1, bar.get_y() + bar.get_height()/2, 
                         f'{metrics_df["value"].iloc[i]:.2f}', 
                         ha='left', va='center')
            
            plt.title("Scheduler Efficiency Metrics")
            plt.xlabel("Value")
            plt.tight_layout()
            self._save(fig, "scheduler_efficiency")
    
    def plot_task_makespan(self, data:pd.DataFrame)-> fig.Figure:
        """
        Visualizes the makespan (total time to complete all tasks) and critical path.
        """
        # Filter out rows with NaN values in start_time or finish_time
        valid_data = data.dropna(subset=["start_time", "finish_time"])
        
        if valid_data.empty:
            print("Warning: No valid task execution data for makespan plot")
            fig = plt.figure(figsize=(8, 6))
            plt.text(0.5, 0.5, "No valid task execution data available",
                    ha='center', va='center', fontsize=14)
            plt.title("Task Makespan")
            self._save(fig, "task_makespan")
            return fig
            
        if "start_time" in valid_data.columns and "finish_time" in valid_data.columns:
            # Calculate makespan
            makespan = valid_data["finish_time"].max() - valid_data["start_time"].min()
            
            # Sort tasks by start time
            sorted_tasks = valid_data.sort_values("start_time")
            
            fig = plt.figure(figsize=(12, 6))
            
            # Plot Gantt chart
            for i, task in sorted_tasks.iterrows():
                plt.barh(y=task["task_id"], 
                         left=task["start_time"], 
                         width=task["finish_time"] - task["start_time"],
                         color=self.palette.get(task["criticality"], 'gray'),
                         alpha=0.7,
                         label=f"Task {task['task_id']}")
                
                # Add task details as text
                plt.text(task["start_time"] + (task["finish_time"] - task["start_time"])/2, 
                         task["task_id"], 
                         f"Task {task['task_id']}", 
                         ha='center', va='center')
            
            # Remove duplicate labels
            handles, labels = plt.gca().get_legend_handles_labels()
            by_label = dict(zip(labels, handles))
            plt.legend(by_label.values(), by_label.keys(), title="Tasks")
            
            plt.title(f"Task Execution Timeline (Makespan: {makespan} ticks)")
            plt.xlabel("Time (ticks)")
            plt.ylabel("Task ID")
            plt.grid(axis='x', linestyle='--', alpha=0.7)
            
            # Add makespan annotation
            plt.axvline(x=valid_data["finish_time"].max(), color='red', linestyle='--', alpha=0.7)
            plt.text(valid_data["finish_time"].max(), plt.ylim()[0] - 0.5, 
                     f"Makespan: {makespan}", color='red', ha='right')
            
            self._save(fig, "task_makespan")
    
    def generate_comprehensive_report(self, metrics_data:pd.DataFrame, kernel_data:pd.DataFrame=None)-> fig.Figure:
        """
        Generates a comprehensive report with key metrics summary.
        """
        # Calculate key metrics
        metrics = {}
        
        # Task count by criticality
        criticality_counts = metrics_data["criticality"].value_counts().to_dict()
        metrics["Task Counts"] = criticality_counts
        
        # Average response times by criticality
        avg_response = metrics_data.groupby("criticality")["response_time"].mean().to_dict()
        metrics["Avg Response Time"] = avg_response
        
        # Deadline miss rates by criticality
        deadline_miss = metrics_data.groupby("criticality")["lateness"].apply(
            lambda x: (x > 0).mean() * 100
        ).to_dict()
        metrics["Deadline Miss Rate (%)"] = deadline_miss
        
        # Makespan
        if "finish_time" in metrics_data.columns:
            metrics["Makespan"] = metrics_data["finish_time"].max()
        
        # Context switches if available
        if kernel_data is not None and "context_switch_count" in kernel_data.columns:
            if isinstance(kernel_data["context_switch_count"], pd.Series):
                metrics["Context Switches"] = kernel_data["context_switch_count"].iloc[0]
            else:
                metrics["Context Switches"] = kernel_data["context_switch_count"]
        
        # Create a figure to display the report
        fig = plt.figure(figsize=(10, 8))
        plt.axis('off')  # Turn off the axes
        
        # Create report text
        report_text = "# RTOS Performance Summary Report\n\n"
        
        # Add task counts
        report_text += "## Task Distribution\n"
        for crit, count in metrics.get("Task Counts", {}).items():
            report_text += f"- {crit}: {count} tasks\n"
        
        # Add response times
        report_text += "\n## Average Response Times\n"
        for crit, rt in metrics.get("Avg Response Time", {}).items():
            report_text += f"- {crit}: {rt:.2f} ticks\n"
        
        # Add deadline miss rates
        report_text += "\n## Deadline Miss Rates\n"
        for crit, rate in metrics.get("Deadline Miss Rate (%)", {}).items():
            report_text += f"- {crit}: {rate:.2f}%\n"
        
        # Add makespan
        if "Makespan" in metrics:
            report_text += f"\n## Overall Performance\n"
            report_text += f"- Makespan: {metrics['Makespan']} ticks\n"
        
        # Add context switches
        if "Context Switches" in metrics:
            report_text += f"- Total Context Switches: {metrics['Context Switches']}\n"
        
        # Display the report
        plt.text(0.05, 0.95, report_text, fontsize=12, va='top', ha='left', 
                 transform=plt.gca().transAxes, family='monospace')
        
        self._save(fig, "performance_summary_report")
