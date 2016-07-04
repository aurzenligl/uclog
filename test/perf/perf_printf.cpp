#include <cstdio>
#include <benchmark/benchmark.h>
#include <uclog/bprintf.hpp>
#include <uclog/site.hpp>
#include <uclog/logger.hpp>

using namespace uclog;

template <int N>
struct printf_t
{
    printf_t(const char*){}

    void operator()(void* buf, size_t size, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        vsnprintf(static_cast<char*>(buf), size, fmt, args);
        va_end(args);
    }
};

template <int N>
struct bprintf_t
{
    bprintf_t(const char*){}

    void operator()(void* buf, size_t size, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        vsnbprintf(buf, size, fmt, args);
        va_end(args);
    }
};

template <int N>
struct args_encode_t
{
    args_encode_t(const char* fmt)
    {
        while (*fmt)
        {
            arg_spec spec;
            fmt += arg_decode(fmt, &spec);
            types.push_back(spec.type);
        }
    }

    void operator()(void* buf, size_t size, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        vargs_encode(buf, size, types.data(), types.size(), args);
        va_end(args);
    }

    std::vector<arg_type> types;
};

template <int N>
struct site_data_t
{
    site_data_t(const char*){}

    void operator()(void* buf, size_t size, const char* fmt, ...)
    {
        site_data<N>(lgr, level_critical, fmt);
    }

    logger lgr;
};

template <template <int> class Storage>
void bm_fmt(benchmark::State& state)
{
    char buf[1024];
    const char* fmt = "hisuhdspoaiuhdaouhfaodshapsdgoihsdgouashpfdhfp[wenpimc[ejmiejrcqwejrwepijraoakpsoka";
    Storage<0> stor(fmt);

    while (state.KeepRunning())
    {
        stor(buf, 1024, fmt);
    }
}

template <template <int> class Storage>
void bm_strings(benchmark::State& state)
{
    char buf[1024];
    const char* fmt = "%s %s %s";
    Storage<3> stor(fmt);

    while (state.KeepRunning())
    {
        stor(buf, 1024, fmt, "ala", "ma", "kota");
    }
}

template <template <int> class Storage>
void bm_ints(benchmark::State& state)
{
    char buf[1024];
    const char* fmt = "%d %d %d %d";
    Storage<4> stor(fmt);

    while (state.KeepRunning())
    {
        stor(buf, 1024, fmt, 42, 42, 42, 42);
    }
}

template <template <int> class Storage>
void bm_float(benchmark::State& state)
{
    char buf[1024];
    const char* fmt = "%f %le %G %lA";
    Storage<4> stor(fmt);

    while (state.KeepRunning())
    {
        stor(buf, 1024, fmt, 42., 42., 42., 42.);
    }
}

template <template <int> class Storage>
void bm_many_ints(benchmark::State& state)
{
    char buf[1024];
    const char* fmt =
        "%hhd %hhd %hhd %hhd "
        "%hd %hd %hd %hd "
        "%d %d %d %d "
        "%ld %ld %ld %ld "
        "%lld %lld %lld %lld";
    Storage<20> stor(fmt);

    while (state.KeepRunning())
    {
        stor(buf, 1024, fmt,
            42, 42, 42, 42,
            42, 42, 42, 42,
            42, 42, 42, 42,
            42L, 42L, 42L, 42L,
            42LL, 42LL, 42LL, 42LL
        );
    }
}

BENCHMARK_TEMPLATE(bm_fmt, printf_t);
BENCHMARK_TEMPLATE(bm_strings, printf_t);
BENCHMARK_TEMPLATE(bm_ints, printf_t);
BENCHMARK_TEMPLATE(bm_float, printf_t);
BENCHMARK_TEMPLATE(bm_many_ints, printf_t);

BENCHMARK_TEMPLATE(bm_fmt, bprintf_t);
BENCHMARK_TEMPLATE(bm_strings, bprintf_t);
BENCHMARK_TEMPLATE(bm_ints, bprintf_t);
BENCHMARK_TEMPLATE(bm_float, bprintf_t);
BENCHMARK_TEMPLATE(bm_many_ints, bprintf_t);

BENCHMARK_TEMPLATE(bm_fmt, args_encode_t);
BENCHMARK_TEMPLATE(bm_strings, args_encode_t);
BENCHMARK_TEMPLATE(bm_ints, args_encode_t);
BENCHMARK_TEMPLATE(bm_float, args_encode_t);
BENCHMARK_TEMPLATE(bm_many_ints, args_encode_t);

BENCHMARK_TEMPLATE(bm_fmt, site_data_t);
BENCHMARK_TEMPLATE(bm_strings, site_data_t);
BENCHMARK_TEMPLATE(bm_ints, site_data_t);
BENCHMARK_TEMPLATE(bm_float, site_data_t);
BENCHMARK_TEMPLATE(bm_many_ints, site_data_t);

BENCHMARK_MAIN();
