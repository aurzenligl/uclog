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

/// Reads format string until argument specifier is encountered.
/// Always writes specifier to spec, even if no specifier is encountered.
/// Returns number of format string characters read.
///
size_t arg_decode(const char* fmt, arg_spec* spec);

/// Writes variadic arguments to buf in binary, in order, without padding in between.
/// Buf will only be written up to size bytes.
/// Args parameter is an array of types or variadic arguments.
/// Returns number of bytes written to buf.
///
size_t args_encode(void* buf, size_t size, const arg_type* types, size_t ntypes, ...);

size_t vargs_encode(void* buf, size_t size, const arg_type* types, size_t ntypes, va_list args);

size_t snbprintf(void* buf, size_t size, const char* fmt, ...);

size_t vsnbprintf(void* buf, size_t size, const char* fmt, va_list args);

} // namespace uclog

#endif /* UCLOG_BPRINTF_HPP_ */
