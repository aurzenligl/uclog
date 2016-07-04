#include <gtest/gtest.h>
#include <uclog/basic_log.hpp>
#include <uclog/handler.hpp>
#include "fake_storage.hpp"

using namespace testing;
using namespace uclog;

template <typename T, typename... Arg>
static std::vector<T> to_vec(Arg... as)
{
    return std::vector<T>{as...};
}

template <typename T, typename U, typename Pred>
static std::vector<T> ex_vec(const std::vector<U>& as, Pred pred)
{
    std::vector<T> xs;
    for (const auto& a : as)
    {
        xs.push_back(pred(a));
    }
    return xs;
}

TEST(basic_log, logs)
{
    logger lgr;
    fake_storage stor;
    handler handl(stor);
    lgr.add_handler(handl);

#define LOG(LEVEL, FMT, ...) UCLOG_BASIC_LOG(&lgr, LEVEL, FMT, __VA_ARGS__)
    LOG(info, "first %d %d", 1, 2);
    LOG(warning, "second %d", 42);
    LOG(error, "third");
#undef LOG

    EXPECT_EQ("first 1 2\nsecond 42\nthird\n", stor.data);
    EXPECT_EQ(
        to_vec<std::string>("first %d %d", "second %d", "third"),
        ex_vec<std::string>(stor.sites, [](const uclog::site_t* s){ return s->fmt; }));
    EXPECT_EQ(
        to_vec<int>(2, 1, 0),
        ex_vec<int>(stor.sites, [](const uclog::site_t* s){ return s->args.size(); }));
    EXPECT_EQ(
        to_vec<level_t>(level_info, level_warning, level_error),
        ex_vec<level_t>(stor.sites, [](const uclog::site_t* s){ return s->level; }));
}

TEST(basic_log, logs_multiple_times)
{
    logger lgr;
    fake_storage stor;
    handler handl(stor);
    lgr.add_handler(handl);

#define LOG(LEVEL, FMT, ...) UCLOG_BASIC_LOG(&lgr, LEVEL, FMT, __VA_ARGS__)
    auto log = [&](){ LOG(info, "first"); };
#undef LOG

    log();
    log();
    log();

    EXPECT_EQ("first\nfirst\nfirst\n", stor.data);
    EXPECT_EQ(1, stor.sites.size());
}

TEST(basic_log, logs_name_collision)
{
    logger lgr;
    fake_storage stor;
    handler handl(stor);
    lgr.add_handler(handl);

    int site = 42;

#define LOG(LEVEL, FMT, ...) UCLOG_BASIC_LOG(&lgr, LEVEL, FMT, __VA_ARGS__)
    LOG(info, "first %d", site);
#undef LOG

    EXPECT_EQ("first 42\n", stor.data);
}

TEST(basic_log, no_multiple_arg_evaluation)
{
    logger lgr;
    fake_storage stor;
    handler handl(stor);
    lgr.add_handler(handl);

    int x = 0;

    struct y_t
    {
        int operator()()
        {
            ++calls;
            return calls;
        }
        int calls = 0;
    };
    y_t y;

#define LOG(LEVEL, FMT, ...) UCLOG_BASIC_LOG(&lgr, LEVEL, FMT, __VA_ARGS__)
    LOG(info, "%d", ++x);
    LOG(info, "%d %d", x + x, x - x);
    LOG(info, "%d", y());
#undef LOG

    EXPECT_EQ(1, x);
    EXPECT_EQ(1, y.calls);
    EXPECT_EQ("1\n2 0\n1\n", stor.data);
}
