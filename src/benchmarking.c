#include "libhdcr.h"
#include "benchmarking.h"
#include "mcc.h"
#include "morph.h"
#include "thinning.h"
#include "thresh.h"
#include "IO.h"
#include "cppthresh.hpp"
#include <time.h>

void run_benchmarks(char* imageName, uint8_t CGL)
{
    IMAGE test;
    readPNGandClose(imageName, &test);
    printf("Timing benchmarks for libhdcr using %s\n", imageName);
    printf("Image is %dx%d pixels\n", test.n_rows, test.n_cols);

    printf("\n");
    printf("function, seconds\n");

    // provided in time.h
    clock_t start, end;
    double cpu_time_used;

    /* erosion */
    printf("erodeImage3by3Kernel, ");
    start = clock();
    erodeImage3by3Kernel(&test, CGL);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%lf\n", cpu_time_used);


    printf("dilateImage3by3Kernel, ");
    start = clock();
    dilateImage3by3Kernel(&test, CGL);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%lf\n", cpu_time_used);


}
