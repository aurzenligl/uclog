#ifndef UCLOG_LOGGER_HPP_
#define UCLOG_LOGGER_HPP_

#include <cstdarg>
#include <vector>
#include <uclog/site.hpp>
#include <uclog/level.hpp>

namespace uclog
{

class handler;

class logger
{
public:
    logger(): level_(level_debug)
    { }

    explicit logger(level_t level): level_(level)
    { }

    level_t level() const
    {
        return level_;
    }

    void set_level(level_t level)
    {
        level_ = level;
    }

    void add_handler(handler& handler);

    void add_site(const site_t& site);

    void log(const site_t* site, ...);

private:
    typedef std::vector<handler*> handlers_t;

    handlers_t handlers_;
    level_t level_;
};

} // namespace uclog

#endif /* UCLOG_LOGGER_HPP_ */
