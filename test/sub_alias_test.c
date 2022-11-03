#include "common.h"

static cmdp_command_st g_command = {
    "App",
    .sub_commands =
        (cmdp_command_st *[]){
            &(cmdp_command_st){"init", "init something"},
            &(cmdp_command_st){"run", "run something", .alias_name = "r",
                               .sub_commands =
                                   (cmdp_command_st *[]){
                                       &(cmdp_command_st){"app-a", "run app a", .alias_name = "a"},
                                       &(cmdp_command_st){"app-b", "run app b"},
                                       NULL,
                                   }},
            &(cmdp_command_st){"test", "test something", .alias_name = "t"},
            &(cmdp_command_st){"help", "help doc", .alias_name = "h"},
            &(cmdp_command_st){"exit", "exit app"},
            NULL,
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
    EXPECT_CMD(0, g_App_help, "");
    END_CMD();
}

UTEST(sub_alias, r)
{
    START_CMD();
    RUN_CMD(&g_command, "r");
    EXPECT_CMD(0, g_run_help, "");
    END_CMD();
}

UTEST(sub_alias, r_a)
{
    START_CMD();
    RUN_CMD(&g_command, "r", "a");
    EXPECT_CMD(0, "", "");
    END_CMD();
}

UTEST(sub_alias, r_b)
{
    START_CMD();
    RUN_CMD(&g_command, "r", "b");
    EXPECT_CMD(1, "", "Unknown command b.\n");
    END_CMD();
}
