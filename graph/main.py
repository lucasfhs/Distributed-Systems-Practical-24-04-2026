from plot import plot_execution_time_graph, plot_buffer_usage_line, plot_buffer_usage_grid
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

    with open("results.json") as f:
        data = json.load(f)

    data = {int(k): v for k, v in data.items()}
    
    plot_buffer_usage_grid(data, target_N=1, output_path="output/buffer_usage_grid_N_1.png")
    plot_buffer_usage_grid(data, target_N=10, output_path="output/buffer_usage_grid_N_10.png")
    plot_buffer_usage_grid(data, target_N=100, output_path="output/buffer_usage_grid_N_100.png")
    plot_buffer_usage_grid(data, target_N=1000, output_path="output/buffer_usage_grid_N_1000.png")

    print("Gráficos gerados com sucesso!\nVerifique a pasta 'output' para visualizá-los.")
if __name__ == "__main__":
    main()