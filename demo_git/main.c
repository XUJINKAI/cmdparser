#include "cmdparser.h"

extern cmdp_command_st cmdp_clone;
extern cmdp_command_st cmdp_pull;
static cmdp_action_t cb_top_level(cmdp_process_param_st *params);

struct
{
    bool version;
    char *git_dir;
    char *work_tree;
} arg_top;

static cmdp_command_st cmdp_top = {
    .doc = "Usage: git [--version] [--help]\n"
           "           [--git-dir=<path>] [--work-tree=<path>]\n"
           "           <command> [<args>]\n"
           "\n",
    .options =
        (cmdp_option_st[]){
            {0, "version", "Show version", CMDP_TYPE_BOOL, &arg_top.version, CMDP_HIDE},
            {0, "git-dir", ".git directory path", CMDP_TYPE_STRING_PTR, &arg_top.git_dir, CMDP_HIDE},
            {0, "work-tree", "work directory path", CMDP_TYPE_STRING_PTR, &arg_top.work_tree, CMDP_HIDE},
            {0},
        },
    .sub_commands =
        (cmdp_command_st *[]){
            CMDP_DOC("These are common Git commands used in various situations:\n"
                     "\n"
                     "start a working area (see also: git help tutorial)\n"),
            &cmdp_clone,
            CMDP_DOC("\n"
                     "collaborate (see also: git help workflows)\n"),
            &cmdp_pull,
            CMDP_DOC("\n"
                     "cmdparser git style demo by XUJINKAI.\n"
                     "https://github.com/XUJINKAI/cmdparser\n"),
            NULL,
        },
    .fn_process = cb_top_level,
};

int main(int argc, char **argv)
{
    return cmdp_run(argc - 1, argv + 1, &cmdp_top);
}

static cmdp_action_t cb_top_level(cmdp_process_param_st *params)
{
    if (arg_top.version)
    {
        printf("git version cmdparser by XUJINKAI\n");
        return CMDP_ACT_OVER;
    }
    else if (params->argc == 0)
    {
        return CMDP_ACT_SHOW_HELP;
    }
    return CMDP_ACT_CONTINUE;
}
