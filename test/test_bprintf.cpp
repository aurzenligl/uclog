#include <gtest/gtest.h>
#include <uclog/bprintf.hpp>
#include "bytes.hpp"

using namespace testing;
using namespace uclog;

// *** WARNING ***
// these tests will work only on little endian machines

template <typename T>
std::vector<uint8_t> snbprintf_to_vector(const char* fmt, T t)
{
    std::vector<uint8_t> out(1024);
    size_t written = snbprintf(out.data(), out.size(), fmt, t);
    out.resize(written);
    return out;
}

TEST(bprintf, prints_int)
{
    char ref[] = "\xd6\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff";

    EXPECT_EQ(bytes(&ref, 1), snbprintf_to_vector("%hhd", char(-42)));
    EXPECT_EQ(bytes(&ref, sizeof(short)), snbprintf_to_vector("%hd", short(-42)));
    EXPECT_EQ(bytes(&ref, sizeof(int)), snbprintf_to_vector("%d", -42));
    EXPECT_EQ(bytes(&ref, sizeof(long)), snbprintf_to_vector("%ld", -42L));
    EXPECT_EQ(bytes(&ref, sizeof(long long)), snbprintf_to_vector("%lld", -42LL));
}

TEST(bprintf, prints_other_int_codes)
{
    char ref[] = "\xd6\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff";

    EXPECT_EQ(bytes(&ref, 1), snbprintf_to_vector("%hhi", char(-42)));
    EXPECT_EQ(bytes(&ref, sizeof(short)), snbprintf_to_vector("%hi", short(-42)));
    EXPECT_EQ(bytes(&ref, sizeof(int)), snbprintf_to_vector("%i", -42));
    EXPECT_EQ(bytes(&ref, sizeof(long)), snbprintf_to_vector("%li", -42L));
    EXPECT_EQ(bytes(&ref, sizeof(long long)), snbprintf_to_vector("%lli", -42LL));

    EXPECT_EQ(bytes(&ref, 1), snbprintf_to_vector("%hhu", char(-42)));
    EXPECT_EQ(bytes(&ref, sizeof(short)), snbprintf_to_vector("%hu", short(-42)));
    EXPECT_EQ(bytes(&ref, sizeof(int)), snbprintf_to_vector("%u", -42));
    EXPECT_EQ(bytes(&ref, sizeof(long)), snbprintf_to_vector("%lu", -42L));
    EXPECT_EQ(bytes(&ref, sizeof(long long)), snbprintf_to_vector("%llu", -42LL));

    EXPECT_EQ(bytes(&ref, 1), snbprintf_to_vector("%hho", char(-42)));
    EXPECT_EQ(bytes(&ref, sizeof(short)), snbprintf_to_vector("%ho", short(-42)));
    EXPECT_EQ(bytes(&ref, sizeof(int)), snbprintf_to_vector("%o", -42));
    EXPECT_EQ(bytes(&ref, sizeof(long)), snbprintf_to_vector("%lo", -42L));
    EXPECT_EQ(bytes(&ref, sizeof(long long)), snbprintf_to_vector("%llo", -42LL));

    EXPECT_EQ(bytes(&ref, 1), snbprintf_to_vector("%hhx", char(-42)));
    EXPECT_EQ(bytes(&ref, sizeof(short)), snbprintf_to_vector("%hx", short(-42)));
    EXPECT_EQ(bytes(&ref, sizeof(int)), snbprintf_to_vector("%x", -42));
    EXPECT_EQ(bytes(&ref, sizeof(long)), snbprintf_to_vector("%lx", -42L));
    EXPECT_EQ(bytes(&ref, sizeof(long long)), snbprintf_to_vector("%llx", -42LL));

    EXPECT_EQ(bytes(&ref, 1), snbprintf_to_vector("%hhX", char(-42)));
    EXPECT_EQ(bytes(&ref, sizeof(short)), snbprintf_to_vector("%hX", short(-42)));
    EXPECT_EQ(bytes(&ref, sizeof(int)), snbprintf_to_vector("%X", -42));
    EXPECT_EQ(bytes(&ref, sizeof(long)), snbprintf_to_vector("%lX", -42L));
    EXPECT_EQ(bytes(&ref, sizeof(long long)), snbprintf_to_vector("%llX", -42LL));
}

TEST(bprintf, prints_char)
{
    uint8_t buf[1024] = {0};
    size_t written = snbprintf(buf, 1024, "%c", 42);
    EXPECT_EQ(1, written);
    EXPECT_EQ('\x2a', buf[0]);
}

TEST(bprintf, prints_pointer)
{
    uint8_t buf[1024] = {0};
    void* ptr = buf + 999;
    size_t written = snbprintf(buf, 1024, "%p", ptr);
    EXPECT_EQ(sizeof(void*), written);
    EXPECT_EQ(bytes(&ptr, sizeof(void*)), bytes(buf, sizeof(void*)));
}

TEST(bprintf, prints_string)
{
    uint8_t buf[1024] = {0};
    size_t written = snbprintf(buf, 1024, "%s", "the string");
    EXPECT_EQ(11, written);
    EXPECT_EQ(bytes("the string\0"), bytes(buf, 11));
}
