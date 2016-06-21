#include <uclog/logger.hpp>

namespace uclog
{

void logger::log(level_t level, const char* fmt, ...)
{
    typedef std::vector<handler*>::iterator iterator;

    if (level < level_)
    {
        return;
    }

    va_list src_args;
    va_start(src_args, fmt);
    for (iterator handler = handlers_.begin(); handler != handlers_.end(); ++handler)
    {
        va_list args;
        va_copy(args, src_args);
        (*handler)->log(level, fmt, args);
        va_end(args);
    }
    va_end(src_args);
}

} // namespace uclog
