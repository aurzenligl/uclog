#ifndef TEST_FAKE_STORAGE_HPP_
#define TEST_FAKE_STORAGE_HPP_

#include <string>
#include <uclog/storage.hpp>

struct fake_storage : public uclog::storage
{
    virtual void store(const uclog::logger_info& logger, uclog::level_t level, const char* fmt, va_list args)
    {
        char buf[1024];
        int size = vsnprintf(buf, 1024, fmt, args);
        data.insert(data.end(), buf, buf + size);
    }

    std::string read_and_clear()
    {
        std::string out;
        out.swap(data);
        return out;
    }

    std::string data;
};

#endif /* TEST_FAKE_STORAGE_HPP_ */
