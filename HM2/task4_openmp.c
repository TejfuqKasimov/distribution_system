#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <errno.h>
#include "timer.h"
#include <omp.h>

// =================================
// Сonstants MCG59
// =================================
#define MCG59_A 302875106592253ULL // 13^13
#define MCG59_M (1ULL << 59)  // 2^59
#define SOBOL_M (1ULL << 32)  // 2^32

typedef struct {
    double S0;
    double K;
    double r;
    double sigma;
    double T;
} Option;

// =================================
// Black Scholes
// =================================
double black_scholes(Option o) {
    double d1 = log(o.S0 / o.K) + (o.r + o.sigma * o.sigma / 2) * o.T / (o.sigma * sqrt(o.T));
    double d2 = d1 - (o.sigma * sqrt(o.T));
    double F1 = 0.5 * (1 + erf(d1/sqrt(2)));
    double F2 = 0.5 * (1 + erf(d2/sqrt(2)));
    return o.S0 * F1 - o.K * exp(-o.r * o.T) * F2;
}

// =================================
// MCG59
// =================================
static inline uint64_t mul_mod(uint64_t x, uint64_t y) {
    return (uint64_t)(((__uint128_t)x * y) % MCG59_M);
}

// Peter Acklam's Algorithm
#define LOW 0.02425
#define HIGH 0.97575

double ltqnorm(double p) {
    static const double a[] =
    {
        -3.969683028665376e+01,
        2.209460984245205e+02,
        -2.759285104469687e+02,
        1.383577518672690e+02,
        -3.066479806614716e+01,
        2.506628277459239e+00
    };

    static const double b[] =
    {
        -5.447609879822406e+01,
        1.615858368580409e+02,
        -1.556989798598866e+02,
        6.680131188771972e+01,
        -1.328068155288572e+01
    };

    static const double c[] =
    {
        -7.784894002430293e-03,
        -3.223964580411365e-01,
        -2.400758277161838e+00,
        -2.549732539343734e+00,
        4.374664141464968e+00,
        2.938163982698783e+00
    };

    static const double d[] =
    {
        7.784695709041462e-03,
        3.224671290700398e-01,
        2.445134137142996e+00,
        3.754408661907416e+00
    };

    double q, r;

    errno = 0;

    if (p < 0 || p > 1)
    {
        errno = EDOM;
        return 0.0;
    }
    else if (p == 0)
    {
        errno = ERANGE;
        return -HUGE_VAL /* minus "infinity" */;
    }
    else if (p == 1)
    {
        errno = ERANGE;
        return HUGE_VAL /* "infinity" */;
    }
    else if (p < LOW)
    {
        /* Rational approximation for lower region */
        q = sqrt(-2*log(p));
        return (((((c[0]*q+c[1])*q+c[2])*q+c[3])*q+c[4])*q+c[5]) /
               ((((d[0]*q+d[1])*q+d[2])*q+d[3])*q+1);
    }
    else if (p > HIGH)
    {
        /* Rational approximation for upper region */
        q  = sqrt(-2*log(1-p));
        return -(((((c[0]*q+c[1])*q+c[2])*q+c[3])*q+c[4])*q+c[5]) /
                ((((d[0]*q+d[1])*q+d[2])*q+d[3])*q+1);
    }
    else
    {
        /* Rational approximation for central region */
            q = p - 0.5;
            r = q*q;
        return (((((a[0]*r+a[1])*r+a[2])*r+a[3])*r+a[4])*r+a[5])*q /
               (((((b[0]*r+b[1])*r+b[2])*r+b[3])*r+b[4])*r+1);
    }
}

uint64_t fast_pow_mod(uint64_t a, uint64_t n) {
    uint64_t result = 1;
    while (n > 0) {
        if (n & 1) {
            result = mul_mod(result, a);
        }
        a = mul_mod(a, a);
        n >>= 1;
    }
    return result;
}

