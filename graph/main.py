from plot import plot_execution_time_graph
import json

def load_json_and_format(file_path):
    with open(file_path, "r") as f:
        data = json.load(f)

    formatted_data = {}

    for N_str, executions in data.items():
        N = int(N_str)
        formatted_data[N] = executions

    return formatted_data

def main():
    plot_execution_time_graph(load_json_and_format("results.json"))

if __name__ == "__main__":
    main()