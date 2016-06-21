#ifndef UCLOG_HANDLER_HPP_
#define UCLOG_HANDLER_HPP_

#include <cstdarg>
#include <uclog/storage.hpp>
#include <uclog/level.hpp>

namespace uclog
{

class handler
{
public:
    handler(): storage_(), level_(level_debug)
    { }

    explicit handler(storage& storage): storage_(&storage), level_(level_debug)
    { }

    handler(storage& storage, level_t level): storage_(&storage), level_(level)
    { }

    level_t level() const
    {
        return level_;
    }

    void set_level(level_t level)
    {
        level_ = level;
    }

    void log(level_t level, const char* fmt, va_list args)
    {
        if (level >= level_)
        {
            storage_->store(level, fmt, args);
        }
    }

private:
    storage* storage_;
    level_t level_;
};

} // namespace uclog

#endif /* UCLOG_HANDLER_HPP_ */
