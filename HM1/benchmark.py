#!/usr/bin/env python3

import subprocess
import re
import sys

THREADS = "4"

INPUT = """10000
10000
0.99
0.005
"""


def run_once(program):
    result = subprocess.run(
        [program, THREADS],
        input=INPUT,
        text=True,
        capture_output=True,
        check=True,
    )

    match = re.search(r"Elapsed time\s*=\s*([0-9.eE+-]+)", result.stdout)

    if not match:
        raise RuntimeError(
            f"Could not find elapsed time in output:\n{result.stdout}"
        )

    return float(match.group(1))


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} N orig/custom")
        sys.exit(1)

    n = int(sys.argv[1])
    if (sys.argv[2].lower() == "orig"):
        program = "./build/task3_orig"
    elif (sys.argv[2].lower() == "custom"):
        program = "./build/task3_custom"
    else:
        print("Error: Incorrect argv[2]")
        sys.exit(1)

    times = []

    for i in range(n):
        elapsed = run_once(program)
        times.append(elapsed)
        print(f"Run {i + 1}/{n}: {elapsed:.9f} s")

    print()
    print(f"Min:     {min(times):.9f} s")
    print(f"Max:     {max(times):.9f} s")
    print(f"Average: {sum(times) / len(times):.9f} s")
    print(f"Median:  {sorted(times)[len(times)//2] if not (len(times)&1) else (sorted(times)[len(times)//2] + sorted(times)[len(times)//2 + 1]) / 2} s")


if __name__ == "__main__":
    main()
    