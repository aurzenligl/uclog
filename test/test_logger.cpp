#include <gtest/gtest.h>
#include <uclog/logger.hpp>
#include "fake_storage.hpp"

using namespace testing;
using namespace uclog;

TEST(logger, initializes_level)
{
    logger fake_parent;

    EXPECT_EQ(level_debug, logger().level());
    EXPECT_EQ(level_warning, logger(level_warning).level());
}

TEST(logger, sets_level)
{
    logger lgr;
    lgr.set_level(level_info);
    EXPECT_EQ(level_info, lgr.level());
}

TEST(logger, logs_to_single_handler)
{
    fake_storage storage;
    handler h(storage);
    logger lgr(level_warning);
    lgr.add_handler(h);

    lgr.log(level_warning, "test %d", 42);

    EXPECT_EQ("test 42", storage.data);
}

TEST(logger, logs_to_multiple_handlers)
{
    fake_storage storage;
    handler h1(storage);
    handler h2(storage);
    logger lgr(level_warning);
    lgr.add_handler(h1);
    lgr.add_handler(h2);

    lgr.log(level_warning, "test %d ", 42);

    EXPECT_EQ("test 42 test 42 ", storage.data);
}

TEST(logger, filters_when_level_lower)
{
    fake_storage storage;
    handler h(storage);
    logger lgr(level_warning);
    lgr.add_handler(h);

    lgr.log(level_debug, "test %d ", 42);
    EXPECT_EQ("", storage.read_and_clear());
    lgr.log(level_info, "test %d ", 42);
    EXPECT_EQ("", storage.read_and_clear());
    lgr.log(level_warning, "test %d ", 42);
    EXPECT_EQ("test 42 ", storage.read_and_clear());
    lgr.log(level_error, "test %d ", 42);
    EXPECT_EQ("test 42 ", storage.read_and_clear());
    lgr.log(level_critical, "test %d ", 42);
    EXPECT_EQ("test 42 ", storage.read_and_clear());
}
