#include <gtest/gtest.h>
#include <uclog/bprintf.hpp>
#include "bytes.hpp"

using namespace testing;
using namespace uclog;

// *** WARNING ***
// these tests will work only on little endian machines

TEST(bprintf, prints_int)
{
    char buf[1024];
    int written = snbprintf(buf, 1024, "%d", 42);
    EXPECT_EQ(4, written);
    EXPECT_EQ(bytes("\x2a\x00\x00\x00"), bytes(&buf, 4));
}
