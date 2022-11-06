#include "common.h"

static struct
{
    bool opt_a;
    bool opt_b;
} g_arg = {0};

static cmdp_action_t g_process(cmdp_process_param_st *params);

static cmdp_command_st g_command = {
    .options =
        (cmdp_option_st[]){
            {'a', NULL, "option a", CMDP_TYPE_BOOL, &g_arg.opt_a},
            {'b', NULL, "option b", CMDP_TYPE_BOOL, &g_arg.opt_b},
            {0},
        },
    .fn_process = g_process,
};

static cmdp_action_t g_process(cmdp_process_param_st *params)
{
    if (params->argc == 0 && params->opts == 0)
    {
        return CMDP_ACT_SHOW_HELP;
    }
    fprintf(params->out_stream, "a:%d,b:%d", g_arg.opt_a, g_arg.opt_b);
    for (size_t i = 0; i < params->argc; i++)
    {
        fprintf(params->out_stream, ",%zu:%s", i, params->argv[i]);
    }

    return CMDP_ACT_OK;
}

UTEST(argument, simple)
{
    START_CMD();
    RUN_CMD(&g_command, "-a", "-b", "arg1", "arg2");
    EXPECT_CMD(0, "a:1,b:1,0:arg1,1:arg2", "");
    END_CMD();
}

UTEST(argument, end_op_options)
{
    START_CMD();
    RUN_CMD(&g_command, "-a", "--", "-b", "arg1", "arg2");
    EXPECT_CMD(0, "a:1,b:0,0:-b,1:arg1,2:arg2", "");
    END_CMD();
}
