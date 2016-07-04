#include <uclog/site.hpp>
#include <uclog/logger.hpp>
#include <algorithm>

namespace uclog
{

namespace
{

struct gen_args
{
    gen_args(const char* fmt): fmt_(fmt)
    { }

    arg_type operator()()
    {
        arg_spec arg;
        fmt_ += arg_decode(fmt_, &arg);
        return arg.type;
    }

    const char* fmt_;
};

} // unnamed namespace

site_t::site_t(logger& lgr, level_t lvl, const char* in_fmt, const array_view<arg_type>& in_args)
    : hash(fnv_hash(in_fmt)), level(lvl), fmt(in_fmt), args(in_args)
{
    std::generate(args.begin(), args.end(), gen_args(in_fmt));
    lgr.add_site(*this);
}

} // namespace uclog
