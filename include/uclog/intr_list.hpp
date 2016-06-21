#ifndef UCLOG_INTR_LIST_HPP_
#define UCLOG_INTR_LIST_HPP_

namespace uclog
{

template <typename T>
struct intr_list_node
{
    T data;
    intr_list_node<T>* next;
};

template <typename T>
class intr_list
{
public:
    intr_list(): first_(0)
    { }

    void push_front(intr_list_node<T>& node)
    {
        node.next = first_;
        first_ = &node;
    }

    intr_list_node<T>* begin() { return first_; }
    const intr_list_node<T>* begin() const { return first_; }
    intr_list_node<T>* end() { return 0; }
    const intr_list_node<T>* end() const { return 0; }

private:
    intr_list_node<T>* first_;
};

} // namespace uclog

#endif /* UCLOG_INTR_LIST_HPP_ */
