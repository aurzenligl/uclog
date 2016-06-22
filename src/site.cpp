#include <uclog/site.hpp>
#include <uclog/logger.hpp>

namespace uclog
{

site::site(logger& lgr, const char* in_fmt, array_view<arg_spec> in_args)
    : fmt(in_fmt), args(in_args)
{
    // fmt to args
    lgr.add_site(*this);
}

} // namespace uclog
