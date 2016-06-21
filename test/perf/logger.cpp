#include <cstdio>
#include <ctime>
#include <uclog/logger.hpp>
#include "measure_time.hpp"

enum { N = 1024 * 128 };

struct null_storage : public uclog::storage
{
    virtual void store(const uclog::logger_info& logger, uclog::level_t level, const char* fmt, va_list args)
    { }
};

struct test_result
{
    const char* name;
    double time;
};

uclog::logger* lgr;

struct test_filter_on_logger
{
    void operator()()
    {
        lgr->log(uclog::level_debug, "test logger %d %d", 13, 42);
    }
};

struct test_filter_on_handlers
{
    void operator()()
    {
        lgr->log(uclog::level_info, "test logger %d %d", 13, 42);
    }
};

struct test_pass_to_handlers
{
    void operator()()
    {
        lgr->log(uclog::level_warning, "test logger %d %d", 13, 42);
    }
};

template <typename TestCase>
struct test
{
    test_result operator()(const char* name)
    {
        test_result res;
        res.name = name;
        {
            measure_time _(&res.time);
            for (int i = 0; i < N; i++)
            {
                TestCase()();
            }
        }
        return res;
    }
};

int main()
{
    null_storage storage;
    uclog::handler h1(storage, uclog::level_warning);
    uclog::handler h2(storage, uclog::level_warning);
    uclog::logger logger(uclog::logger_info(), uclog::level_info);
    logger.add_handler(h1);
    logger.add_handler(h2);
    lgr = &logger;

    test_result results[] = {
        test<test_filter_on_logger>()("test_filter_on_logger"),
        test<test_filter_on_handlers>()("test_filter_on_handlers"),
        test<test_pass_to_handlers>()("test_pass_to_handlers"),
    };

    printf("%-30s %20s\n", "", "diamond logger");
    for (auto& res : results)
    {
        printf("%-30s %20lf\n", res.name, res.time);
    }

    return 0;
}
