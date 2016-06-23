#ifndef TEST_FAKE_STORAGE_HPP_
#define TEST_FAKE_STORAGE_HPP_

#include <vector>
#include <string>
#include <uclog/storage.hpp>

struct fake_storage : public uclog::storage
{
    void add_site(const uclog::site_t& site)
    {
        sites.push_back(&site);
    }

    void log(const uclog::site_t& site, va_list args)
    {
        char buf[1024];
        int size = vsnprintf(buf, 1024, site.fmt, args);
        data.insert(data.end(), buf, buf + size);
    }

    std::string read_and_clear()
    {
        std::string out;
        out.swap(data);
        return out;
    }

    std::string data;
    std::vector<const uclog::site_t*> sites;
};

#endif /* TEST_FAKE_STORAGE_HPP_ */
