#include "common.h"

static struct
{
    bool g_is_verbose;
    bool g_is_debug;
    char *work_dir;
} arg_global = {0};

static struct
{
    bool opt_a;
    bool opt_b;
    char *opt_input;
} arg_sub = {0};

static cmdp_action_t global_process(cmdp_process_param_st *params);
static cmdp_action_t sub_process(cmdp_process_param_st *params);

static cmdp_command_st g_command = {
    .options =
        (cmdp_option_st[]){
            {0, "verbose", "Verbose Log", CMDP_TYPE_BOOL, &arg_global.g_is_verbose},
            {0, "debug", "Debug Log", CMDP_TYPE_BOOL, &arg_global.g_is_debug},
            {0, "work-dir", "Work Directory", CMDP_TYPE_STRING_PTR, &arg_global.work_dir},
            {0},
        },
    .sub_commands =
        (cmdp_command_st *[]){
            &(cmdp_command_st){
                "run",
                .options =
                    (cmdp_option_st[]){
                        {'a', NULL, "A option", CMDP_TYPE_BOOL, &arg_sub.opt_a},
                        {'b', NULL, "B option", CMDP_TYPE_BOOL, &arg_sub.opt_b},
                        {'i', "input", "Input option", CMDP_TYPE_STRING_PTR, &arg_sub.opt_input},
                        {0},
                    },
                .fn_process = sub_process,
            },
            NULL,
        },
    .fn_process = global_process,
};

static cmdp_action_t global_process(cmdp_process_param_st *params)
{
    fprintf(params->out_stream, "Global: verbose=%d, debug=%d, work_dir=%s\n", arg_global.g_is_verbose,
            arg_global.g_is_debug, arg_global.work_dir ? arg_global.work_dir : "");
    return CMDP_ACT_CONTINUE;
}
static cmdp_action_t sub_process(cmdp_process_param_st *params)
{
    fprintf(params->out_stream, "Sub: a=%d, b=%d, input=%s\n", arg_sub.opt_a, arg_sub.opt_b,
            arg_sub.opt_input ? arg_sub.opt_input : "");
    return CMDP_ACT_OK;
}


UTEST(global_option, run_normal_full)
{
    START_CMD();
    RUN_CMD(&g_command, "--verbose", "--debug", "--work-dir", "/root", "run", "-a", "-b", "--input", "xjk");
    EXPECT_STREQ("Global: verbose=1, debug=1, work_dir=/root\n"
                 "Sub: a=1, b=1, input=xjk\n",
                 r_out);
    EXPECT_STREQ("", r_err);
    EXPECT_EQ(0, r_code);
    END_CMD();
}

UTEST(global_option, only_global_options)
{
    START_CMD();
    RUN_CMD(&g_command, "--verbose", "--debug", "--work-dir", "/root");
    EXPECT_STREQ("Global: verbose=1, debug=1, work_dir=/root\n", r_out);
    EXPECT_STREQ("", r_err);
    EXPECT_EQ(0, r_code);
    END_CMD();
}

UTEST(global_option, run_global_option_behind)
{
    START_CMD();
    RUN_CMD(&g_command, "run", "-b", "--verbose", "--work-dir", "/root", "--input", "xjk");
    EXPECT_STREQ("Global: verbose=1, debug=0, work_dir=/root\n"
                 "Sub: a=0, b=1, input=xjk\n",
                 r_out);
    EXPECT_STREQ("", r_err);
    EXPECT_EQ(0, r_code);
    END_CMD();
}
