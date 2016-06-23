#ifndef UCLOG_STORAGE_HPP_
#define UCLOG_STORAGE_HPP_

#include <cstdarg>
#include <uclog/level.hpp>
#include <uclog/site.hpp>

namespace uclog
{

struct storage
{
    virtual void add_site(const site_t& site) = 0;
    virtual void log(const site_t& site, va_list args) = 0;
};

} // namespace uclog

#endif /* UCLOG_STORAGE_HPP_ */
