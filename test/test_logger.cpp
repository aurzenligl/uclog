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

TEST(logger, initializes_propagate)
{
    logger fake_parent;

    EXPECT_EQ(true, logger().propagate());
    EXPECT_EQ(true, logger(test_info).propagate());
    EXPECT_EQ(true, logger(test_info, fake_parent).propagate());
    EXPECT_EQ(true, logger(test_info, fake_parent, level_warning).propagate());
    EXPECT_EQ(true, logger(test_info, level_warning).propagate());
}

TEST(logger, sets_level)
{
    logger lgr;
    lgr.set_level(level_info);
    EXPECT_EQ(level_info, lgr.level());
}

TEST(logger, sets_propagate)
{
    logger lgr;
    lgr.set_propagate(false);
    EXPECT_EQ(false, lgr.propagate());
}

TEST(logger, logs_to_single_handler)
{
    fake_storage storage;
    handler h(storage);
    logger lgr(test_info, level_warning);
    lgr.add_handler(h);

    lgr.log(level_warning, "test %d", 42);

    EXPECT_EQ("test 42", storage.data);
}

TEST(logger, logs_to_multiple_handlers)
{
    fake_storage storage;
    handler h1(storage);
    handler h2(storage);
    logger lgr(test_info, level_warning);
    lgr.add_handler(h1);
    lgr.add_handler(h2);

    lgr.log(level_warning, "test %d ", 42);

    EXPECT_EQ("test 42 test 42 ", storage.data);
}

TEST(logger, propagates_to_parent_handler)
{
    fake_storage storage;
    handler h(storage);
    logger parent(test_info, level_warning);
    logger lgr(test_info, parent, level_warning);
    parent.add_handler(h);

    lgr.log(level_warning, "test %d ", 42);

    EXPECT_EQ("test 42 ", storage.data);
}

TEST(logger, does_not_propagate_to_parent_handler)
{
    fake_storage storage;
    handler h(storage);
    logger parent(test_info, level_warning);
    logger lgr(test_info, parent, level_warning);
    parent.add_handler(h);

    lgr.set_propagate(false);
    lgr.log(level_warning, "test %d ", 42);

    EXPECT_EQ("", storage.data);
}

TEST(logger, propagates_to_multiple_parents_handlers)
{
    fake_storage storage;
    handler h(storage);
    logger lgr1(test_info, level_warning);
    logger lgr2(test_info, lgr1, level_warning);
    logger lgr3(test_info, lgr2, level_warning);
    lgr1.add_handler(h);
    lgr2.add_handler(h);

    lgr3.log(level_warning, "test %d ", 42);

    EXPECT_EQ("test 42 test 42 ", storage.data);
}

TEST(logger, propagates_to_multiple_parents_handlers_even_if_parents_do_not_propagate)
{
    fake_storage storage;
    handler h(storage);
    logger lgr1(test_info, level_warning);
    logger lgr2(test_info, lgr1, level_warning);
    logger lgr3(test_info, lgr2, level_warning);
    lgr1.add_handler(h);
    lgr2.add_handler(h);

    lgr1.set_propagate(false);
    lgr2.set_propagate(false);
    lgr3.log(level_warning, "test %d ", 42);

    EXPECT_EQ("test 42 test 42 ", storage.data);
}

TEST(logger, filters_when_level_lower)
{
    fake_storage storage;
    handler h(storage);
    logger lgr(test_info, level_warning);
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

TEST(logger, filters_with_parent_level_when_not_set)
{
    fake_storage storage;
    handler h(storage);
    logger lgr1(test_info, level_warning);
    logger lgr2(test_info, lgr1);
    logger lgr3(test_info, lgr2);
    lgr1.add_handler(h);

    lgr3.log(level_info, "test %d ", 42);
    EXPECT_EQ("", storage.read_and_clear());
    lgr3.log(level_warning, "test %d ", 42);
    EXPECT_EQ("test 42 ", storage.read_and_clear());
}

TEST(logger, filters_when_level_not_set)
{
    fake_storage storage;
    handler h(storage);
    logger lgr(test_info);
    lgr.add_handler(h);

    lgr.log(level_debug, "test %d ", 42);
    EXPECT_EQ("", storage.read_and_clear());
    lgr.log(level_critical, "test %d ", 42);
    EXPECT_EQ("", storage.read_and_clear());
}
