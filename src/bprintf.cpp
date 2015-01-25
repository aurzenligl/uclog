#include <uclog/bprintf.hpp>
#include <cstring>

namespace uclog
{

struct arg_spec
{
    char type; // one of: '\0', 'l', 'L', 'h', 'H', 'd'
};

static int arg_decode(const char* fmt, arg_spec* spec)
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

    if (fmt != start || !*fmt)
    {
        return fmt - start;
    }

    ++fmt;

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

    return ++fmt - start;
}

int vsnbprintf(char* buf, size_t size, const char* fmt, va_list args)
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

    arg_spec spec = {0};
    const char* start = buf;
    const char* end = buf + size;

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
        }
    }

    return buf - start;
}

int snbprintf(char* buf, size_t size, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int read = vsnbprintf(buf, size, fmt, args);
    va_end(args);
    return read;
}

} // namespace uclog
