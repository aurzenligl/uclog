#include <gtest/gtest.h>
#include <uclog/preprocessor.hpp>

TEST(preprocessor, line)
{
    struct X
    {
        int UCLOG_LINE(x);
        int UCLOG_LINE(x);
        int UCLOG_LINE(x);
    };

    static_assert(sizeof(X) == sizeof(int) * 3, "");
}

TEST(preprocessor, nargs)
{
    static_assert(UCLOG_NARG() == 0, "");
    static_assert(UCLOG_NARG(1) == 1, "");
    static_assert(UCLOG_NARG(1,1) == 2, "");
    static_assert(UCLOG_NARG(a,b,c) == 3, "");
    static_assert(UCLOG_NARG(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16) == 16, "");
}

TEST(preprocessor, when)
{
    static_assert(UCLOG_WHEN(0)(1) 42 == 42, "");
    static_assert(UCLOG_WHEN(1)(1) == 1, "");
    static_assert(UCLOG_WHEN(2)(1) == 1, "");
    static_assert(UCLOG_WHEN(a)(1) == 1, "");
    static_assert(UCLOG_WHEN(asdjasd)(1) == 1, "");
}
