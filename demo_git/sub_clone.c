#include "cmdparser.h"

struct
{
    bool verbose;
    int jobs;
    char *origin;
} arg_clone;

static cmdp_action_t cb_clone(cmdp_process_param_st *params);

cmdp_command_st cmdp_clone = {
    "clone",
    "Clone a repository into a new directory",
    "usage: git clone [<options>] <repo> [<dir>]\n\n",
    (cmdp_option_st[]){
        {'v', "verbose", "be more verbose", CMDP_TYPE_BOOL, &arg_clone.verbose},
        {'j', "jobs", "number of submodules cloned in parallel", CMDP_TYPE_INT4, &arg_clone.jobs, "<n>"},
        {'o', "origin", "use <name> instead of 'origin' to track upstream", CMDP_TYPE_STRING_PTR, &arg_clone.origin,
         "<name>"},
        {0},
    },
    .fn_process = cb_clone,
};

static cmdp_action_t cb_clone(cmdp_process_param_st *params)
{
    if (params->argc == 0)
    {
        fprintf(stderr, "fatal: You must specify a repository to clone.\n\n");
        return CMDP_ACT_FAIL | CMDP_ACT_SHOW_HELP;
    }

    char *repo = NULL;
    char *dir  = NULL;

    if (params->argc == 1)
    {
        repo = params->argv[0];
        dir  = ".";
    }
    else if (params->argc == 2)
    {
        repo = params->argv[0];
        dir  = params->argv[1];
    }
    else
    {
        fprintf(stderr, "fatal: too many arguments\n\n");
        return CMDP_ACT_FAIL | CMDP_ACT_SHOW_HELP;
    }

    printf("verbose: %d\n", arg_clone.verbose);
    printf("jobs: %d\n", arg_clone.jobs);
    printf("origin: %s\n", arg_clone.origin);
    printf("repo: %s\n", repo);
    printf("dir: %s\n", dir);

    return CMDP_ACT_OVER;
}
