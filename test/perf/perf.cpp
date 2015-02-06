#include <cstdio>
#include <ctime>
#include <uclog/bprintf.hpp>

enum { N = 1024 * 128 };

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

struct test_result
{
    const char* name;
    double v1;
    double v2;
};

template <typename TestCase>
test_result test(const char* name)
{
    test_result res;
    res.name = name;
    {
        measure_time _(&res.v1);
        for (int i = 0; i < N; i++)
        {
            TestCase()(&snprintf);
        }
    }
    {
        measure_time _(&res.v2);
        for (int i = 0; i < N; i++)
        {
            TestCase()(&uclog::snbprintf);
        }
    }
    return res;
}

struct test_fmt
{
    template <typename FuncType>
    void operator()(FuncType fun)
    {
        char buf[1024];
        fun(buf, 1024,
            "hisuhdspoaiuhdaouhfaodshapsdgoihsdgouashpfdhfp[wenpimc[ejmiejrcqwejrwepijraoakpsoka"
        );
    }
};

struct test_strings
{
    template <typename FuncType>
    void operator()(FuncType fun)
    {
        char buf[1024];
        fun(buf, 1024, "%s %s %s", "ala", "ma", "kota");
    }
};

struct test_int
{
    template <typename FuncType>
    void operator()(FuncType fun)
    {
        char buf[1024];
        fun(buf, 1024, "%d %d %d %d", 42, 42, 42, 42);
    }
};

struct test_float
{
    template <typename FuncType>
    void operator()(FuncType fun)
    {
        char buf[1024];
        fun(buf, 1024, "%f %le %G %lA", 42., 42., 42., 42.);
    }
};

struct test_many_ints
{
    template <typename FuncType>
    void operator()(FuncType fun)
    {
        char buf[1024];
        fun(buf, 1024,
            "%hhd %hhd %hhd %hhd "
            "%hd %hd %hd %hd "
            "%d %d %d %d "
            "%ld %ld %ld %ld "
            "%lld %lld %lld %lld",
            42, 42, 42, 42,
            42, 42, 42, 42,
            42, 42, 42, 42,
            42L, 42L, 42L, 42L,
            42LL, 42LL, 42LL, 42LL
        );
    }
};

int main()
{
    test_result results[] = {
        test<test_fmt>("test_fmt"),
        test<test_strings>("test_strings"),
        test<test_int>("test_int"),
        test<test_float>("test_float"),
        test<test_many_ints>("test_many_ints")
    };

    printf("%20s %20s %20s\n", "", "snprintf", "uclog::snbprintf");
    for (auto& res : results)
    {
        printf("%-20s %20lf %20lf\n", res.name, res.v1, res.v2);
    }

    return 0;
}
