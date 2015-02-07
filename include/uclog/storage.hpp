#ifndef UCLOG_STORAGE_HPP_
#define UCLOG_STORAGE_HPP_

#include <cstdarg>
#include <uclog/attributes.hpp>

namespace uclog
{

struct storage
{
    virtual ~storage() { }
    virtual void store(const logger_info& logger, level_t level, const char* fmt, va_list args) = 0;
};

} // namespace uclog

#endif /* UCLOG_STORAGE_HPP_ */
