#include "common.h"

static cmdp_command_st g_command = {
    .options =
        (cmdp_option_st[]){
            {'v', "verbose", "Verbose Log"},
            {0},
        },
    .sub_commands =
        (cmdp_command_st[]){
            {
                "run",
                "run command",
            },
            {0},
        },
};
static const char *g_main_help = "  -v, --verbose              Verbose Log\n"
                                 "  run                        run command\n";

#define __START(h_short, h_long)                                                                                       \
    START_CMD();                                                                                                       \
    _cfg->help_short_option = h_short;                                                                                 \
    _cfg->help_long_option  = h_long;
#define __END()                                                                                                        \
    END_CMD();                                                                                                         \
    cmdp_reset_global_config();


UTEST(help_option, NULL__h)
{
    __START(0, NULL);
    RUN_CMD(&g_command, "-h");
    EXPECT_CMD(CMDP_FAIL, "", "Unknown option -h.\n");
    __END();
}

UTEST(help_option, NULL__help)
{
    __START(0, NULL);
    RUN_CMD(&g_command, "--help");
    EXPECT_CMD(CMDP_FAIL, "", "Unknown option --help.\n");
    __END();
}


UTEST(help_option, custom__h)
{
    __START('?', "?");
    RUN_CMD(&g_command, "-?");
    EXPECT_CMD(CMDP_OK, g_main_help, "");
    __END();
}

UTEST(help_option, custom__help)
{
    __START('?', "?");
    RUN_CMD(&g_command, "--?");
    EXPECT_CMD(CMDP_OK, g_main_help, "");
    __END();
}
