#ifndef UCLOG_LOGGER_HPP_
#define UCLOG_LOGGER_HPP_

#include <cstdarg>
#include <vector>
#include <uclog/attributes.hpp>
#include <uclog/handler.hpp>
#include <uclog/site.hpp>

namespace uclog
{

class logger
{
public:
    logger(): info_(), level_(level_debug)
    { }

    explicit logger(const logger_info& info)
        : info_(info), level_(level_debug)
    { }

    logger(const logger_info& info, level_t level)
        : info_(info), level_(level)
    { }

    level_t level() const
    {
        return level_;
    }

    const char* name() const
    {
        return info_.name;
    }

    int id() const
    {
        return info_.id;
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
    logger_info info_;
    level_t level_;
};

} // namespace uclog

#endif /* UCLOG_LOGGER_HPP_ */
