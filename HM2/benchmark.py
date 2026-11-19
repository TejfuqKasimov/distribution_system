
import subprocess
import re
import csv
import statistics
import os
import matplotlib.pyplot as plt

EXECUTABLE = "./build/task4_openmp"
PARAMS_FILE = "./build/params.txt"

N_VALUES = [
    1_000,
    10_000,
    100_000,
    1_000_000,
    10_000_000,
    100_000_000
]

THREADS = [2, 4, 8, 16]
REPEATS = 5

OUTPUT_DIR = "outputs"

METHODS = {
    "MCG59": r"MCG59 price:\s*([\d.eE+-]+)\s*\nElapsed time = ([\d.eE+-]+)",
    "Sobol": r"Sobol price:\s*([\d.eE+-]+)\s*\nElapsed time = ([\d.eE+-]+)",
    "Default_std_normal":
        r"Default_std_normal price:\s*([\d.eE+-]+)\s*\nElapsed time = ([\d.eE+-]+)"
}


def read_parameters():
    with open(PARAMS_FILE, "r") as f:
        return f.read()


def run_program(n, params, threads):
    result = subprocess.run(
        [EXECUTABLE, str(n), str(threads)],
        input=params,
        text=True,
        capture_output=True,
        check=True
    )

    output = result.stdout

    bs_match = re.search(
        r"Black-Scholes price:\s*([\d.eE+-]+)",
        output
    )

    if not bs_match:
        raise RuntimeError(
            "Black-Scholes price not found:\n" + output
        )

    bs_price = float(bs_match.group(1))
    measurements = {}

    for method, pattern in METHODS.items():
        match = re.search(pattern, output)

        if not match:
            raise RuntimeError(
                f"Results for {method} not found:\n{output}"
            )

        measurements[method] = {
            "price": float(match.group(1)),
            "time": float(match.group(2))
        }

    return bs_price, measurements


def main():
    os.makedirs(OUTPUT_DIR, exist_ok=True)

    params = read_parameters()
    results = []

    for n in N_VALUES:
        print(f"\n{'=' * 65}")
        print(f"N = {n:,}")
        print(f"{'=' * 65}")

        for thread in THREADS:
            print(f"\nThreads = {thread}")

            runs = {
                method: {"prices": [], "times": []}
                for method in METHODS
            }

            bs_price = None

            for repeat in range(REPEATS):
                current_bs, measurements = run_program(
                    n, params, thread
                )
                bs_price = current_bs

                for method in METHODS:
                    runs[method]["prices"].append(
                        measurements[method]["price"]
                    )
                    runs[method]["times"].append(
                        measurements[method]["time"]
                    )

            for method in METHODS:
                # Среднее значение цены по повторам
                price = statistics.mean(
                    runs[method]["prices"]
                )

                # Медиана времени выполнения
                elapsed = statistics.median(
                    runs[method]["times"]
                )

                abs_error = abs(price - bs_price)
                rel_error = abs_error / abs(bs_price) * 100.0

                row = {
                    "N": n,
                    "threads": thread,
                    "method": method,
                    "price": price,
                    "black_scholes": bs_price,
                    "absolute_error": abs_error,
                    "relative_error_percent": rel_error,
                    "time_seconds": elapsed
                }

                results.append(row)

                print(
                    f"{method:20s} "
                    f"price={price:.9f}, "
                    f"error={abs_error:.9e}, "
                    f"time={elapsed:.6f} s"
                )

    # =================================
    # Save CSV
    # =================================

    csv_path = os.path.join(OUTPUT_DIR, "results.csv")

    with open(csv_path, "w", newline="") as f:
        writer = csv.DictWriter(
            f,
            fieldnames=results[0].keys()
        )
        writer.writeheader()
        writer.writerows(results)

    print(f"\nResults saved to {csv_path}")

    # =================================
    # Absolute error
    # =================================

    for thread in THREADS:
        plt.figure(figsize=(10, 6))

        for method in METHODS:
            data = [
                r for r in results
                if r["method"] == method
                and r["threads"] == thread
            ]

            plt.loglog(
                [r["N"] for r in data],
                [r["absolute_error"] for r in data],
                marker="o",
                label=method
            )

        plt.xlabel("Number of simulations N")
        plt.ylabel("Absolute error")
        plt.title(f"Absolute error, threads = {thread}")
        plt.grid(True, which="both")
        plt.legend()
        plt.tight_layout()

        plt.savefig(
            os.path.join(
                OUTPUT_DIR,
                f"absolute_error_{thread}_threads.png"
            ),
            dpi=200
        )

    # =================================
    # Relative error
    # =================================

    for thread in THREADS:
        plt.figure(figsize=(10, 6))

        for method in METHODS:
            data = [
                r for r in results
                if r["method"] == method
                and r["threads"] == thread
            ]

            plt.loglog(
                [r["N"] for r in data],
                [r["relative_error_percent"] for r in data],
                marker="o",
                label=method
            )

        plt.xlabel("Number of simulations N")
        plt.ylabel("Relative error (%)")
        plt.title(f"Relative error, threads = {thread}")
        plt.grid(True, which="both")
        plt.legend()
        plt.tight_layout()

        plt.savefig(
            os.path.join(
                OUTPUT_DIR,
                f"relative_error_{thread}_threads.png"
            ),
            dpi=200
        )

    # =================================
    # Execution time
    # =================================

    for thread in THREADS:
        plt.figure(figsize=(10, 6))

        for method in METHODS:
            data = [
                r for r in results
                if r["method"] == method
                and r["threads"] == thread
            ]

            plt.loglog(
                [r["N"] for r in data],
                [r["time_seconds"] for r in data],
                marker="o",
                label=method
            )

        plt.xlabel("Number of simulations N")
        plt.ylabel("Execution time (seconds)")
        plt.title(f"Execution time, threads = {thread}")
        plt.grid(True, which="both")
        plt.legend()
        plt.tight_layout()

        plt.savefig(
            os.path.join(
                OUTPUT_DIR,
                f"execution_time_{thread}_threads.png"
            ),
            dpi=200
        )

    # =================================
    # Speedup relative to 2 threads
    # =================================

    plt.figure(figsize=(10, 6))

    for method in METHODS:
        speedups = []

        for n in N_VALUES:
            baseline = next(
                r["time_seconds"] for r in results
                if r["N"] == n
                and r["threads"] == THREADS[0]
                and r["method"] == method
            )

            for thread in THREADS:
                current = next(
                    r["time_seconds"] for r in results
                    if r["N"] == n
                    and r["threads"] == thread
                    and r["method"] == method
                )

                speedups.append({
                    "N": n,
                    "threads": thread,
                    "speedup": baseline / current
                })

        # График speedup для максимального N
        data = [
            r for r in speedups
            if r["N"] == max(N_VALUES)
        ]

        plt.plot(
            [r["threads"] for r in data],
            [r["speedup"] for r in data],
            marker="o",
            label=method
        )

    plt.plot(
        THREADS,
        [t / THREADS[0] for t in THREADS],
        linestyle="--",
        label="Ideal speedup"
    )

    plt.xlabel("Number of threads")
    plt.ylabel("Speedup")
    plt.title(f"Speedup for N = {max(N_VALUES):,}")
    plt.xticks(THREADS)
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    plt.savefig(
        os.path.join(OUTPUT_DIR, "speedup.png"),
        dpi=200
    )

    plt.show()


if __name__ == "__main__":
    main()
