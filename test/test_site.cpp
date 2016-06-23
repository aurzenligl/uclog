#include <gtest/gtest.h>
#include <uclog/logger.hpp>
#include <uclog/site.hpp>

using namespace testing;
using namespace uclog;

template <typename... Arg>
static std::vector<arg_type> to_args(Arg... as)
{
    return std::vector<arg_type>{as...};
}

static std::vector<arg_type> to_args(const array_view<arg_type>& as)
{
    return std::vector<arg_type>(as.begin(), as.end());
}

TEST(site, saves_fmt_and_level)
{
    const char* fmt = "abc";
    logger lgr;
    site_data<0> site(lgr, level_warning, fmt);

    EXPECT_EQ(fmt, site.site.fmt);
    EXPECT_EQ(level_warning, site.site.level);
}

TEST(site, saves_arg_specs_0)
{
    logger lgr;
    site_data<0> site(lgr, level_info, "");
    EXPECT_EQ(to_args(), to_args(site.site.args));
}

TEST(site, saves_arg_specs_1)
{
    logger lgr;
    site_data<1> site(lgr, level_info, "%d");
    EXPECT_EQ(to_args(arg_type_int), to_args(site.site.args));
}

TEST(site, saves_arg_specs_N)
{
    logger lgr;
    site_data<3> site(lgr, level_info, "%f %s %hhd");
    EXPECT_EQ(to_args(arg_type_float, arg_type_string, arg_type_char), to_args(site.site.args));
}

TEST(site, saves_arg_specs_too_few)
{
    logger lgr;
    site_data<2> site(lgr, level_info, "%d");
    EXPECT_EQ(to_args(arg_type_int, arg_type_none), to_args(site.site.args));
}

TEST(site, saves_arg_specs_too_many)
{
    logger lgr;
    site_data<1> site(lgr, level_info, "%d %d");
    EXPECT_EQ(to_args(arg_type_int), to_args(site.site.args));
}

TEST(site, saves_id)
{
    logger lgr;
    site_data<0> site1(lgr, level_info, "");
    site_data<0> site2(lgr, level_info, "");
    site_data<0> site3(lgr, level_info, "");

    EXPECT_EQ(0, site1.site.id);
    EXPECT_EQ(1, site2.site.id);
    EXPECT_EQ(2, site3.site.id);
}

TEST(site, makes_linked_list)
{
    logger lgr;
    site_data<0> site1(lgr, level_info, "");
    site_data<0> site2(lgr, level_info, "");
    site_data<0> site3(lgr, level_info, "");

    EXPECT_EQ(0, site1.site.id);
    EXPECT_EQ(1, site2.site.id);
    EXPECT_EQ(2, site3.site.id);
}
