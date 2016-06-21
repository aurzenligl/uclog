#ifndef UCLOG_SITE_HPP_
#define UCLOG_SITE_HPP_

#include <uclog/array_view.hpp>
#include <uclog/intr_list.hpp>
#include <uclog/bprintf.hpp>

namespace uclog
{

class logger;

struct site_info
{
    int id;
    const char* fmt;
    array_view<arg_spec> args;
};

void init_site(array_view<arg_spec> args,
               intr_list_node<site_info>& info,
               logger& lgr,
               const char* fmt,
               va_list vaargs);

template <int N>
class site
{
public:
    site(logger& lgr, const char* fmt, ...)
    {
        va_list vaargs;
        va_start(vaargs, fmt);
        init_site(args, node, lgr, fmt, vaargs);
        va_end(vaargs);
    }

private:
    arg_spec args[N];
    intr_list_node<site_info> node;
};

} // namespace uclog

#endif /* UCLOG_SITE_HPP_ */
