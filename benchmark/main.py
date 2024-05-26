import json
import sys
import matplotlib.pyplot as plt


def load_benchmark_results(file_path):
    """Load benchmark results from a JSON file."""
    with open(file_path) as f:
        data = json.load(f)
    return data


def extract_benchmark_data(benchmarks):
    """Extract benchmark data for plotting."""
    operations = []
    xor_list_times = []
    std_list_times = []

    for benchmark in benchmarks:
        name = benchmark["name"]
        time = benchmark["real_time"]

        if "XORList" in name:
            operations.append(name.replace("BM_XORList_", "").split("/")[0])
            xor_list_times.append(time)
        elif "StdList" in name:
            std_list_times.append(time)

    return operations, xor_list_times, std_list_times


def plot_benchmark_comparison(operations, xor_list_times, std_list_times, output_file):
    """Plot the benchmark comparison and save it to a file."""
    x = range(len(operations))
    plt.figure(figsize=(12, 6))
    plt.bar(x, xor_list_times, width=0.4, label="XORList", align="center")
    plt.bar(x, std_list_times, width=0.4, label="std::list", align="edge")

    plt.xlabel("Operations")
    plt.ylabel("Time (ns)")
    plt.title("Performance Comparison: XORList vs std::list")
    plt.xticks(x, operations, rotation=45)
    plt.legend()
    plt.tight_layout()

    # Save the plot
    plt.savefig(output_file)
    plt.show()


def main():
    if len(sys.argv) > 1:
        input_file = sys.argv[1]
    else:
        print("Usage: python script_name.py <path_to_json_file>")
        sys.exit(1)

    output_file = "benchmark_comparison.png"

    # Load and process benchmark results
    data = load_benchmark_results(input_file)
    benchmarks = data["benchmarks"]
    operations, xor_list_times, std_list_times = extract_benchmark_data(benchmarks)

    # Plot the benchmark comparison
    plot_benchmark_comparison(operations, xor_list_times, std_list_times, output_file)


if __name__ == "__main__":
    main()
