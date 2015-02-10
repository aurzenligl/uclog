#include <cstdio>
#include <ctime>
#include <uclog/bprintf.hpp>
#include "measure_time.hpp"

enum { N = 1024 * 128 };

struct test_result
{
    const char* name;
    double v1;
    double v2;
};

struct printf_adapter
{
    void operator()(void* buf, size_t size, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        vsnprintf(static_cast<char*>(buf), size, fmt, args);
        va_end(args);
    }
};

struct bprintf_adapter
{
    void operator()(void* buf, size_t size, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        uclog::vsnbprintf(buf, size, fmt, args);
        va_end(args);
    }
};

template <template <typename> class TestCase>
test_result test(const char* name)
{
    test_result res;
    res.name = name;
    {
        measure_time _(&res.v1);
        for (int i = 0; i < N; i++)
        {
            TestCase<printf_adapter>()();
        }
    }
    {
        measure_time _(&res.v2);
        for (int i = 0; i < N; i++)
        {
            TestCase<bprintf_adapter>()();
        }
    }
    return res;
}

template <typename FuncType>
struct test_fmt
{
    void operator()()
    {
        char buf[1024];
        FuncType()(buf, 1024,
            "hisuhdspoaiuhdaouhfaodshapsdgoihsdgouashpfdhfp[wenpimc[ejmiejrcqwejrwepijraoakpsoka"
        );
    }
};

template <typename FuncType>
struct test_strings
{
    void operator()()
    {
        char buf[1024];
        FuncType()(buf, 1024, "%s %s %s", "ala", "ma", "kota");
    }
};

template <typename FuncType>
struct test_int
{
    void operator()()
    {
        char buf[1024];
        FuncType()(buf, 1024, "%d %d %d %d", 42, 42, 42, 42);
    }
};

template <typename FuncType>
struct test_float
{
    void operator()()
    {
        char buf[1024];
        FuncType()(buf, 1024, "%f %le %G %lA", 42., 42., 42., 42.);
    }
};

template <typename FuncType>
struct test_many_ints
{
    void operator()()
    {
        char buf[1024];
        FuncType()(buf, 1024,
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
