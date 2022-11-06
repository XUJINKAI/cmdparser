#pragma once

#include "cmdparser.h"
#include "utest.h"

#pragma GCC diagnostic ignored "-Wformat-security"

extern bool g_log_switch;
#define LOG_INFO(...)                                                                                                  \
    if (g_log_switch)                                                                                                  \
    {                                                                                                                  \
        fprintf(stdout, __VA_ARGS__);                                                                                  \
    }

#define DIM(STRING)    "\x1b[2m" STRING "\x1b[0m"
#define countof(array) (sizeof(array) / sizeof(array[0]))

#define __COMBINE__(X, Y) X##Y
#define COMBINE(X, Y)     __COMBINE__(X, Y)


#define START_CMD()                                                                                                    \
    char *r_out      = NULL;                                                                                           \
    size_t _out_len  = 0;                                                                                              \
    FILE *out_stream = open_memstream(&r_out, &_out_len);                                                              \
    char *r_err      = NULL;                                                                                           \
    size_t _err_len  = 0;                                                                                              \
    FILE *err_stream = open_memstream(&r_err, &_err_len);                                                              \
    cmdp_ctx _ctx    = {0};                                                                                            \
    cmdp_set_default_context(&_ctx);                                                                                   \
    _ctx.out_stream = out_stream;                                                                                      \
    _ctx.err_stream = err_stream;                                                                                      \
    int r_code      = 0;

#define RUN_CMD(ref_cmdp, ...)                                                                                         \
    char *COMBINE(_arg_, __LINE__)[] = {__VA_ARGS__};                                                                  \
    r_code = cmdp_run(countof(COMBINE(_arg_, __LINE__)), COMBINE(_arg_, __LINE__), ref_cmdp, &_ctx);                   \
    fflush(out_stream);                                                                                                \
    fflush(err_stream);                                                                                                \
    LOG_INFO("%s %d\n", DIM("r_code:"), r_code);                                                                       \
    LOG_INFO("%s\n%s\n", DIM("r_out:"), r_out);                                                                        \
    LOG_INFO("%s\n%s\n", DIM("r_err:"), r_err);

#define EXPECT_CMD(code, out, err)                                                                                     \
    EXPECT_EQ(code, r_code);                                                                                           \
    EXPECT_STREQ(out, r_out);                                                                                          \
    EXPECT_STREQ(err, r_err);

#define END_CMD()                                                                                                      \
    fclose(out_stream);                                                                                                \
    fclose(err_stream);                                                                                                \
    (void)r_code;                                                                                                      \
    free(r_out);                                                                                                       \
    r_out = NULL;                                                                                                      \
    free(r_err);                                                                                                       \
    r_err = NULL;
