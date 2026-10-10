#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

int npoints;
int npoints_count;

double* ansX;
double* ansY;

const double xmin = -2.0, xmax = 1.0;
const double ymin = -1.5, ymax = 1.5;

int max_iter = 1000;

double rand_unit(unsigned int* seed) {
    return (double)rand_r(seed)/(double)RAND_MAX;
}


int in_mandelbrot(double cr, double ci) {
    double zr = 0.0;
    double zi = 0.0;

    for (int i = 0; i < max_iter; ++i) {
        double zr2 = zr * zr;
        double zi2 = zi * zi;

        if (zr2 + zi2 > 4.0) {
            return 0;
        }

        zi = 2.0 * zr * zi + ci;
        zr = zr2 - zi2 + cr;
    }

    return 1;
}

int main(int argc, char* argv[]) {
    
    npoints = strtol(argv[1], NULL, 10);
    if (argc > 2) {
        max_iter = strtol(argv[2], NULL, 10);
    }

    ansX = malloc(npoints * sizeof(double));
    ansY = malloc(npoints * sizeof(double));

    printf("Start with 1 Threads and %d points\n", npoints);
    unsigned seed = (unsigned int)(time(NULL));

    while (1) {
        if (npoints_count >= npoints) break;

        double x = xmin + (xmax - xmin) * rand_unit(&seed);
        double y = ymin + (ymax - ymin) * rand_unit(&seed);

        if (in_mandelbrot(x, y)) {
            if (npoints_count < npoints) {
                ansX[npoints_count] = x;
                ansY[npoints_count] = y;
                ++npoints_count;
            }
        }
    }


    FILE *f = fopen("mandelbrot.csv", "w");
    if (!f) { perror("file open error"); return 1; }

    fprintf(f, "x,y\n");
    for (int i = 0; i < npoints_count; ++i) {
        fprintf(f, "%.10lf, %.10lf\n", ansX[i], ansY[i]);
    }
    fclose(f);

    free(ansX);
    free(ansY);
}