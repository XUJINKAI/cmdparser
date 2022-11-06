#include "common.h"

static cmdp_command_st g_command = {
    .doc = "doc\n",
    .options =
        (cmdp_option_st[]){
            {'v', "verbose", "Verbose Log"},
            {0, "looooooooooooooooooooooooooooooog", "Long option will break line"},
            {0},
        },
    .sub_commands =
        (cmdp_command_st *[]){
            &(cmdp_command_st){
                "run",
                "run command",
            },
            NULL,
        },
};
static const char *g_main_help = "doc\n"
                                 "  run                        run command\n"
                                 "  -v, --verbose              Verbose Log\n"
                                 "  --looooooooooooooooooooooooooooooog\n"
                                 "                             Long option will break line\n";

#define __START(h_short, h_long)                                                                                       \
    START_CMD();                                                                                                       \
    _ctx.help_short_option = h_short;                                                                                  \
    _ctx.help_long_option  = h_long;
#define __END() END_CMD();


UTEST(help_option, NULL__h)
{
    __START(0, NULL);
    RUN_CMD(&g_command, "-h");
    EXPECT_CMD(1, "", "Unknown option -h.\n");
    __END();
}

UTEST(help_option, NULL__help)
{
    __START(0, NULL);
    RUN_CMD(&g_command, "--help");
    EXPECT_CMD(1, "", "Unknown option --help.\n");
    __END();
}


UTEST(help_option, custom__h)
{
    __START('?', "?");
    RUN_CMD(&g_command, "-?");
    EXPECT_CMD(0, g_main_help, "");
    __END();
}

UTEST(help_option, custom__help)
{
    __START('?', "?");
    RUN_CMD(&g_command, "--?");
    EXPECT_CMD(0, g_main_help, "");
    __END();
}
