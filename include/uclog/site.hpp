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
    int id;
    const char* fmt;
    array_view<arg_spec> args;
};

template <int N>
struct site_data
{
    site_data(logger& lgr, const char* fmt, ...);

    arg_spec args[N];
    intr_list_node<site> node;
};

} // namespace uclog

#endif /* UCLOG_SITE_HPP_ */
