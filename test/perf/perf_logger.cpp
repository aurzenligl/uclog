#include <cstdio>
#include <vector>
#include <string.h>
#include <benchmark/benchmark.h>
#include <uclog/bprintf.hpp>
#include <uclog/logger.hpp>
#include <uclog/basic_log.hpp>
#include <uclog/handler.hpp>
#include <uclog/storage.hpp>

using namespace uclog;

enum { entry_size = 255 };

struct m_trunc
{
    m_trunc(): data(entry_size)
    { }

    uint8_t* reserve(size_t size)
    {
        return data.data();
    }

    void commit(size_t)
    { }

    std::vector<uint8_t> data;
};

template <int Size>
struct m_wrap
{
    m_wrap(): data(Size), offset(0)
    { }

    uint8_t* reserve(size_t size)
    {
        if (left() < size)
        {
            offset = 0;
        }
        return data.data() + offset;
    }

    void commit(size_t written)
    {
        data[offset] = written;
        offset += written + 1;
    }

    size_t left()
    {
        return data.size() - offset;
    }

    std::vector<uint8_t> data;
    size_t offset;
};

struct a_printf
{
    int operator()(uint8_t* buf, size_t size, const site_t& site, va_list args)
    {
        return vsnprintf(reinterpret_cast<char*>(buf), size, site.fmt, args);
    }
};

struct a_bprintf_slow
{
    int operator()(uint8_t* buf, size_t size, const site_t& site, va_list args)
    {
        uint32_t hash = fnv_hash(site.fmt);
        memcpy(buf, &hash, sizeof(uint32_t));
        buf += sizeof(uint32_t);
        size -= sizeof(uint32_t);

        return vsnbprintf(buf, size, site.fmt, args) + sizeof(uint32_t);
    }
};

struct a_bprintf_fast
{
    int operator()(uint8_t* buf, size_t size, const site_t& site, va_list args)
    {
        memcpy(buf, &site.hash, sizeof(uint32_t));
        buf += sizeof(uint32_t);
        size -= sizeof(uint32_t);

        return vargs_encode(buf, size, site.args.begin(), site.args.size(), args) + sizeof(uint32_t);
    }
};

template <typename Membuf, typename Algo>
struct storage_t : public uclog::storage
{
    virtual void add_site(const site_t& site)
    { }

    virtual void log(const site_t& site, va_list args)
    {
        uint8_t* buf = membuf.reserve(entry_size);
        int written = Algo()(buf, entry_size, site, args);
        membuf.commit(written);
    }

    Membuf membuf;
};

template <typename Membuf, typename Algo>
void bm_log(benchmark::State& state)
{
    storage_t<Membuf, Algo> s;
    handler h(s);
    logger lgr;
    lgr.add_handler(h);

    while (state.KeepRunning())
    {
        UCLOG_BASIC_LOG(lgr, info, "This is an exemplary log: %d %f %hhd %s", 1, 2., 3, "abc");
    }
}

BENCHMARK_TEMPLATE2(bm_log, m_trunc, a_printf);
BENCHMARK_TEMPLATE2(bm_log, m_wrap<1024>, a_printf);
BENCHMARK_TEMPLATE2(bm_log, m_wrap<64*1024*1024>, a_printf);

BENCHMARK_TEMPLATE2(bm_log, m_trunc, a_bprintf_slow);
BENCHMARK_TEMPLATE2(bm_log, m_wrap<1024>, a_bprintf_slow);
BENCHMARK_TEMPLATE2(bm_log, m_wrap<64*1024*1024>, a_bprintf_slow);

BENCHMARK_TEMPLATE2(bm_log, m_trunc, a_bprintf_fast);
BENCHMARK_TEMPLATE2(bm_log, m_wrap<1024>, a_bprintf_fast);
BENCHMARK_TEMPLATE2(bm_log, m_wrap<64*1024*1024>, a_bprintf_fast);

BENCHMARK_MAIN();
