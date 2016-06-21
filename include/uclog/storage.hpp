#ifndef UCLOG_STORAGE_HPP_
#define UCLOG_STORAGE_HPP_

#include <cstdarg>
#include <uclog/level.hpp>

namespace uclog
{

struct storage
{
    virtual void store(level_t level, const char* fmt, va_list args) = 0;
};

} // namespace uclog

#endif /* UCLOG_STORAGE_HPP_ */
