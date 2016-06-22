#ifndef UCLOG_INTR_LIST_HPP_
#define UCLOG_INTR_LIST_HPP_

#include <cstddef>

namespace uclog
{

/*
 * List elements must have id and next members.
 */
template <typename T>
class intr_list
{
public:
    intr_list(): first_(0)
    { }

    void push_front(T& node)
    {
        node.id = first_ ? (first_->id + 1) : 0;
        node.next = first_;
        first_ = &node;
    }

    T* begin() { return first_; }
    const T* begin() const { return first_; }
    T* end() { return 0; }
    const T* end() const { return 0; }

private:
    T* first_;
};

} // namespace uclog

#endif /* UCLOG_INTR_LIST_HPP_ */
