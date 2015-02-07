#include <uclog/logger.hpp>

namespace uclog
{

void logger::log(level_t level, const char* fmt, ...)
{
    va_list src_args;

    for (logger* lgr = this; lgr; lgr = propagate_ ? lgr->parent_ : 0)
    {
        typedef std::vector<handler*>::iterator iterator;
        for (iterator handler = lgr->handlers_.begin(); handler != lgr->handlers_.end(); ++handler)
        {
            va_list args;
            va_copy(src_args, args);
            va_start(args, fmt);
            (*handler)->log(info_, level, fmt, args);
            va_end(args);
        }
    }
}

} // namespace uclog