double monte_carlo_mcg59(Option o, uint64_t n, uint64_t threads) {
    uint64_t x_n = 1ULL; // start from 1 (seed = 1)
    double sum = 0.0;
    #pragma omp parallel num_threads(threads) reduction(+:sum) 
    {
        int tid = omp_get_thread_num();
        int nt = omp_get_num_threads();
        size_t begin = n * tid / nt;
        size_t end = n * (tid + 1) / nt;

        uint64_t x_n_1 = mul_mod(x_n, fast_pow_mod(MCG59_A, (uint64_t)begin + 1));
        double local_sum = 0.0;

        for (size_t i = begin; i < end; i++) {
            double u_n = (double)x_n_1 / (double)MCG59_M;
            double z_i = ltqnorm(u_n);
            local_sum += fmax(o.S0 * exp((o.r - o.sigma*o.sigma/2.0)*o.T + o.sigma*sqrt(o.T)*z_i) - o.K, 0.0);
            x_n_1 = mul_mod(x_n_1, MCG59_A);
        }

        sum += local_sum;
    }
    return exp(-o.r*o.T) * sum / (double)n;
}

// =================================
// SOBOL
// =================================
double van_der_corput_fast(uint32_t bits) {
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    
    return (double)bits / SOBOL_M;
}

double monte_carlo_sobol(Option o, uint64_t n, uint64_t threads) {
    double sum = 0.0;
    #pragma omp parallel for num_threads(threads) reduction(+:sum) schedule(static)
    for (uint64_t i = 0; i < n; i++) {
        double u_n = van_der_corput_fast((uint32_t)(i + 1));
        double z_i = ltqnorm(u_n);
        sum += fmax(o.S0 * exp((o.r - o.sigma*o.sigma/2.0)*o.T + o.sigma*sqrt(o.T)*z_i) - o.K, 0.0);
    }
    return exp(-o.r*o.T) * sum / (double)n;
}

double generate_standard_normal(unsigned int *seed) {
    double u1 = ((double)rand_r(seed) + 1.0) / ((double)RAND_MAX + 1.0);
    double u2 = ((double)rand_r(seed) + 1.0) / ((double)RAND_MAX + 1.0);
    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

double monte_carlo_default(Option o, uint64_t n, uint64_t threads) {
    double sum = 0.0;
    #pragma omp parallel num_threads(threads) reduction(+:sum)
    {
        unsigned int seed = (unsigned int)(time(NULL) ^ (omp_get_thread_num() * 2654435761u));
        #pragma omp for schedule(static)
        for (uint64_t i = 0; i < n; i++) {
            double z_i = generate_standard_normal(&seed);
            sum += fmax(o.S0 * exp((o.r - o.sigma*o.sigma/2.0)*o.T + o.sigma*sqrt(o.T)*z_i) - o.K, 0.0);
        }
    }
    return exp(-o.r*o.T) * sum / (double)n;
}

int main(int argc, char* argv[]) {
    if (argc > 3) {
        printf("Using: ./task4_openmp <N> <threads_count>\n");
        return 1;
    }

    uint64_t attempts = strtol(argv[1], NULL, 10);
    uint64_t threads_count = strtol(argv[2], NULL, 10);
    double start, finish;

    double s0, K, r, sigma, T;
    scanf("%lf", &s0);     // S0 - начальная цена акции
    scanf("%lf", &K);      // K - цена исполнения
    scanf("%lf", &r);      // r - процентная ставка
    scanf("%lf", &sigma);  // sigma - волатильность
    scanf("%lf", &T);      // T - время до исполнения 
    Option option = {s0, K, r, sigma, T};
    
    start = omp_get_wtime();
    double ans = black_scholes(option);
    printf("Black-Scholes price: %.9f\n", ans);
    finish = omp_get_wtime();
    printf("Elapsed time = %e seconds\n\n", finish - start);
    
    start = omp_get_wtime();
    ans = monte_carlo_mcg59(option, attempts, threads_count);
    printf("MCG59 price: %.9f\n", ans);
    finish = omp_get_wtime();
    printf("Elapsed time = %e seconds\n\n", finish - start);

    start = omp_get_wtime();
    ans = monte_carlo_sobol(option, attempts, threads_count);
    printf("Sobol price: %.9f\n", ans);
    finish = omp_get_wtime();
    printf("Elapsed time = %e seconds\n\n", finish - start);

    start = omp_get_wtime();
    ans = monte_carlo_default(option, attempts, threads_count);
    printf("Default_std_normal price: %.9f\n", ans);
    finish = omp_get_wtime();
    printf("Elapsed time = %e seconds\n", finish - start);

    return 0;
}
