#include <uclog/logger.hpp>
#include <uclog/handler.hpp>

namespace uclog
{

void logger::add_handler(handler& handler)
{
    handlers_.push_back(&handler);
}

void logger::add_site(const site_t& site)
{
    for (handlers_t::iterator h = handlers_.begin(); h != handlers_.end(); ++h)
    {
        (*h)->add_site(site);
    }
}

int logger::enumerate_site()
{
    // TODO: concurrency: this needs to be guarded
    return next_site_++;
}

void logger::log(const site_t* site, ...)
{
    if (site->level < level_)
    {
        return;
    }

    va_list src_args;
    va_start(src_args, site);
    for (handlers_t::iterator h = handlers_.begin(); h != handlers_.end(); ++h)
    {
        va_list args;
        va_copy(args, src_args);
        (*h)->log(*site, args);
        va_end(args);
    }
    va_end(src_args);
}

} // namespace uclog
