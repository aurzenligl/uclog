#ifndef UCLOG_LOGGER_HPP_
#define UCLOG_LOGGER_HPP_

#include <cstdarg>
#include <vector>
#include <uclog/attributes.hpp>
#include <uclog/handler.hpp>

namespace uclog
{

class logger
{
public:
    logger(): info_(), parent_(), level_(level_not_set), propagate_(true)
    { }

    explicit logger(const logger_info& info)
        : info_(info), parent_(), level_(level_not_set), propagate_(true)
    { }

    logger(const logger_info& info, logger& parent)
        : info_(info), parent_(&parent), level_(level_not_set), propagate_(true)
    { }

    logger(const logger_info& info, logger& parent, level_t level)
        : info_(info), parent_(&parent), level_(level), propagate_(true)
    { }

    logger(const logger_info& info, level_t level)
        : info_(info), parent_(), level_(level), propagate_(true)
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

    bool propagate() const
    {
        return propagate_;
    }

    void set_level(level_t level)
    {
        level_ = level;
    }

    void set_propagate(bool value)
    {
        propagate_ = value;
    }

    void add_handler(handler& handler)
    {
        handlers_.push_back(&handler);
    }

    void log(level_t level, const char* fmt, ...);

private:
    std::vector<handler*> handlers_;
    logger_info info_;
    logger* parent_;
    level_t level_;
    bool propagate_;
};

} // namespace uclog

#endif /* UCLOG_LOGGER_HPP_ */
