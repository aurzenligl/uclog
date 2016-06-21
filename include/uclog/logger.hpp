#ifndef UCLOG_LOGGER_HPP_
#define UCLOG_LOGGER_HPP_

#include <cstdarg>
#include <vector>
#include <uclog/handler.hpp>
#include <uclog/site.hpp>
#include <uclog/level.hpp>

namespace uclog
{

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

    void add_handler(handler& handler)
    {
        handlers_.push_back(&handler);
    }

    void log(level_t level, const char* fmt, ...);

private:
    std::vector<handler*> handlers_;
    level_t level_;
};

} // namespace uclog

#endif /* UCLOG_LOGGER_HPP_ */
