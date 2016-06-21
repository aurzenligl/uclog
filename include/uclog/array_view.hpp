#ifndef UCLOG_ARRAY_VIEW_HPP_
#define UCLOG_ARRAY_VIEW_HPP_

#include <cstddef>

namespace uclog
{

template <typename T>
class array_view
{
public:
    array_view(T* data, size_t size): begin_(data), end_(data + size)
    { }

    template <int N>
    array_view(T (&data) [N]): begin_(data), end_(data + N)
    { }

    T* begin() { return begin_; }
    const T* begin() const { return begin_; }
    T* end() { return end_; }
    const T* end() const { return end_; }

private:
    T* begin_;
    T* end_;
};

} // namespace uclog

#endif /* UCLOG_ARRAY_VIEW_HPP_ */
