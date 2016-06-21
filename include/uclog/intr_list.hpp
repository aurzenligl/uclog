#ifndef UCLOG_INTR_LIST_HPP_
#define UCLOG_INTR_LIST_HPP_

namespace uclog
{

template <typename T>
struct intr_list_node
{
    T data;
    T* next;
};

template <typename T>
class intr_list
{
public:
    void push_front(intr_list_node<T>& node)
    {
        node.next = first;
        first = &node;
    }

    intr_list_node<T>* begin()
    {
        return first;
    }

    intr_list_node<T>* end()
    {
        return 0;
    }

private:
    intr_list_node<T>* first;
};

} // namespace uclog

#endif /* UCLOG_INTR_LIST_HPP_ */
