#include <cstdio>
#include <ctime>
#include <vector>
#include <uclog/bprintf.hpp>

struct measure_time
{
    measure_time(std::vector<double>& results): results(&results)
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
        results->push_back(elapsed_msecs);
    }

    timespec tic;
    std::vector<double>* results;
};

#define FMT_TEST(FUNCTION) \
    { \
        char buf[1024]; \
        FUNCTION(buf, 1024, \
            "very long format string " \
            "very long format string " \
            "very long format string " \
            "very long format string " \
            "very long format string" \
        ); \
    }
#define INT_TEST(FUNCTION) \
    { \
        char buf[1024]; \
        FUNCTION(buf, 1024, "%d %d %d %d", 42, 42, 42, 42); \
    }
#define FLOAT_TEST(FUNCTION) \
    { \
        char buf[1024]; \
        FUNCTION(buf, 1024, "%f %le %G %lA", 42., 42., 42., 42.); \
    }
#define MULTIPLE_TEST(FUNCTION) \
    { \
        char buf[1024]; \
        FUNCTION(buf, 1024, \
            "%hhd %hhd %hhd %hhd " \
            "%hd %hd %hd %hd " \
            "%d %d %d %d " \
            "%ld %ld %ld %ld " \
            "%lld %lld %lld %lld", \
            42, 42, 42, 42, \
            42, 42, 42, 42, \
            42, 42, 42, 42, \
            42L, 42L, 42L, 42L, \
            42LL, 42LL, 42LL, 42LL \
        ); \
    }

enum { N = 1024 * 128 };

#define TEST(TEST_NAME, TEST_FUNCTION, TEST_RESULTS) \
    { \
        measure_time _(TEST_RESULTS); \
        for (int i = 0; i < N; i++) \
        { \
            TEST_NAME(TEST_FUNCTION); \
        } \
    }

int main()
{
    std::vector<double> snprintf_results;
    std::vector<double> snbprintf_results;

    const char* test_names[] = {
        "FMT_TEST",
        "INT_TEST",
        "FLOAT_TEST",
        "MULTIPLE_TEST"
    };

    TEST(FMT_TEST, snprintf, snprintf_results);
    TEST(FMT_TEST, uclog::snbprintf, snbprintf_results);
    TEST(INT_TEST, snprintf, snprintf_results);
    TEST(INT_TEST, uclog::snbprintf, snbprintf_results);
    TEST(FLOAT_TEST, snprintf, snprintf_results);
    TEST(FLOAT_TEST, uclog::snbprintf, snbprintf_results);
    TEST(MULTIPLE_TEST, snprintf, snprintf_results);
    TEST(MULTIPLE_TEST, uclog::snbprintf, snbprintf_results);

    printf("%20s %20s %20s\n", "", "snprintf", "uclog::snbprintf");
    for (unsigned i = 0; i < snprintf_results.size(); i++)
    {
        printf("%-20s %20lf %20lf\n", test_names[i], snprintf_results[i], snbprintf_results[i]);
    }

    return 0;
}
