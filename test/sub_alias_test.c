#include "common.h"

static cmdp_command_st g_command = {
    "App",
    .sub_commands =
        (cmdp_command_st[]){
            {"init", "init something"},
            {"run", "run something", .alias_name = "r",
             .sub_commands =
                 (cmdp_command_st[]){
                     {"app-a", "run app a", .alias_name = "a"},
                     {"app-b", "run app b"},
                     {0},
                 }},
            {"test", "test something", .alias_name = "t"},
            {"help", "help doc", .alias_name = "h"},
            {"exit", "exit app"},
            {0},
        },
};

static const char *g_App_help = "  init                       init something\n"
                                "  r, run                     run something\n"
                                "  t, test                    test something\n"
                                "  h, help                    help doc\n"
                                "  exit                       exit app\n";
static const char *g_run_help = "  a, app-a                   run app a\n"
                                "  app-b                      run app b\n";

UTEST(sub_alias, void)
{
    START_CMD();
    RUN_CMD(&g_command, );
    EXPECT_CMD(CMDP_OK, g_App_help, "");
    END_CMD();
}

UTEST(sub_alias, r)
{
    START_CMD();
    RUN_CMD(&g_command, "r");
    EXPECT_CMD(CMDP_OK, g_run_help, "");
    END_CMD();
}

UTEST(sub_alias, r_a)
{
    START_CMD();
    RUN_CMD(&g_command, "r", "a");
    EXPECT_CMD(CMDP_OK, "", "");
    END_CMD();
}

UTEST(sub_alias, r_b)
{
    START_CMD();
    RUN_CMD(&g_command, "r", "b");
    EXPECT_CMD(CMDP_FAIL, "", "Unknown command b.\n");
    END_CMD();
}
