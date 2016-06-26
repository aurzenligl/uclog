#include <uclog/bprintf.hpp>
#include <stdint.h>
#include <cstring>
#include <cctype>

namespace uclog
{
namespace
{

struct stream
{
    stream(uint8_t* begin, const uint8_t* end): begin_(begin), cur_(begin), end_(end)
    { }

    template <typename T>
    void put(T x)
    {
        if (sizeof(T) > left())
        {
            return;
        }

        memcpy(cur_, &x, sizeof(T));
        cur_ += sizeof(T);
    }

    void put(const char* str)
    {
        size_t len = strlen(str) + 1;
        if (len > left())
        {
            return;
        }

        memcpy(cur_, str, len);
        cur_ += len;
    }

    size_t written() const
    {
        return cur_ - begin_;
    }

    size_t left() const
    {
        return end_ - cur_;
    }

    const uint8_t* begin_;
    uint8_t* cur_;
    const uint8_t* end_;
};

}  // unnamed namespace

size_t arg_decode(const char* fmt, arg_spec* spec)
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

    ++fmt;
    if (*fmt == '%')
    {
        return fmt - start;
    }

    for (; *fmt; ++fmt)
    {
        if (((*fmt >= 'a') && (*fmt <= 'z')) || ((*fmt >= 'A') && (*fmt <= 'Z')))
        {
            break;
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
    default:
        spec->type = arg_type_none;
        return fmt - start;
    }
}

/// due to "default argument promotions" as described in 6.5.2.2/6,
/// char and short arguments are promoted to int
#define pull_int_arg(type)                                                     \
    ((sizeof(type) == sizeof(unsigned long long)) ?                            \
        static_cast<type>(va_arg(args, unsigned long long)) :                  \
        static_cast<type>(va_arg(args, int)))

/// due to "default argument promotions" as described in 6.5.2.2/6,
/// float is promoted to double
#define pull_float_arg(type)                                                   \
    (static_cast<type>(va_arg(args, double)))

size_t vargs_encode(void* buf, size_t size, const arg_type* types, size_t ntypes, va_list args)
{
    stream str(static_cast<uint8_t*>(buf), static_cast<uint8_t*>(buf) + size);

    for (size_t i = 0; i < ntypes; ++i)
    {
        switch (types[i])
        {
        case arg_type_none:
            break;
        case arg_type_char:
            str.put(pull_int_arg(char));
            break;
        case arg_type_short:
            str.put(pull_int_arg(short));
            break;
        case arg_type_int:
            str.put(pull_int_arg(int));
            break;
        case arg_type_long:
            str.put(pull_int_arg(long));
            break;
        case arg_type_long_long:
            str.put(pull_int_arg(long long));
            break;
        case arg_type_float:
            str.put(pull_float_arg(float));
            break;
        case arg_type_double:
            str.put(pull_float_arg(double));
            break;
        case arg_type_pointer:
            str.put(pull_int_arg(uintptr_t));
            break;
        case arg_type_string:
            str.put(va_arg(args, const char*));
            break;
        }
    }

    return str.written();
}

size_t vsnbprintf(void* buf, size_t size, const char* fmt, va_list args)
{
    stream str(static_cast<uint8_t*>(buf), static_cast<uint8_t*>(buf) + size);

    while (*fmt)
    {
        arg_spec spec;
        int read = arg_decode(fmt, &spec);
        fmt += read;

        switch (spec.type)
        {
        case arg_type_none:
            break;
        case arg_type_char:
            str.put(pull_int_arg(char));
            break;
        case arg_type_short:
            str.put(pull_int_arg(short));
            break;
        case arg_type_int:
            str.put(pull_int_arg(int));
            break;
        case arg_type_long:
            str.put(pull_int_arg(long));
            break;
        case arg_type_long_long:
            str.put(pull_int_arg(long long));
            break;
        case arg_type_float:
            str.put(pull_float_arg(float));
            break;
        case arg_type_double:
            str.put(pull_float_arg(double));
            break;
        case arg_type_pointer:
            str.put(pull_int_arg(uintptr_t));
            break;
        case arg_type_string:
            str.put(va_arg(args, const char*));
            break;
        }
    }

    return str.written();
}

size_t args_encode(void* buf, size_t size, const arg_type* types, size_t ntypes, ...)
{
    va_list args;
    va_start(args, ntypes);
    size_t written = vargs_encode(buf, size, types, ntypes, args);
    va_end(args);
    return written;
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
