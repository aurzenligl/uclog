#include <gtest/gtest.h>
#include <uclog/handler.hpp>
#include "fake_storage.hpp"

using namespace testing;
using namespace uclog;

TEST(handler, initializes_level)
{
    storage* fake_stor = 0;

    EXPECT_EQ(level_debug, handler().level());
    EXPECT_EQ(level_debug, handler(*fake_stor).level());
    EXPECT_EQ(level_critical, handler(*fake_stor, level_critical).level());
}

TEST(handler, sets_level)
{
    handler h;
    h.set_level(level_info);
    EXPECT_EQ(level_info, h.level());
}

static void log(handler& handler, level_t level, const char* fmt, ...)
{
    site_t site;
    site.level = level;
    site.fmt = fmt;

    va_list args;
    va_start(args, fmt);
    handler.log(site, args);
    va_end(args);
}

TEST(handler, logs_when_level_not_set)
{
    fake_storage storage;
    handler h(storage);

    log(h, level_debug, "test %d", 42);

    EXPECT_EQ("test 42", storage.data);
}

TEST(handler, logs_when_level_not_lower)
{
    fake_storage storage;
    handler h(storage, level_warning);

    log(h, level_debug, "a");
    log(h, level_info, "b");
    log(h, level_warning, "c");
    log(h, level_error, "d");
    log(h, level_critical, "e");

    EXPECT_EQ("cde", storage.data);
}
