#ifndef PERF_MEASURE_TIME_HPP_
#define PERF_MEASURE_TIME_HPP_

#include <ctime>

struct measure_time
{
    measure_time(double* res): res(res)
    {
        clock_gettime(CLOCK_MONOTONIC, &tic);
    }

    ~measure_time()
    {
        timespec toc;
        clock_gettime(CLOCK_MONOTONIC, &toc);
        if (toc.tv_nsec < tic.tv_nsec)
        {
            toc.tv_nsec += 1000ULL * 1000 * 1000;
            toc.tv_sec -= 1;
        }
        double elapsed_msecs = double(1000) * (toc.tv_sec - tic.tv_sec) + (toc.tv_nsec - tic.tv_nsec) / double(1000 * 1000);
        *res = elapsed_msecs;
    }

    timespec tic;
    double* res;
};

#endif /* PERF_MEASURE_TIME_HPP_ */
