#include "common.h"

static struct
{
    bool g_is_verbose;
    bool is_all;
} g_arg = {0};

static void g_before(cmdp_before_param_st *params);
static cmdp_action_t g_process(cmdp_process_param_st *params);

static cmdp_command_st g_command = {
    .options =
        (cmdp_option_st[]){
            {'v', "verbose", "Verbose Log", CMDP_TYPE_BOOL, &g_arg.g_is_verbose},
            {0},
        },
    .sub_commands =
        (cmdp_command_st[]){
            {
                "run",
                .options =
                    (cmdp_option_st[]){
                        {'a', "all", "Is All", CMDP_TYPE_BOOL, &g_arg.is_all},
                        {0},
                    },
                .fn_before  = g_before,
                .fn_process = g_process,
            },
            {0},
        },
};

static void g_before(cmdp_before_param_st *params)
{
    memset(&g_arg, 0, sizeof(g_arg));
}
static cmdp_action_t g_process(cmdp_process_param_st *params)
{
    LOG_INFO("is_verbose: %d\n", g_arg.g_is_verbose);
    LOG_INFO("is_all: %d\n", g_arg.is_all);
    return CMDP_ACT_OVER;
}


UTEST(future, global_options)
{
    START_CMD();
    RUN_CMD(&g_command, "run", "--all", "--verbose");
    EXPECT_STREQ("", r_out);
    /* 
    expect no error, but: Unknown option --verbose.
     */
    // EXPECT_STREQ("", r_err);
    // EXPECT_EQ(CMDP_OK, r_code);
    END_CMD();
}
