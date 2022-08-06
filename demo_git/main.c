#include "cmdparser.h"

static cmdp_action_t cb_top_level(int argc, char **argv);
static cmdp_action_t cb_clone(int argc, char **argv);
static cmdp_action_t cb_pull(int argc, char **argv);

struct
{
    bool version;
    char *git_dir;
    char *work_tree;
} arg_top;

struct
{
    bool verbose;
    int jobs;
    char *origin;
} arg_clone;

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
        (cmdp_command_st[]){
            {
                .doc = "These are common Git commands used in various situations:\n"
                       "\n"
                       "start a working area (see also: git help tutorial)\n",
            },
            {
                "clone",
                "Clone a repository into a new directory",
                "usage: git clone [<options>] <repo> [<dir>]\n\n",
                (cmdp_option_st[]){
                    {'v', "verbose", "be more verbose", CMDP_TYPE_BOOL, &arg_clone.verbose},
                    {'j', "jobs", "number of submodules cloned in parallel", CMDP_TYPE_INT4, &arg_clone.jobs, "<n>"},
                    {'o', "origin", "use <name> instead of 'origin' to track upstream", CMDP_TYPE_STRING_PTR,
                     &arg_clone.origin, "<name>"},
                    {0},
                },
                .fn_action = cb_clone,
            },
            {
                .doc = "\n"
                       "collaborate (see also: git help workflows)\n",
            },
            {
                "pull",
                "Fetch from and integrate with another repository or a local branch",
                "usage: git pull\n\n",
                .fn_action = cb_pull,
            },
            {
                .doc = "\n"
                       "cmdparser git style demo by XUJINKAI.\n"
                       "https://github.com/XUJINKAI/cmdparser\n",
            },
            {0},
        },
    .fn_action = cb_top_level,
};

int main(int argc, char **argv)
{
    return cmdp_run(argc - 1, argv + 1, &cmdp_top);
}

static cmdp_action_t cb_top_level(int argc, char **argv)
{
    if (arg_top.version)
    {
        printf("git version cmdparser by XUJINKAI\n");
        return CMDP_ACT_OVER;
    }
    else if (argc == 0)
    {
        return CMDP_ACT_SHOW_HELP;
    }
    return CMDP_ACT_CONTINUE;
}

static cmdp_action_t cb_clone(int argc, char **argv)
{
    if (argc == 0)
    {
        printf("fatal: You must specify a repository to clone.\n\n");
        return CMDP_ACT_FAIL | CMDP_ACT_SHOW_HELP;
    }

    char *repo = NULL;
    char *dir  = NULL;

    if (argc == 1)
    {
        repo = argv[0];
        dir  = ".";
    }
    else if (argc == 2)
    {
        repo = argv[0];
        dir  = argv[1];
    }
    else
    {
        printf("fatal: too many arguments\n\n");
        return CMDP_ACT_FAIL | CMDP_ACT_SHOW_HELP;
    }

    printf("verbose: %d\n", arg_clone.verbose);
    printf("jobs: %d\n", arg_clone.jobs);
    printf("origin: %s\n", arg_clone.origin);
    printf("repo: %s\n", repo);
    printf("dir: %s\n", dir);

    return CMDP_ACT_OVER;
}

static cmdp_action_t cb_pull(int argc, char **argv)
{
    printf("cb_pull called.\n");
    return CMDP_ACT_OVER;
}
