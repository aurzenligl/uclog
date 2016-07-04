#include <cstdio>
#include <ctime>
#include <uclog/logger.hpp>
#include <uclog/handler.hpp>
#include "measure_time.hpp"

enum { N = 1024 * 128 };

struct null_storage : public uclog::storage
{
    void add_site(const uclog::site_t& site)
    { }

    void log(const uclog::site_t& site, va_list args)
    { }
};

struct test_result
{
    const char* name;
    double time;
};

template <uclog::level_t Level>
static const uclog::site_t& make_site()
{
    static uclog::site_t site;
    site.level = Level;
    site.fmt = "test logger %d %d";
    site.hash = 0x12345678;
    return site;
}

struct test_filter_on_logger
{
    void operator()(uclog::logger& lgr, const uclog::site_t& site)
    {
        lgr.log(&site, 13, 42);
    }
};

struct test_filter_on_handlers
{
    void operator()(uclog::logger& lgr, const uclog::site_t& site)
    {
        lgr.log(&site, 13, 42);
    }
};

struct test_pass_to_handlers
{
    void operator()(uclog::logger& lgr, const uclog::site_t& site)
    {
        lgr.log(&site, 13, 42);
    }
};

template <typename TestCase>
struct test
{
    test_result operator()(uclog::logger& lgr, const uclog::site_t& site, const char* name)
    {
        test_result res;
        res.name = name;
        {
            measure_time _(&res.time);
            for (int i = 0; i < N; i++)
            {
                TestCase()(lgr, site);
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
    uclog::logger logger(uclog::level_info);
    logger.add_handler(h1);
    logger.add_handler(h2);

    test_result results[] = {
        test<test_filter_on_logger>()(logger, make_site<uclog::level_debug>(), "test_filter_on_logger"),
        test<test_filter_on_handlers>()(logger, make_site<uclog::level_info>(), "test_filter_on_handlers"),
        test<test_pass_to_handlers>()(logger, make_site<uclog::level_warning>(), "test_pass_to_handlers"),
    };

    printf("%-30s %20s\n", "", "diamond logger");
    for (auto& res : results)
    {
        printf("%-30s %20lf\n", res.name, res.time);
    }

    return 0;
}
