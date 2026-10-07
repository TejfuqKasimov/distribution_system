#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int thread_count;
int npoints;
int npoints_count;

double rand_double() {
    return (double)rand()/(double)(RAND_MAX/2) - 4;
}

const double xmin = -2.0, xmax = 1.0;
const double ymin = -1.5, ymax = 1.5;
const int    max_iter = 1000;

pthread_mutex_t mutex;
double* ansX;
double* ansY;

int in_mandelbrot(double cr, double ci)
{
    double zr = 0.0;   // Re(z)
    double zi = 0.0;   // Im(z) 
    const int    max_iter = 1000;
    const double escape_radius_sq = 4.0;   // радиус убегания R = 2, R^2 = 4

    for (int i = 0; i < max_iter; ++i) {
        double zr2 = zr * zr;
        double zi2 = zi * zi;

        // |z|^2 = zr^2 + zi^2 > 4 точка не подходит
        if (zr2 + zi2 > escape_radius_sq)
            return 0;

        // z = z^2 + c,  где z^2 = (zr + i*zi)^2 = (zr^2 - zi^2) + i*(2*zr*zi)
        double new_zr = zr2 - zi2 + cr;
        double new_zi = 2.0 * zr * zi + ci;

        zr = new_zr;
        zi = new_zi;
    }

    return 1;
}

float rand_unit(unsigned int* seed) {
    return (float)rand_r(seed) / (float)RAND_MAX;   // [0, 1]
}

void* routine(void* rank) {
    long this_rank = (long) rank;
    unsigned int seed = (unsigned int)(time(NULL) ^ (this_rank * 2654435761u));

    while (1) {
        pthread_mutex_lock(&mutex);
        int done = (npoints_count >= npoints);
        pthread_mutex_unlock(&mutex);
        if (done) break;

        float x = xmin + (xmax - xmin) * rand_unit(&seed);
        float y = ymin + (ymax - ymin) * rand_unit(&seed);

        if (in_mandelbrot(x, y)) {
            pthread_mutex_lock(&mutex);
            if (npoints_count < npoints) {
                ansX[npoints_count] = x;
                ansY[npoints_count] = y;
                ++npoints_count;
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    thread_count = strtol(argv[1], NULL, 10);
    npoints = strtol(argv[2], NULL, 10);
    ansX = malloc(npoints * sizeof(double));
    ansY = malloc(npoints * sizeof(double));
    pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);
    double need_cells = (double)npoints / 0.05;
    for (long i = 0; i < thread_count; ++i) {
        int err = pthread_create(&thread_handles[i], NULL, routine, (void*) i);
        if (err != 0) {
            perror("Error");
        }
    }

    printf("Start with %d threads and %d points\n", thread_count, npoints);


    for (int i = 0; i < thread_count; ++i) {
        pthread_join(thread_handles[i], NULL);
    }



    // for (int i = 0; i < npoints_count; ++i) {
    //     printf("x=%f, y=%f\n", ansX[i], ansY[i]);
    // }

    FILE *f = fopen("mandelbrot.csv", "w");
    if (!f) { perror("file open error"); return 1; }

    fprintf(f, "x,y\n");
    for (int i = 0; i < npoints_count; ++i) {
        fprintf(f, "%.10f, %.10f\n", ansX[i], ansY[i]);
    }
    fclose(f);

    free (thread_handles);
    free(ansX);
    free(ansY);
    pthread_mutex_destroy(&mutex);
}