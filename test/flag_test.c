#include "common.h"

static struct
{
    bool g_is_verbose;
} g_arg = {0};

static cmdp_flag_t g_flag_opt_verbose = CMDP_FLAG_NONE;
static cmdp_flag_t g_flag_cmd_run     = CMDP_FLAG_NONE;

#define __START()                                                                                                      \
    START_CMD();                                                                                                       \
    g_flag_opt_verbose = CMDP_FLAG_NONE;                                                                               \
    g_flag_cmd_run     = CMDP_FLAG_NONE;

static cmdp_action_t cb_process(cmdp_process_param_st *params)
{
    LOG_INFO("is_verbose: %d\n", g_arg.g_is_verbose);
    return CMDP_ACT_OK;
}
static cmdp_flag_t cb_flag_opt_verbose(cmdp_option_st *self)
{
    return g_flag_opt_verbose;
}
static cmdp_flag_t cb_flag_cmd_run(cmdp_command_st *self)
{
    return g_flag_cmd_run;
}

static cmdp_command_st g_command = {
    .options =
        (cmdp_option_st[]){
            {'v', "verbose", "Verbose Log", CMDP_TYPE_BOOL, &g_arg.g_is_verbose, .fn_flag = cb_flag_opt_verbose},
            {0},
        },
    .sub_commands =
        (cmdp_command_st *[]){
            &(cmdp_command_st){
                "run",
                "run command",
                .fn_process = cb_process,
                .fn_flag    = cb_flag_cmd_run,
            },
            NULL,
        },
};


UTEST(flag, normal)
{
    __START();
    RUN_CMD(&g_command, "--help");
    EXPECT_CMD(0,
               "  run                        run command\n"
               "  -v, --verbose              Verbose Log\n",
               "");
    END_CMD();
}


UTEST(flag, hide_option_verbose__help)
{
    __START();
    g_flag_opt_verbose = CMDP_FLAG_HIDE;
    RUN_CMD(&g_command, "--help");
    EXPECT_CMD(0, "  run                        run command\n", "");
    EXPECT_EQ(false, g_arg.g_is_verbose);
    END_CMD();
}

UTEST(flag, hide_option_verbose__v)
{
    __START();
    g_flag_opt_verbose = CMDP_FLAG_HIDE;
    RUN_CMD(&g_command, "-v");
    EXPECT_CMD(0, "", "");
    EXPECT_EQ(true, g_arg.g_is_verbose);
    END_CMD();
}

UTEST(flag, hide_option_verbose__verbose)
{
    __START();
    g_flag_opt_verbose = CMDP_FLAG_HIDE;
    RUN_CMD(&g_command, "--verbose");
    EXPECT_CMD(0, "", "");
    EXPECT_EQ(true, g_arg.g_is_verbose);
    END_CMD();
}


UTEST(flag, disable_option_verbose__help)
{
    __START();
    g_flag_opt_verbose = CMDP_FLAG_DISABLE;
    RUN_CMD(&g_command, "--help");
    EXPECT_CMD(0, "  run                        run command\n", "");
    EXPECT_EQ(false, g_arg.g_is_verbose);
    END_CMD();
}

UTEST(flag, disable_option_verbose__v)
{
    __START();
    g_flag_opt_verbose = CMDP_FLAG_DISABLE;
    RUN_CMD(&g_command, "-v");
    EXPECT_CMD(1, "", "Unknown option -v.\n");
    EXPECT_EQ(false, g_arg.g_is_verbose);
    END_CMD();
}

UTEST(flag, disable_option_verbose__verbose)
{
    __START();
    g_flag_opt_verbose = CMDP_FLAG_DISABLE;
    RUN_CMD(&g_command, "--verbose");
    EXPECT_CMD(1, "", "Unknown option --verbose.\n");
    EXPECT_EQ(false, g_arg.g_is_verbose);
    END_CMD();
}


UTEST(flag, hide_cmd_run__help)
{
    __START();
    g_flag_cmd_run = CMDP_FLAG_HIDE;
    RUN_CMD(&g_command, "--help");
    EXPECT_CMD(0, "  -v, --verbose              Verbose Log\n", "");
    END_CMD();
}

UTEST(flag, hide_cmd_run__run)
{
    __START();
    g_flag_cmd_run = CMDP_FLAG_HIDE;
    RUN_CMD(&g_command, "run");
    EXPECT_CMD(0, "", "");
    END_CMD();
}


UTEST(flag, disable_cmd_run__help)
{
    __START();
    g_flag_cmd_run = CMDP_FLAG_DISABLE;
    RUN_CMD(&g_command, "--help");
    EXPECT_CMD(0, "  -v, --verbose              Verbose Log\n", "");
    END_CMD();
}

UTEST(flag, disable_cmd_run__run)
{
    __START();
    g_flag_cmd_run = CMDP_FLAG_DISABLE;
    RUN_CMD(&g_command, "run");
    EXPECT_CMD(1, "", "Unknown command run.\n");
    END_CMD();
}
