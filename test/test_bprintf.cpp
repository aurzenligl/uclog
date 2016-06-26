#include <gtest/gtest.h>
#include <uclog/bprintf.hpp>
#include <inttypes.h>
#include "bytes.hpp"

using namespace testing;
using namespace uclog;

// *** WARNING ***
// 1. these tests will work only on little endian machines
// 2. SCN scanf macros used since they differentiate between i8, i16, i32

typedef std::vector<uint8_t> bytes_t;

template <typename... Ts>
static bytes_t snbprintf_to_limited_vector(size_t limit, const char* fmt, Ts... vs)
{
    bytes_t out(limit);
    size_t written = snbprintf(out.data(), out.size(), fmt, vs...);
    out.resize(written);
    return out;
}

template <typename... Ts>
static bytes_t snbprintf_to_vector(const char* fmt, Ts... vs)
{
    return snbprintf_to_limited_vector(1024, fmt, vs...);
}

TEST(bprintf, handles_empty_fmt)
{
    EXPECT_EQ(bytes(""), snbprintf_to_vector(""));
}

TEST(bprintf, handles_fmt_without_conversion_specifiers)
{
    EXPECT_EQ(bytes(""), snbprintf_to_vector("qwertyuiop[]asdfghjkl;'zxcvbnm,./1234567890-=!@#$^&*()_+ "));
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

TEST(bprintf, prints_int)
{
    char ref[] = "\xd6\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff";

    EXPECT_EQ(bytes(&ref, 1), snbprintf_to_vector("%hhd", char(-42))); // char gets promoted to int anyway
    EXPECT_EQ(bytes(&ref, sizeof(short)), snbprintf_to_vector("%hd", short(-42))); // short gets promoted to int anyway
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

TEST(bprintf, prints_float)
{
    EXPECT_EQ(bytes("\x00\x00\x28\xc2"), snbprintf_to_vector("%f", -42.f)); // float gets promoted to double anyway
    EXPECT_EQ(bytes("\x00\x00\x28\xc2"), snbprintf_to_vector("%f", -42.));
    EXPECT_EQ(bytes("\x00\x00\x00\x00\x00\x00\x45\xc0"), snbprintf_to_vector("%lf", -42.f)); // float gets promoted to double anyway
    EXPECT_EQ(bytes("\x00\x00\x00\x00\x00\x00\x45\xc0"), snbprintf_to_vector("%lf", -42.));
}

TEST(bprintf, prints_other_float_codes)
{
    EXPECT_EQ(bytes("\x00\x00\x28\xc2"), snbprintf_to_vector("%e", -42.));
    EXPECT_EQ(bytes("\x00\x00\x28\xc2"), snbprintf_to_vector("%g", -42.));
    EXPECT_EQ(bytes("\x00\x00\x28\xc2"), snbprintf_to_vector("%a", -42.));
    EXPECT_EQ(bytes("\x00\x00\x28\xc2"), snbprintf_to_vector("%F", -42.));
    EXPECT_EQ(bytes("\x00\x00\x28\xc2"), snbprintf_to_vector("%E", -42.));
    EXPECT_EQ(bytes("\x00\x00\x28\xc2"), snbprintf_to_vector("%G", -42.));
    EXPECT_EQ(bytes("\x00\x00\x28\xc2"), snbprintf_to_vector("%A", -42.));
    EXPECT_EQ(bytes("\x00\x00\x00\x00\x00\x00\x45\xc0"), snbprintf_to_vector("%le", -42.));
    EXPECT_EQ(bytes("\x00\x00\x00\x00\x00\x00\x45\xc0"), snbprintf_to_vector("%lg", -42.));
    EXPECT_EQ(bytes("\x00\x00\x00\x00\x00\x00\x45\xc0"), snbprintf_to_vector("%la", -42.));
    EXPECT_EQ(bytes("\x00\x00\x00\x00\x00\x00\x45\xc0"), snbprintf_to_vector("%lF", -42.));
    EXPECT_EQ(bytes("\x00\x00\x00\x00\x00\x00\x45\xc0"), snbprintf_to_vector("%lE", -42.));
    EXPECT_EQ(bytes("\x00\x00\x00\x00\x00\x00\x45\xc0"), snbprintf_to_vector("%lG", -42.));
    EXPECT_EQ(bytes("\x00\x00\x00\x00\x00\x00\x45\xc0"), snbprintf_to_vector("%lA", -42.));
}

TEST(bprintf, omits_flags)
{
    EXPECT_EQ(bytes("\xd6\xff\xff\xff"), snbprintf_to_vector("%-" SCNi32, int32_t(-42)));
    EXPECT_EQ(bytes("\xd6\xff\xff\xff"), snbprintf_to_vector("%+" SCNi32, int32_t(-42)));
    EXPECT_EQ(bytes("\xd6\xff\xff\xff"), snbprintf_to_vector("% " SCNi32, int32_t(-42)));
    EXPECT_EQ(bytes("\xd6\xff\xff\xff"), snbprintf_to_vector("%#" SCNi32, int32_t(-42)));
    EXPECT_EQ(bytes("\xd6\xff\xff\xff"), snbprintf_to_vector("%0" SCNi32, int32_t(-42)));

    EXPECT_EQ(bytes("\xd6\xff\xff\xff"), snbprintf_to_vector("%-+" SCNi32, int32_t(-42)));

    EXPECT_EQ(bytes("\xd6\xff\xff\xff"), snbprintf_to_vector("%-+ #0" SCNi32, int32_t(-42)));
}

TEST(bprintf, omits_width_and_precision)
{
    EXPECT_EQ(bytes("\xd6\xff\xff\xff"), snbprintf_to_vector("%8" SCNi32, int32_t(-42)));
    EXPECT_EQ(bytes("\xd6\xff\xff\xff"), snbprintf_to_vector("%159" SCNi32, int32_t(-42)));

    EXPECT_EQ(bytes("\xd6\xff\xff\xff"), snbprintf_to_vector("%.8" SCNi32, int32_t(-42)));
    EXPECT_EQ(bytes("\xd6\xff\xff\xff"), snbprintf_to_vector("%.159" SCNi32, int32_t(-42)));

    EXPECT_EQ(bytes("\xd6\xff\xff\xff"), snbprintf_to_vector("%8.8" SCNi32, int32_t(-42)));
    EXPECT_EQ(bytes("\xd6\xff\xff\xff"), snbprintf_to_vector("%159.159" SCNi32, int32_t(-42)));
}

TEST(bprintf, omits_verbatim_percent_sign)
{
    EXPECT_EQ(bytes(""), snbprintf_to_vector("%%"));
    EXPECT_EQ(bytes("\xd6\xff\xff\xff"), snbprintf_to_vector("%%%" SCNi32 "%%", int32_t(-42)));
}

TEST(bprintf, handles_invalid_percent_sign_usage_gracefully)
{
    EXPECT_EQ(bytes(""), snbprintf_to_vector("#$%^&"));
}

TEST(bprintf, handles_overflow_int_gracefully)
{
    EXPECT_EQ(bytes(""), snbprintf_to_limited_vector(0, "%" SCNi32, int32_t(-42)));
    EXPECT_EQ(bytes(""), snbprintf_to_limited_vector(1, "%" SCNi32, int32_t(-42)));
    EXPECT_EQ(bytes(""), snbprintf_to_limited_vector(2, "%" SCNi32, int32_t(-42)));
    EXPECT_EQ(bytes(""), snbprintf_to_limited_vector(3, "%" SCNi32, int32_t(-42)));
    EXPECT_EQ(bytes("\xd6\xff\xff\xff"), snbprintf_to_limited_vector(4, "%" SCNi32, int32_t(-42)));
}

TEST(bprintf, handles_overflow_string_gracefully)
{
    EXPECT_EQ(bytes(""), snbprintf_to_limited_vector(0, "%s", "abcd"));
    EXPECT_EQ(bytes(""), snbprintf_to_limited_vector(4, "%s", "abcd"));
    EXPECT_EQ(bytes("abcd\x00"), snbprintf_to_limited_vector(5, "%s", "abcd"));
}

TEST(bprintf, prints_multiple_arguments)
{
    EXPECT_EQ(
        bytes("\xd6\xff\xff\xff\xd6\xff\xff\xff\xd6\xff\xff\xff"),
        snbprintf_to_vector("%" SCNi32 "%" SCNi32 "%" SCNi32, int32_t(-42), int32_t(-42), int32_t(-42))
    );
}

TEST(bprintf, prints_heterogenous_arguments)
{
    EXPECT_EQ(
        bytes(
            "\xd6\xff\xff\xff"
            "\xd6\xff"
            "\xd6"
            "abcd\x00"
            "\x00\x00\x28\xc2"
            "\x00\x00\x00\x00\x00\x00\x45\xc0"
        ),
        snbprintf_to_vector(
            "i32 %" SCNi32 " i16 %" SCNi16 " i8 %" SCNi8 " string %s float %f double %lf",
            int32_t(-42), int32_t(-42), int32_t(-42), "abcd", -42., -42.
        )
    );
}

template <typename... Ts>
static bytes_t args_encode_to_limit_vec(size_t limit, const std::vector<arg_type>& types, Ts... vs)
{
    bytes_t out(limit);
    size_t written = args_encode(out.data(), out.size(), types.data(), types.size(), vs...);
    out.resize(written);
    return out;
}

template <typename... Ts>
static bytes_t args_encode_to_vec(const std::vector<arg_type>& types, Ts... vs)
{
    return args_encode_to_limit_vec(1024, types, vs...);
}

template <typename T>
static bytes_t to_bytes(T t)
{
    bytes_t buf(sizeof(T));
    memcpy(buf.data(), &t, sizeof(T));
    return buf;
}

static bytes_t str_to_bytes(std::string t)
{
    bytes_t buf(t.size() + 1);
    memcpy(buf.data(), t.c_str(), t.size() + 1);
    return buf;
}

static bytes_t operator+(const bytes_t& x, const bytes_t& y)
{
    bytes_t z(x);
    z.insert(z.end(), y.begin(), y.end());
    return z;
}

TEST(bprintf, encodes_single_arg)
{
    EXPECT_EQ(to_bytes<char>(1), args_encode_to_vec({arg_type_char}, 1));
    EXPECT_EQ(to_bytes<short>(2), args_encode_to_vec({arg_type_short}, 2));
    EXPECT_EQ(to_bytes<int>(3), args_encode_to_vec({arg_type_int}, 3));
    EXPECT_EQ(to_bytes<long>(4), args_encode_to_vec({arg_type_long}, 4));
    EXPECT_EQ(to_bytes<long long>(5), args_encode_to_vec({arg_type_long_long}, 5));
    EXPECT_EQ(to_bytes<float>(6.), args_encode_to_vec({arg_type_float}, 6.));
    EXPECT_EQ(to_bytes<double>(6.), args_encode_to_vec({arg_type_double}, 6.));
    EXPECT_EQ(to_bytes<void*>(nullptr), args_encode_to_vec({arg_type_pointer}, nullptr));
    EXPECT_EQ(str_to_bytes("abc"), args_encode_to_vec({arg_type_string}, "abc"));
    EXPECT_EQ(bytes(""), args_encode_to_vec({arg_type_none}));
}

TEST(bprintf, encodes_multiple_args)
{
    EXPECT_EQ(
        to_bytes<char>(1) + to_bytes<long long>(5) + str_to_bytes("abc") + to_bytes<float>(6.),
        args_encode_to_vec({arg_type_char, arg_type_long_long, arg_type_string, arg_type_float}, 1, 5, "abc", 6.));
}
