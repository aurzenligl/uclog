#ifndef UCLOG_FNV_HASH_HPP_
#define UCLOG_FNV_HASH_HPP_

#include <stdint.h>

namespace uclog
{

inline uint32_t fnv_hash(const char* str)
{
    const uint32_t prime = 0x01000193; //   16777619
    const uint32_t seed  = 0x811C9DC5; // 2166136261

    uint32_t hash = seed;
    while (*str)
    {
        hash = (*str++ ^ hash) * prime;
    }
    return hash;
}

} // namespace uclog

#endif /* UCLOG_FNV_HASH_HPP_ */
