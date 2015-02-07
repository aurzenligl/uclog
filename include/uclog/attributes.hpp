#ifndef UCLOG_ATTRIBUTES_HPP_
#define UCLOG_ATTRIBUTES_HPP_

namespace uclog
{

enum level_t
{
    level_not_set = 0,
    level_debug = 10,
    level_info = 20,
    level_warning = 30,
    level_error = 40,
    level_critical = 50
};

struct logger_info
{
    int id;
    const char* name;
};

} // namespace uclog

#endif /* UCLOG_ATTRIBUTES_HPP_ */
