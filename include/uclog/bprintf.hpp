#ifndef UCLOG_BPRINTF_HPP_
#define UCLOG_BPRINTF_HPP_

#include <cstddef>
#include <cstdarg>

namespace uclog
{

/*
 * Conversion specifiers %f %e %g %a are serialized as floats, %lf %le %lg %la as doubles.
 *
 * Not implemented:
 * - * to dynamically specify width and precision
 * - %n conversion specifier
 * - long double formats
 */

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

size_t arg_decode(const char* fmt, arg_spec* spec);

size_t snbprintf(void* buf, size_t size, const char* fmt, ...);

size_t vsnbprintf(void* buf, size_t size, const char* fmt, va_list args);

} // namespace uclog

#endif /* UCLOG_BPRINTF_HPP_ */
