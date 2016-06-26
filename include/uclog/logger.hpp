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
    logger(): level_(level_debug), next_site_(0)
    { }

    explicit logger(level_t level): level_(level), next_site_(0)
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

    int enumerate_site();

    void log(const site_t* site, ...);

private:
    typedef std::vector<handler*> handlers_t;

    handlers_t handlers_;
    level_t level_;
    int next_site_;
};

} // namespace uclog

#endif /* UCLOG_LOGGER_HPP_ */
