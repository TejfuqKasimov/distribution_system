import sys
import csv
import matplotlib.pyplot as plt


def read_points(path):
    xs, ys = [], []
    with open(path, newline="") as f:
        reader = csv.reader(f)
        header = next(reader, None)
        if header != ["x", "y"]:
            try:
                xs.append(float(header[0]))
                ys.append(float(header[1]))
            except (TypeError, ValueError, IndexError):
                pass
        for row in reader:
            if len(row) < 2:
                continue
            try:
                xs.append(float(row[0]))
                ys.append(float(row[1]))
            except ValueError:
                continue
    return xs, ys


def plot(xs, ys, out_path):
    fig, ax = plt.subplots(figsize=(8, 8))

    ax.scatter(xs, ys, s=1, c="black", marker=".", linewidths=0)

    ax.set_aspect("equal")
    ax.set_title(f"Множество Мандельброта ({len(xs)} точек)")
    ax.set_xlabel("Re(c)")
    ax.set_ylabel("Im(c)")
    ax.grid(True, linestyle=":", alpha=0.4)

    ax.set_xlim(-2.0, 1.0)
    ax.set_ylim(-1.5, 1.5)

    fig.tight_layout()
    fig.savefig(out_path, dpi=200)
    print(f"Сохранено: {out_path} ({len(xs)} точек)")
    plt.show()


def main():
    csv_path = sys.argv[1] if len(sys.argv) > 1 else "mandelbrot.csv"
    out_path = sys.argv[2] if len(sys.argv) > 2 else "mandelbrot.png"

    xs, ys = read_points(csv_path)
    if not xs:
        print(f"Нет точек в файле {csv_path}", file=sys.stderr)
        sys.exit(1)

    plot(xs, ys, out_path)


if __name__ == "__main__":
    main()