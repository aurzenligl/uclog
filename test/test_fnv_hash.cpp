#include <gtest/gtest.h>
#include <uclog/fnv_hash.hpp>

using namespace testing;
using namespace uclog;

TEST(fnv_hash, hash)
{
    EXPECT_NE(fnv_hash("abc"), fnv_hash("def"));
}
