#ifndef UCLOG_BPRINTF_HPP_
#define UCLOG_BPRINTF_HPP_

#include <cstddef>
#include <cstdarg>
#include <stdint.h>

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

size_t snbprintf(uint8_t* buf, size_t size, const char* fmt, ...);

size_t vsnbprintf(uint8_t* buf, size_t size, const char* fmt, va_list args);

} // namespace uclog

#endif /* UCLOG_BPRINTF_HPP_ */
