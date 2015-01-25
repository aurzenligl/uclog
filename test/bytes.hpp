#ifndef TEST_BYTES_HPP_
#define TEST_BYTES_HPP_

#include <vector>
#include <stdint.h>

template <size_t N>
std::vector<uint8_t> bytes(const char (&x) [N])
{
    return std::vector<uint8_t>(x, x + N - 1);
}

inline std::vector<uint8_t> bytes(const void* x, size_t size)
{
    return std::vector<uint8_t>(static_cast<const uint8_t*>(x), static_cast<const uint8_t*>(x) + size);
}

#endif /* TEST_BYTES_HPP_ */
