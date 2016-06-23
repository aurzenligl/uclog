#ifndef UCLOG_SITE_HPP_
#define UCLOG_SITE_HPP_

#include <uclog/array_view.hpp>
#include <uclog/bprintf.hpp>
#include <uclog/level.hpp>

namespace uclog
{

class logger;

struct site_t
{
    site_t(){}
    site_t(logger& lgr, level_t lvl, const char* in_fmt, const array_view<arg_type>& in_args);

    int id;
    level_t level;
    const char* fmt;
    array_view<arg_type> args;
};

template <int N>
struct site_data
{
    site_data(logger& lgr, level_t lvl, const char* fmt): site(lgr, lvl, fmt, array_view<arg_type>(args, N))
    { }

    arg_type args[N];
    site_t site;
};

} // namespace uclog

#endif /* UCLOG_SITE_HPP_ */
