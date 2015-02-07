#include <uclog/logger.hpp>

namespace uclog
{

void logger::log(level_t level, const char* fmt, ...)
{
    va_list src_args;
    typedef std::vector<handler*>::iterator iterator;
    for (iterator handler = handlers_.begin(); handler != handlers_.end(); ++handler)
    {
        va_list args;
        va_copy(src_args, args);
        va_start(args, fmt);
        (*handler)->log(info_, level, fmt, args);
        va_end(args);
    }
}

} // namespace uclog
