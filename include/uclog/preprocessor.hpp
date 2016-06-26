#ifndef UCLOG_PREPROCESSOR_HPP_
#define UCLOG_PREPROCESSOR_HPP_

#define UCLOG_LINE(X) UCLOG_LINE_HELPER1(X, __LINE__)
    #define UCLOG_LINE_HELPER1(X, Y) UCLOG_LINE_HELPER2(X, Y)
    #define UCLOG_LINE_HELPER2(X, Y) X##Y

#define UCLOG_NARG(...)                                                        \
    UCLOG_NARG_HELPER1(                                                        \
        UCLOG_ARG_N(__VA_ARGS__, UCLOG_COMMA_SEQ()),                           \
        UCLOG_ARG_N(UCLOG_COMMA __VA_ARGS__ (), UCLOG_COMMA_SEQ()),            \
        UCLOG_ARG_N(__VA_ARGS__, UCLOG_R_SEQ()))

    #define UCLOG_NARG_HELPER1(a, b, N)    UCLOG_NARG_HELPER2(a, b, N)
    #define UCLOG_NARG_HELPER2(a, b, N)    UCLOG_NARG_HELPER3_ ## a ## b(N)
    #define UCLOG_NARG_HELPER3_01(N)       0
    #define UCLOG_NARG_HELPER3_00(N)       1
    #define UCLOG_NARG_HELPER3_11(N)       N

    #define UCLOG_ARG_N(...) UCLOG_ARG_N_HELPER1(__VA_ARGS__)
    #define UCLOG_ARG_N_HELPER1(                                                   \
        _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10,                           \
        _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,                          \
        _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,                          \
        _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,                          \
        _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,                          \
        _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,                          \
        _61, _62, _63, N, ...) N

    #define UCLOG_COMMA(...) ,

    #define UCLOG_R_SEQ()                                                          \
             63, 62, 61, 60,                                                       \
             59, 58, 57, 56, 55, 54, 53, 52, 51, 50,                               \
             49, 48, 47, 46, 45, 44, 43, 42, 41, 40,                               \
             39, 38, 37, 36, 35, 34, 33, 32, 31, 30,                               \
             29, 28, 27, 26, 25, 24, 23, 22, 21, 20,                               \
             19, 18, 17, 16, 15, 14, 13, 12, 11, 10,                               \
              9,  8,  7,  6,  5,  4,  3,  2,  1,  0

    #define UCLOG_COMMA_SEQ()                                                      \
              1,  1,  1,  1,                                                       \
              1,  1,  1,  1,  1,  1,  1,  1,  1,  1,                               \
              1,  1,  1,  1,  1,  1,  1,  1,  1,  1,                               \
              1,  1,  1,  1,  1,  1,  1,  1,  1,  1,                               \
              1,  1,  1,  1,  1,  1,  1,  1,  1,  1,                               \
              1,  1,  1,  1,  1,  1,  1,  1,  1,  1,                               \
              1,  1,  1,  1,  1,  1,  1,  1,  0,  0

#define UCLOG_WHEN(c) UCLOG_IF(c)(UCLOG_EXPAND, UCLOG_EAT)
    #define UCLOG_EXPAND(...) __VA_ARGS__
    #define UCLOG_EAT(...)

    #define UCLOG_IF(c) UCLOG_IIF(UCLOG_BOOL(c))
    #define UCLOG_IIF(c) UCLOG_PRIMITIVE_CAT(UCLOG_IIF_, c)
    #define UCLOG_IIF_0(t, ...) __VA_ARGS__
    #define UCLOG_IIF_1(t, ...) t

    #define UCLOG_BOOL(x) UCLOG_COMPL(UCLOG_NOT(x))
    #define UCLOG_COMPL(b) UCLOG_PRIMITIVE_CAT(UCLOG_COMPL_, b)
    #define UCLOG_COMPL_0 1
    #define UCLOG_COMPL_1 0
    #define UCLOG_NOT(x) UCLOG_CHECK(UCLOG_PRIMITIVE_CAT(UCLOG_NOT_, x))
    #define UCLOG_NOT_0 ~, 1,
    #define UCLOG_CHECK(...) UCLOG_CHECK_N(__VA_ARGS__, 0,)
    #define UCLOG_CHECK_N(x, n, ...) n

    #define UCLOG_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#endif /* UCLOG_PREPROCESSOR_HPP_ */
