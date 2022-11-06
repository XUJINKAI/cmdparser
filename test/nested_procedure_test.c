#include "common.h"

static char g_procedure[1024];
#define __PROCEDURE_CAT(...) sprintf(g_procedure + strlen(g_procedure), __VA_ARGS__);

static cmdp_action_t g_action[10]; // default: CMDP_ACT_CONTINUE
#define __ACTION_SET(level, act) g_action[level] = act;

#define __START()                                                                                                      \
    START_CMD();                                                                                                       \
    memset(g_procedure, 0, sizeof(g_procedure));                                                                       \
    memset(g_action, 0, sizeof(g_action));

#define __END()                                                                                                        \
    LOG_INFO("%s %s\n", DIM("procedure:"), g_procedure);                                                               \
    END_CMD();

#define __EXPECT(code, out, err, procedure)                                                                            \
    EXPECT_CMD(code, out, err);                                                                                        \
    EXPECT_STREQ(procedure, g_procedure);

static cmdp_action_t cb_process(cmdp_process_param_st *params)
{
    __PROCEDURE_CAT("L%d ", params->sub_level);
    if (params->next == NULL && params->argc == 0 && params->opts == 0)
    {
        return CMDP_ACT_SHOW_HELP;
    }
    return g_action[params->sub_level];
}

static cmdp_command_st g_command = {
    .name       = "L0",
    .desc       = "L0 desc",
    .doc        = "L0 doc\n",
    .fn_process = cb_process,
    .sub_commands =
        (cmdp_command_st *[]){
            &(cmdp_command_st){
                .name       = "L1",
                .desc       = "L1 desc",
                .doc        = "L1 doc\n",
                .fn_process = cb_process,
                .sub_commands =
                    (cmdp_command_st *[]){
                        &(cmdp_command_st){
                            .name       = "L2",
                            .desc       = "L2 desc",
                            .doc        = "L2 doc\n",
                            .fn_process = cb_process,
                        },
                        NULL,
                    },
            },
            NULL,
        },
};
static const char *g_L0_help = "L0 doc\n"
                               "  L1                         L1 desc\n";
static const char *g_L1_help = "L1 doc\n"
                               "  L2                         L2 desc\n";
static const char *g_L2_help = "L2 doc\n";
static const char *err_other = "Unknown command other.\n";

UTEST(nested_procedure, L0)
{
    __START();
    RUN_CMD(&g_command, );
    __EXPECT(0, g_L0_help, "", "L0 ");
    __END();
}

UTEST(nested_procedure, L1)
{
    __START();
    RUN_CMD(&g_command, "L1");
    __EXPECT(0, g_L1_help, "", "L0 L1 ");
    __END();
}

UTEST(nested_procedure, L1_L2)
{
    __START();
    RUN_CMD(&g_command, "L1", "L2");
    __EXPECT(0, g_L2_help, "", "L0 L1 L2 ");
    __END();
}


UTEST(nested_procedure, L0_other)
{
    __START();
    RUN_CMD(&g_command, "other");
    __EXPECT(1, "", err_other, "");
    __END();
}

UTEST(nested_procedure, L1_other)
{
    __START();
    RUN_CMD(&g_command, "L1", "other");
    __EXPECT(1, "", err_other, "");
    __END();
}

UTEST(nested_procedure, L1_L2_other)
{
    __START();
    RUN_CMD(&g_command, "L1", "L2", "other");
    __EXPECT(0, "", "", "L0 L1 L2 ");
    __END();
}


UTEST(nested_procedure, L1_L2_params__L0_over)
{
    __START();
    __ACTION_SET(0, CMDP_ACT_OK);
    RUN_CMD(&g_command, "L1", "L2", "other");
    __EXPECT(0, "", "", "L0 ");
    __END();
}

UTEST(nested_procedure, L1_L2_params__L0_fail)
{
    __START();
    __ACTION_SET(0, CMDP_ACT_ERROR);
    RUN_CMD(&g_command, "L1", "L2", "other");
    __EXPECT(1, "", "", "L0 ");
    __END();
}

UTEST(nested_procedure, L1_L2_params__L0_over_help)
{
    __START();
    __ACTION_SET(0, CMDP_ACT_OK | CMDP_ACT_SHOW_HELP);
    RUN_CMD(&g_command, "L1", "L2", "other");
    __EXPECT(0, g_L0_help, "", "L0 ");
    __END();
}

UTEST(nested_procedure, L1_L2_params__L0_fail_help)
{
    __START();
    __ACTION_SET(0, CMDP_ACT_ERROR | CMDP_ACT_SHOW_HELP);
    RUN_CMD(&g_command, "L1", "L2", "other");
    __EXPECT(1, "", g_L0_help, "L0 ");
    __END();
}
