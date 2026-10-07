#!/usr/bin/env python3

import subprocess
import re
import statistics

MACHINE = "Macbook air M4, MacOS 26.5.2, Chip M4, ARM64"

THREADS = ["2", "4", "8"]

INPUT = [
    "10000\n10000\n0.99\n0.005",  # priority to read
    "10000\n10000\n0.2\n0.5",     # priority to write
]

REALIZATIONS = [
    ("Pthread", "./build/task3_orig"),
    ("Custom", "./build/task3_custom"),
]

ATTEMPTS = 100


def run_once(program, threads, params):
    result = subprocess.run(
        [program, threads],
        input=params,
        text=True,
        capture_output=True,
        check=True,
    )

    match = re.search(
        r"Elapsed time\s*=\s*([0-9.eE+-]+)",
        result.stdout,
    )

    if not match:
        raise RuntimeError(
            f"Could not find elapsed time in output:\n{result.stdout}"
        )

    return float(match.group(1))


def format_params(threads, params):
    values = [line.strip() for line in params.strip().splitlines()]
    return f"{threads}, {', '.join(values)}"


def main():
    results = []

    for threads in THREADS:
        for params in INPUT:
            for realization, program in REALIZATIONS:
                times = []

                for i in range(ATTEMPTS):
                    elapsed = run_once(program, threads, params)
                    times.append(elapsed)
                    print(f"[{realization}] Run {i + 1}/{ATTEMPTS}: {elapsed:.9f} s with {threads} threads")

                results.append({
                    "machine": MACHINE,
                    "realization": realization,
                    "attempts": ATTEMPTS,
                    "params": format_params(threads, params),
                    "min": min(times),
                    "max": max(times),
                    "avg": statistics.mean(times),
                    "median": statistics.median(times),
                })

    print("| Machine | Realization | Attemps | Params | Min(sec) | Max(sec) | Avg(sec) | Median(sec) |")
    print("|---------|-------------|---------|--------|----------|----------|----------|-------------|")

    for result in results:
        print(
            f"| {result['machine']} "
            f"| {result['realization']} "
            f"| {result['attempts']} "
            f"| {result['params']} "
            f"| {result['min']:.9f} "
            f"| {result['max']:.9f} "
            f"| {result['avg']:.9f} "
            f"| {result['median']:.9f} |"
        )


if __name__ == "__main__":
    main()