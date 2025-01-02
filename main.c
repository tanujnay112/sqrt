#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


float my_sqrt(float x);

void benchmark_sqrt() {
    int num_tests = 100000; // Number of tests
    float max_error = 0.0f;
    float total_time_custom = 0.0;
    float total_time_math = 0.0;
    
    srand(time(NULL)); // Seed for random number generation
    clock_t start, end;

    for (int i = 0; i < num_tests; i++) {
        float x = rand() / (float)RAND_MAX * 10000.0f;  // Random float in range [0, 10000]

        // Benchmark custom sqrt function
        start = clock();
        float result_custom = my_sqrt(x);
        end = clock();
        total_time_custom += (float)(end - start) / CLOCKS_PER_SEC;

        // Benchmark math.h sqrt function
        start = clock();
        float result_math = sqrtf(x);
        end = clock();
        total_time_math += (float)(end - start) / CLOCKS_PER_SEC;

        // Calculate error
        if (!isnan(result_custom)) {
            float error = fabs(result_custom - result_math);
            if (error > max_error) {
                max_error = error;
            }
        }
    }

    printf("Benchmark completed for %d tests.\n", num_tests);
    printf("Total time for custom sqrt: %.6f seconds\n", total_time_custom);
    printf("Total time for math.h sqrt: %.6f seconds\n", total_time_math);
    printf("Maximum error between custom and math.h sqrt: %.6f\n", max_error);
}

int main() {
    benchmark_sqrt();
    return 0;
}

float my_sqrt(float x) {
    if (x < 0) {
        return NAN;
    }
    
    // Can precompute this and proceed with everything else in
    // parallel.
    float xinv = (1.0 / x);
    unsigned int y = *(unsigned int*) &x;
    // y is pretty much the log of x + some constant
    // at this point.
    y = (y >> 1) - 0xe042e20b;
    float y2 = *(float*) &y;
    
    // Newton's iterations on the function (1/y2^2 - 1/x)
    // Note that while a pure Quake III style of this code segment
    // would be doing newton's iterations on (y2^2 - x), we do
    // this instead as it avoids computing a division involving
    // y2 this way.
    y2 = y2 + 0.5 * y2 * (1.0 - y2 * y2 * xinv);
    y2 = y2 + 0.5 * y2 * (1.0 - y2 * y2 * xinv);

    return y2;
}

// what about e^x??
// log(e) * x = log(k)

