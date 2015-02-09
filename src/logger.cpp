#include <uclog/logger.hpp>

namespace uclog
{

void logger::log(level_t level, const char* fmt, ...)
{
    if (level < level_)
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
