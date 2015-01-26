#include <gtest/gtest.h>
#include <uclog/bprintf.hpp>
#include "bytes.hpp"

using namespace testing;
using namespace uclog;

// *** WARNING ***
// these tests will work only on little endian machines

TEST(bprintf, prints_int)
{
    char buf[1024] = {0};
    char ref[] = "\xd6\xff\xff\xff\xff\xff\xff\xff";
    int written;

    written = snbprintf(buf, 1024, "%hhd", -42);
    EXPECT_EQ(1, written);
    EXPECT_EQ(bytes(&ref, 1), bytes(&buf, 1));

    written = snbprintf(buf, 1024, "%hd", -42);
    EXPECT_EQ(sizeof(short), written);
    EXPECT_EQ(bytes(&ref, sizeof(short)), bytes(&buf, sizeof(short)));

    written = snbprintf(buf, 1024, "%d", -42);
    EXPECT_EQ(sizeof(int), written);
    EXPECT_EQ(bytes(&ref, sizeof(int)), bytes(&buf, sizeof(int)));

    written = snbprintf(buf, 1024, "%ld", -42L);
    EXPECT_EQ(sizeof(long), written);
    EXPECT_EQ(bytes(&ref, sizeof(long)), bytes(&buf, sizeof(long)));

    written = snbprintf(buf, 1024, "%lld", -42LL);
    EXPECT_EQ(sizeof(long long), written);
    EXPECT_EQ(bytes(&ref, sizeof(long long)), bytes(&buf, sizeof(long long)));
}
}
