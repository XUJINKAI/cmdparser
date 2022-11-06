#include "common.h"

static cmdp_action_t __process(cmdp_process_param_st *params);
static struct
{
    char *letter;
} args;

static cmdp_command_st g_command = {
    .sub_commands =
        (cmdp_command_st *[]){
            &(cmdp_command_st){
                "letter",
                "Show letter",
                .options =
                    (cmdp_option_st[]){
                        {'c', "char", "Show char", CMDP_TYPE_STRING_PTR, &args.letter},
                        {0},
                    },
                .variant_cmds = (char *[]){"a", "b", "c", NULL},
                .fn_process   = __process,
            },
            NULL,
        },
};
static cmdp_action_t __process(cmdp_process_param_st *params)
{
    if (strcmp("letter", params->call_name) == 0)
    {
        fprintf(params->out_stream, "letter: %s\n", args.letter);
    }
    else
    {
        fprintf(params->out_stream, "variant: %s\n", params->call_name);
    }
    return CMDP_ACT_OK;
}

UTEST(variant_cmds, letter_z)
{
    START_CMD();
    RUN_CMD(&g_command, "letter", "-c", "z");
    EXPECT_CMD(0, "letter: z\n", "");
    END_CMD();
}

UTEST(variant_cmds, a)
{
    START_CMD();
    RUN_CMD(&g_command, "a");
    EXPECT_CMD(0, "variant: a\n", "");
    END_CMD();
}

UTEST(variant_cmds, z)
{
    START_CMD();
    RUN_CMD(&g_command, "z");
    EXPECT_CMD(1, "", "Unknown command z.\n");
    END_CMD();
}
