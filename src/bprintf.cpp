#include <uclog/bprintf.hpp>
#include <stdint.h>
#include <cstring>
#include <cctype>

namespace uclog
{

struct arg_spec
{
    char type; // one of: '\0', 'l', 'L', 'h', 'H', 'd'
};

static size_t arg_decode(const char* fmt, arg_spec* spec)
{
    const char* start = fmt;

    spec->type = 0;

    for (; *fmt ; ++fmt)
    {
        if (*fmt == '%')
        {
            break;
        }
    }

    if (!*fmt)
    {
        return fmt - start;
    }

    // flags
    for (bool found = true; found;)
    {
        ++fmt;

        switch (*fmt)
        {
        case '-':
        case '+':
        case ' ':
        case '#':
        case '0':
            break;
        default:
            found = false;
            break;
        }
    }

    // width
    while (isdigit(*fmt))
    {
        ++fmt;
    }

    // precision
    if (*fmt == '.')
    {
        ++fmt;
        while (isdigit(*fmt))
        {
            ++fmt;
        }
    }

    spec->type = 'd';
    if (*fmt == 'h' || *fmt == 'l')
    {
        spec->type = *fmt++;
        if (spec->type == *fmt)
        {
            if (spec->type == 'l')
            {
                spec->type = 'L';
                ++fmt;
            }
            else if (spec->type == 'h')
            {
                spec->type = 'H';
                ++fmt;
            }
        }
    }

    switch (*fmt)
    {
    case 'c':
        spec->type = 'H';
        return ++fmt - start;
    case 's':
        spec->type = 's';
        return ++fmt - start;
    case 'p':
        spec->type = 'p';
        return ++fmt - start;
    case 'o':
    case 'x':
    case 'X':
    case 'd':
    case 'i':
    case 'u':
        // type of integer has already been determined
        return ++fmt - start;
    case 'f':
    case 'e':
    case 'g':
    case 'a':
    case 'F':
    case 'E':
    case 'G':
    case 'A':
        // slight deviation from standard:
        // %f treated as float, %lf as double,
        // like in scanf functions
        spec->type = (spec->type == 'l') ? 'F' : 'f';
        return ++fmt - start;
    case '%':
        spec->type = 0;
        return ++fmt - start;
    default:
        spec->type = 0;
        return fmt - start;
    }
}

size_t vsnbprintf(uint8_t* buf, size_t size, const char* fmt, va_list args)
{
    /// 6.5.2.2/6 defines "default argument promotions",
    /// and /7 states that default argument promotions
    /// are applied to "trailing arguments", that is varargs denoted by ....
#define save_arg(type)                                  \
do                                                      \
{                                                       \
    type value;                                         \
    if (sizeof(type) == sizeof(unsigned long long))     \
    {                                                   \
        value = va_arg(args, unsigned long long);       \
    }                                                   \
    else                                                \
    {                                                   \
        value = va_arg(args, int);                      \
    }                                                   \
    if (buf + sizeof(type) <= end)                      \
    {                                                   \
        memcpy(buf, &value, sizeof(type));              \
        buf += sizeof(type);                            \
    }                                                   \
} while (0)

#define save_float_arg(type)                            \
do                                                      \
{                                                       \
    type value = va_arg(args, double);                  \
    if (buf + sizeof(type) <= end)                      \
    {                                                   \
        memcpy(buf, &value, sizeof(type));              \
        buf += sizeof(type);                            \
    }                                                   \
} while (0)

    arg_spec spec = {0};
    const uint8_t* start = buf;
    const uint8_t* end = buf + size;

    while (*fmt)
    {
        int read = arg_decode(fmt, &spec);

        fmt += read;

        switch (spec.type)
        {
        case 'L':
            save_arg(long long);
            break;
        case 'l':
            save_arg(long);
            break;
        case 'H':
            save_arg(char);
            break;
        case 'h':
            save_arg(short);
            break;
        case 'd':
            save_arg(int);
            break;
        case 'f':
            save_float_arg(float);
            break;
        case 'F':
            save_float_arg(double);
            break;
        case 'p':
            save_arg(uintptr_t);
            break;
        case 's':
            const char* str = va_arg(args, const char*);
            while (*str && (buf < end))
            {
                *buf++ = *str++;
            }
            *buf++ = 0;
            break;
        }
    }

    return buf - start;
#undef save_arg
#undef save_float_arg
}

size_t snbprintf(uint8_t* buf, size_t size, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    size_t read = vsnbprintf(buf, size, fmt, args);
    va_end(args);
    return read;
}

} // namespace uclog
