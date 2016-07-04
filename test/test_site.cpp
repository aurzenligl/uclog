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

TEST(site, saves_hash)
{
    logger lgr;

    EXPECT_EQ(0x811c9dc5, site_data<0>(lgr, level_info, "").site.hash);
    EXPECT_EQ(0x340ca71c, site_data<0>(lgr, level_info, "1").site.hash);
    EXPECT_EQ(0x1deb2d6a, site_data<0>(lgr, level_info, "12").site.hash);
}
