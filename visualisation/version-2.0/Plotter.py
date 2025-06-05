import os
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib.figure as fig

class Plotter:
    def __init__(self, plot_dir:str):
        self.plot_dir = plot_dir
        self.palette = {"HARD":"red", "FIRM":"orange", "SOFT":"green"}
        
        sns.set_theme(style="whitegrid")
        os.makedirs(self.plot_dir, exist_ok=True)

    def _save(self, fig, name:str):
        filepath = os.path.join(self.plot_dir, f"{name}.png")
        fig.savefig(filepath, bbox_inches='tight')
        print(f"Plot saved to {filepath}")

    def plot_execution_timeline(self, data:pd.DataFrame)-> fig.Figure:
        fig, ax = plt.subplots(figsize=(10, 5))
        for idx, row in data.iterrows():
            ax.broken_barh(
                [(row.start_time, row.finish_time - row.start_time)],
                (idx - 0.4, 0.8),
                facecolors=self.palette.get(row.criticality, 'gray')
            )
            ax.set_yticks(range(len(data)))
            ax.set_yticklabels(data["task_id"])
            ax.set_xlabel("Tick")
            ax.set_title("Task Execution Timeline")
            ax.set_ylabel("Task ID")
            self._save(fig, "execution_timeline")

    def plot_response_time(self, data:pd.DataFrame)-> fig.Figure:
        fig = plt.figure(figsize=(6, 4))
        sns.barplot(data=data, x="task_id", y="response_time", hue='criticality', palette=self.palette)
        plt.title("Response Time per Task")
        plt.xlabel("Task ID")
        plt.ylabel("Response Time")
        self._save(fig, "response_time")

    def plot_deadline_violations(self, data:pd.DataFrame)-> fig.Figure:
        data["deadline_missed"] = data["lateness"] > 0
        fig = plt.figure(figsize=(6, 4))
        sns.barplot(data=data, x="task_id", y="lateness", hue="deadline_missed", palette=self.palette)
        plt.axhline(0, color="gray", linestyle="--")
        plt.title("Lateness per Task")
        plt.xlabel("Task ID")
        plt.ylabel("Lateness (in ticks)")
        self._save(fig, "lateness_per_task")

    def plot_laxity(self, data:pd.DataFrame)-> fig.Figure:
        fig = plt.figure(figsize=(6, 4))
        sns.barplot(data=data, x="task_id", y="laxity", hue='criticality', palette=self.palette)
        plt.title("Laxity per Task")
        plt.xlabel("Task ID")
        plt.ylabel("Laxity (in ticks)")
        self._save(fig, "laxity_per_task")

    def plot_criticality_aggregation(self, data:pd.DataFrame)-> fig.Figure:
        aggregate = data.groupby("criticality").agg({
            "response_time": "mean",
            "lateness": "mean",
            "laxity": "mean"
        }).reset_index()

        aggregate_melted = aggregate.melt(
            id_vars="criticality",
            var_name="Metric",
            value_name="Mean"
            )
        fig = plt.figure(figsize=(8, 6))
        sns.barplot(
            data=aggregate_melted,
            x="Metric",
            y="Mean",
            hue="criticality",
            palette=self.palette
            )
        plt.title("Aggregate Metrics by Task Criticality")
        self._save(fig, "criticality_aggregation")
        
    def plot_value_vs_criticality(self, data:pd.DataFrame)-> fig.Figure:
        fig = plt.figure(figsize=(8, 6))
        sns.boxplot(data=data, x="criticality", y="value", palette=self.palette, hue="criticality", legend=False)
        sns.stripplot(data=data, x="criticality", y="value", color="black", alpha=0.5)
        plt.title("Task Value by Criticality Level")
        plt.xlabel("Criticality")
        plt.ylabel("Task Value")
        self._save(fig, "value_vs_criticality")
        
    def plot_computation_vs_response(self, data:pd.DataFrame)-> fig.Figure:
        fig = plt.figure(figsize=(8, 6))
        sns.scatterplot(
            data=data, 
            x="computation_time", 
            y="response_time", 
            hue="criticality", 
            size="value",
            sizes=(50, 200),
            palette=self.palette
        )
        # Add a reference line for y=x (response time = computation time)
        max_val = max(data["computation_time"].max(), data["response_time"].max())
        plt.plot([0, max_val], [0, max_val], 'k--', alpha=0.5)
        plt.title("Computation Time vs. Response Time")
        plt.xlabel("Computation Time")
        plt.ylabel("Response Time")
        self._save(fig, "computation_vs_response")
        
    def plot_task_utilization(self, data:pd.DataFrame)-> fig.Figure:
        # Calculate execution time for each task
        data["execution_time"] = data["finish_time"] - data["start_time"]
        
        # Calculate total execution time of all tasks
        total_execution_time = data["execution_time"].sum()
        
        # Calculate utilization percentage
        data["utilization"] = (data["execution_time"] / total_execution_time) * 100
        
        fig = plt.figure(figsize=(8, 6))
        sns.barplot(data=data, x="task_id", y="utilization", hue="criticality", palette=self.palette)
        plt.title("Task Utilization (% of Total Execution Time)")
        plt.xlabel("Task ID")
        plt.ylabel("Utilization (%)")
        self._save(fig, "task_utilization")
        
    def plot_waiting_time(self, data:pd.DataFrame)-> fig.Figure:
        # Calculate waiting time (start_time - arrival_time)
        data["waiting_time"] = data["start_time"] - data["arrival_time"]
        
        fig = plt.figure(figsize=(8, 6))
        sns.barplot(data=data, x="task_id", y="waiting_time", hue="criticality", palette=self.palette)
        plt.title("Waiting Time per Task")
        plt.xlabel("Task ID")
        plt.ylabel("Waiting Time (ticks)")
        self._save(fig, "waiting_time")
        
    def plot_cumulative_completion(self, data:pd.DataFrame)-> fig.Figure:
        # Sort data by finish_time
        sorted_data = data.sort_values("finish_time")
        
        # Create cumulative completion count
        ticks = range(int(sorted_data["finish_time"].max()) + 1)
        cumulative_counts = [len(sorted_data[sorted_data["finish_time"] <= tick]) for tick in ticks]
        
        fig = plt.figure(figsize=(10, 6))
        plt.plot(ticks, cumulative_counts, marker='o')
        plt.title("Cumulative Task Completion Over Time")
        plt.xlabel("Time (ticks)")
        plt.ylabel("Number of Completed Tasks")
        plt.grid(True)
        self._save(fig, "cumulative_completion")
        
    def plot_arrival_pattern(self, data:pd.DataFrame)-> fig.Figure:
        # Count tasks arriving at each tick
        arrival_counts = data.groupby("arrival_time").size().reset_index(name="count")
        
        fig = plt.figure(figsize=(10, 6))
        sns.barplot(data=arrival_counts, x="arrival_time", y="count")
        plt.title("Task Arrival Pattern")
        plt.xlabel("Arrival Time (tick)")
        plt.ylabel("Number of Tasks")
        self._save(fig, "arrival_pattern")
        
    def plot_task_status_distribution(self, data:pd.DataFrame)-> fig.Figure:
        # Count tasks by status
        status_counts = data.groupby(["status", "criticality"]).size().reset_index(name="count")
        
        fig = plt.figure(figsize=(8, 6))
        sns.barplot(data=status_counts, x="status", y="count", hue="criticality", palette=self.palette)
        plt.title("Task Status Distribution by Criticality")
        plt.xlabel("Task Status")
        plt.ylabel("Count")
        self._save(fig, "task_status_distribution")
        
    def plot_context_switch_timeline(self, data:pd.DataFrame, task_data:pd.DataFrame=None)-> fig.Figure:
        # This method needs context switch data which might not be in your current JSON
        # If available, it would show when context switches happened
        if "context_switch_count" in data.columns:
            fig = plt.figure(figsize=(10, 6))
            
            # If we have detailed task data with timestamps
            if task_data is not None and not task_data.empty:
                # Sort by start_time to get execution order
                sorted_tasks = task_data.sort_values("start_time")
                
                # Detect context switches (changes in running task)
                prev_task = None
                switch_times = []
                
                for _, row in sorted_tasks.iterrows():
                    current_task = row["task_id"]
                    if prev_task is not None and current_task != prev_task:
                        switch_times.append(row["start_time"])
                    prev_task = current_task
                
                # Plot timeline with context switches
                plt.plot(range(int(task_data["finish_time"].max()) + 1), [0] * (int(task_data["finish_time"].max()) + 1), 'b-')
                for switch_time in switch_times:
                    plt.axvline(x=switch_time, color='r', linestyle='--', alpha=0.7)
                
                plt.title(f"Context Switch Timeline (Total: {len(switch_times)})")
                plt.xlabel("Time (ticks)")
                plt.yticks([])
                self._save(fig, "context_switch_timeline")
            else:
                # If we only have the count
                total_switches = data["context_switch_count"].iloc[0] if isinstance(data["context_switch_count"], pd.Series) else data["context_switch_count"]
                plt.text(0.5, 0.5, f"Total Context Switches: {total_switches}", 
                        horizontalalignment='center', verticalalignment='center', fontsize=20)
                plt.axis('off')
                self._save(fig, "context_switch_count")