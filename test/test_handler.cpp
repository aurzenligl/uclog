#include <gtest/gtest.h>
#include <uclog/handler.hpp>

using namespace testing;
using namespace uclog;

TEST(handler, initializes_level)
{
    storage* fake_stor = 0;

    EXPECT_EQ(level_not_set, handler().level());
    EXPECT_EQ(level_not_set, handler(*fake_stor).level());
    EXPECT_EQ(level_critical, handler(*fake_stor, level_critical).level());
}

TEST(handler, sets_level)
{
    handler h;
    h.set_level(level_info);
    EXPECT_EQ(level_info, h.level());
}

struct test_storage : public storage
{
    virtual void store(const logger_info& logger, level_t level, const char* fmt, va_list args)
    {
        char buf[1024];
        int size = vsnprintf(buf, 1024, fmt, args);
        data.insert(data.end(), buf, buf + size);
    }

    std::string data;
};

static void log(handler& handler, const logger_info& logger, level_t level, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    handler.log(logger, level, fmt, args);
    va_end(args);
}

TEST(handler, logs_when_level_not_set)
{
    test_storage storage;
    logger_info info;
    handler h(storage);

    log(h, info, level_debug, "test %d", 42);

    EXPECT_EQ("test 42", storage.data);
}

TEST(handler, logs_when_level_not_lower)
{
    test_storage storage;
    logger_info info;
    handler h(storage, level_warning);

    log(h, info, level_debug, "a");
    log(h, info, level_info, "b");
    log(h, info, level_warning, "c");
    log(h, info, level_error, "d");
    log(h, info, level_critical, "e");

    EXPECT_EQ("cde", storage.data);
}
