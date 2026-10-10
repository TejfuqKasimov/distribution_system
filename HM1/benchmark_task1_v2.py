#!/usr/bin/env python3

import subprocess
import statistics
import time

MACHINE = "Macbook air M5, MacOS 26.5, Chip M5, ARM64" 

THREADS = ["2", "4", "8"]

# Каждый набор — это список позиционных аргументов после threads:
# [npoints, max_iter]
INPUT = [
    ["10000", "1000"],     # маленькая задача, доминируют накладные расходы
    ["100000", "1000"],    # больше точек
    ["100000", "5000"],    # дороже на точку
]

REALIZATIONS = [
    ("Pthread", "./build/task1"),
    ("Orig", "./build/task1_orig"),  # сюда добавить свою реализацию, если будет
]

ATTEMPTS = 100


def run_once(program, threads, params):
    start = time.perf_counter()
    subprocess.run(
        [program, threads, *params],
        text=True,
        capture_output=True,
        check=True,
    )
    return time.perf_counter() - start

def run_once_without_threads(program, params):
    start = time.perf_counter()
    subprocess.run(
        [program, *params],
        text=True,
        capture_output=True,
        check=True,
    )
    return time.perf_counter() - start


def format_params(threads, params):
    return f"threads={threads}; " + "; ".join(
        f"argv{i + 1}={v}" for i, v in enumerate(params)
    )


def main():
    results = []
    for realization, program in REALIZATIONS:
        if realization == "Orig":
            for params in INPUT:    
                times = []
                for i in range(ATTEMPTS):
                    elapsed = run_once_without_threads(program, params)
                    times.append(elapsed)
                    print(
                        f"[{realization}] Run {i + 1}/{ATTEMPTS}: "
                        f"{elapsed:.9f} s with - threads, params={params}"
                    )
                results.append({
                                        "machine": MACHINE,
                                        "realization": realization,
                                        "attempts": ATTEMPTS,
                                        "params": format_params("-", params),
                                        "min": min(times),
                                        "max": max(times),
                                        "avg": statistics.mean(times),
                                        "median": statistics.median(times),
                                        })
        else:
            continue
            for threads in THREADS:
                for params in INPUT:
                    times = []
                    for i in range(ATTEMPTS):
                        elapsed = run_once(program, threads, params)
                        times.append(elapsed)
                        print(
                            f"[{realization}] Run {i + 1}/{ATTEMPTS}: "
                            f"{elapsed:.9f} s with {threads} threads, params={params}"
                        )

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

    print()
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