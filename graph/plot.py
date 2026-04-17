import matplotlib.pyplot as plt
from datetime import datetime
import json
import math

def _calculate_average_time(times):
    if not times:
        return 0
    return sum(times) / len(times)


def plot_execution_time_graph(raw_data, title=None, xlabel=None, ylabel=None, output_path="output/" + datetime.now().strftime("%Y%m%d_%H%M%S") + "_execution_time_graph.png"):
    configs = [(1,1), (1,2), (1,4), (1,8), (2,1), (4,1), (8,1)]
    config_labels = [f"({Np},{Nc})" for (Np, Nc) in configs]

    processed_data = {}

    for N, executions in raw_data.items():
        averages = []

        for (Np, Nc) in configs:
            match = next(
                (e for e in executions if e["Np"] == Np and e["Nc"] == Nc),
                None
            )

            if match:
                avg = _calculate_average_time(match["times"])
            else:
                avg = 0

            averages.append(avg)

        processed_data[N] = averages

    for N, times in processed_data.items():
        plt.plot(config_labels, times, marker='o', label=f'N={N}')

    plt.xlabel(xlabel if xlabel else "Configuration (Np, Nc)")
    plt.ylabel(ylabel if ylabel else "Average execution time (s)")
    plt.title(title if title else "Average execution time vs number of threads")
    plt.legend()
    plt.grid(True)

    plt.savefig(output_path, dpi=300, bbox_inches='tight')

    plt.show()


def _downsample(data, max_points=100):
    """
    Reduz quantidade de pontos pra deixar o gráfico legível
    """
    if len(data) <= max_points:
        return data

    step = len(data) // max_points
    return data[::step]


def plot_buffer_usage_line(raw_data, target_N,
                           title=None,
                           xlabel=None,
                           ylabel=None,
                           output_path="buffer_usage_line.png"):

    configs = [(1,1), (1,2), (1,4), (1,8), (2,1), (4,1), (8,1)]

    executions = raw_data.get(target_N)

    if not executions:
        print(f"N={target_N} not found")
        return

    for (Np, Nc) in configs:
        match = next(
            (e for e in executions if e["Np"] == Np and e["Nc"] == Nc),
            None
        )

        if not match:
            continue

        buffer_usage = match["buffer_usage"]

        # 🔥 reduz pontos (ESSENCIAL)
        buffer_usage = _downsample(buffer_usage, max_points=1000)

        x = list(range(len(buffer_usage)))

        plt.plot(x, buffer_usage, label=f"({Np},{Nc})")

    plt.xlabel(xlabel if xlabel else "Operations")
    plt.ylabel(ylabel if ylabel else "Buffer occupancy")
    plt.title(title if title else f"Buffer usage over time (N={target_N})")
    plt.legend()
    plt.grid(True)

    plt.savefig(output_path, dpi=300, bbox_inches='tight')
    plt.show()