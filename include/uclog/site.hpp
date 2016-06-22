#ifndef UCLOG_SITE_HPP_
#define UCLOG_SITE_HPP_

#include <uclog/array_view.hpp>
#include <uclog/intr_list.hpp>
#include <uclog/bprintf.hpp>

namespace uclog
{

class logger;

struct site
{
    site(logger& lgr, const char* in_fmt, array_view<arg_spec> in_args);

    const char* fmt;
    array_view<arg_spec> args;
    int id;
    site* next;
};

template <int N>
class site_data
{
public:
    site_data(logger& lgr, const char* fmt): node(lgr, fmt, args)
    { }

private:
    arg_spec args[N];
    site node;
};

} // namespace uclog

#endif /* UCLOG_SITE_HPP_ */
