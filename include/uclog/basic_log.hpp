#ifndef UCLOG_BASIC_LOG_HPP_
#define UCLOG_BASIC_LOG_HPP_

#include <uclog/preprocessor.hpp>
#include <uclog/logger.hpp>
#include <uclog/site.hpp>

#define UCLOG_BASIC_LOG(LOGGER, LEVEL, FMT, ...)                               \
    if (uclog::logger* UCLOG_LINE(lgr) = LOGGER)                               \
    {                                                                          \
        static const uclog::site_data<UCLOG_NARG(__VA_ARGS__)>                 \
            UCLOG_LINE(site)(*UCLOG_LINE(lgr), uclog::level_##LEVEL, FMT);     \
        UCLOG_LINE(lgr)->log(&UCLOG_LINE(site).site                            \
            UCLOG_WHEN(UCLOG_NARG(__VA_ARGS__))(,) __VA_ARGS__);               \
    }

#endif /* UCLOG_BASIC_LOG_HPP_ */
