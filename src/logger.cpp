#include <uclog/logger.hpp>

namespace uclog
{

static level_t get_effective_level(logger* lgr)
{
    for (; lgr; lgr = lgr->parent())
    {
        if (lgr->level() != level_not_set)
        {
            return lgr->level();
        }
    }
    return level_not_set;
}

void logger::log(level_t level, const char* fmt, ...)
{
    level_t effective_level = get_effective_level(this);
    if (effective_level == level_not_set || level < effective_level)
    {
        return;
    }

    va_list src_args;
    va_start(src_args, fmt);
    for (logger* lgr = this; lgr; lgr = propagate_ ? lgr->parent_ : 0)
    {
        typedef std::vector<handler*>::iterator iterator;
        for (iterator handler = lgr->handlers_.begin(); handler != lgr->handlers_.end(); ++handler)
        {
            va_list args;
            va_copy(args, src_args);
            (*handler)->log(info_, level, fmt, args);
            va_end(args);
        }
    }
    va_end(src_args);
}

} // namespace uclog
