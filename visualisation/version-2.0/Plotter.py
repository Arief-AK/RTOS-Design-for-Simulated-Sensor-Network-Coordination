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