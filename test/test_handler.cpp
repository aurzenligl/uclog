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
