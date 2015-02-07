#include <gtest/gtest.h>
#include <uclog/logger.hpp>
#include "fake_storage.hpp"

using namespace testing;
using namespace uclog;

static logger_info test_info("test_info", 42);

TEST(logger, initializes_level)
{
    logger fake_parent;

    EXPECT_EQ(level_not_set, logger().level());
    EXPECT_EQ(level_not_set, logger(test_info).level());
    EXPECT_EQ(level_not_set, logger(test_info, fake_parent).level());
    EXPECT_EQ(level_warning, logger(test_info, fake_parent, level_warning).level());
    EXPECT_EQ(level_warning, logger(test_info, level_warning).level());
}

TEST(logger, initializes_info)
{
    logger fake_parent;

    EXPECT_EQ(std::string(""), logger().name());
    EXPECT_EQ(std::string("test_info"), logger(test_info).name());
    EXPECT_EQ(std::string("test_info"), logger(test_info, fake_parent).name());
    EXPECT_EQ(std::string("test_info"), logger(test_info, fake_parent, level_warning).name());
    EXPECT_EQ(std::string("test_info"), logger(test_info, level_warning).name());

    EXPECT_EQ(0, logger().id());
    EXPECT_EQ(42, logger(test_info).id());
    EXPECT_EQ(42, logger(test_info, fake_parent).id());
    EXPECT_EQ(42, logger(test_info, fake_parent, level_warning).id());
    EXPECT_EQ(42, logger(test_info, level_warning).id());
}

TEST(logger, sets_level)
{
    logger lgr;
    lgr.set_level(level_info);
    EXPECT_EQ(level_info, lgr.level());
}

TEST(logger, logs)
{
    fake_storage storage;
    handler h(storage);
    logger lgr(test_info, level_warning);
    lgr.add_handler(h);

    lgr.log(level_warning, "test %d", 42);

    EXPECT_EQ("test 42", storage.data);
}
