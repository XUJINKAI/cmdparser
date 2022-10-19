#include "cmdparser.h"

static cmdp_action_t cb_pull(cmdp_process_param_st *params);

cmdp_command_st cmdp_pull = {
    "pull",
    "Fetch from and integrate with another repository or a local branch",
    "usage: git pull\n\n",
    .fn_process = cb_pull,
};

static cmdp_action_t cb_pull(cmdp_process_param_st *params)
{
    printf("cb_pull called.\n");
    return CMDP_ACT_OVER;
}
