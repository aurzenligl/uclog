#include <uclog/bprintf.hpp>
#include <stdint.h>
#include <cstring>
#include <cctype>

namespace uclog
{

enum arg_type
{
    arg_type_none,
    arg_type_char,
    arg_type_short,
    arg_type_int,
    arg_type_long,
    arg_type_long_long,
    arg_type_pointer,
    arg_type_string,
    arg_type_float,
    arg_type_double
};

struct arg_spec
{
    arg_type type;
};

static size_t arg_decode(const char* fmt, arg_spec* spec)
{
    const char* start = fmt;

    spec->type = arg_type_none;

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

    spec->type = arg_type_int;
    if (*fmt == 'h' || *fmt == 'l')
    {
        char qualifier = *fmt++;
        if (qualifier == *fmt)
        {
            spec->type = (qualifier == 'h') ? arg_type_char : arg_type_long_long;
            ++fmt;
        }
        else
        {
            spec->type = (qualifier == 'h') ? arg_type_short : arg_type_long;
        }
    }

    switch (*fmt)
    {
    case 'c':
        spec->type = arg_type_char;
        return ++fmt - start;
    case 's':
        spec->type = arg_type_string;
        return ++fmt - start;
    case 'p':
        spec->type = arg_type_pointer;
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
        spec->type = (spec->type == arg_type_long) ? arg_type_double : arg_type_float;
        return ++fmt - start;
    case '%':
        spec->type = arg_type_none;
        return ++fmt - start;
    default:
        spec->type = arg_type_none;
        return fmt - start;
    }
}

size_t vsnbprintf(void* buf, size_t size, const char* fmt, va_list args)
{
    /// due to "default argument promotions" as described in 6.5.2.2/6,
    /// char and short arguments are promoted to int
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
    if (cur + sizeof(type) <= end)                      \
    {                                                   \
        memcpy(cur, &value, sizeof(type));              \
        cur += sizeof(type);                            \
    }                                                   \
} while (0)

    /// due to "default argument promotions" as described in 6.5.2.2/6,
    /// float is promoted to double
#define save_float_arg(type)                            \
do                                                      \
{                                                       \
    type value = va_arg(args, double);                  \
    if (cur + sizeof(type) <= end)                      \
    {                                                   \
        memcpy(cur, &value, sizeof(type));              \
        cur += sizeof(type);                            \
    }                                                   \
} while (0)

    arg_spec spec = { arg_type_none };
    const uint8_t* start = static_cast<const uint8_t*>(buf);
    const uint8_t* end = static_cast<const uint8_t*>(buf) + size;
    uint8_t* cur = static_cast<uint8_t*>(buf);

    while (*fmt)
    {
        int read = arg_decode(fmt, &spec);

        fmt += read;

        switch (spec.type)
        {
        case arg_type_none:
            break;
        case arg_type_char:
            save_arg(char);
            break;
        case arg_type_short:
            save_arg(short);
            break;
        case arg_type_int:
            save_arg(int);
            break;
        case arg_type_long:
            save_arg(long);
            break;
        case arg_type_long_long:
            save_arg(long long);
            break;
        case arg_type_float:
            save_float_arg(float);
            break;
        case arg_type_double:
            save_float_arg(double);
            break;
        case arg_type_pointer:
            save_arg(uintptr_t);
            break;
        case arg_type_string:
            {
                const char* str = va_arg(args, const char*);
                size_t len = strlen(str) + 1;
                if (cur + len <= end)
                {
                    memcpy(cur, str, len);
                    cur += len;
                }
                break;
            }
        }
    }

    return cur - start;
#undef save_arg
#undef save_float_arg
}

size_t snbprintf(void* buf, size_t size, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    size_t written = vsnbprintf(buf, size, fmt, args);
    va_end(args);
    return written;
}

} // namespace uclog
