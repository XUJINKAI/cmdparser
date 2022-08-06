#include "common.h"

/* 
make memory args="--filter=nested.*"
 */


static cmdp_command_st g_command = {
    "App",
    .sub_commands =
        (cmdp_command_st[]){
            {"init", "init something"},
            {"run", "run something",
             .sub_commands =
                 (cmdp_command_st[]){
                     {"app-a", "run app a"},
                     {"app-b", "run app b"},
                     {0},
                 }},
            {"test", "test something"},
            {"help", "help doc"},
            {"exit", "exit app"},
            {0},
        },
};


UTEST(nested, help)
{
    START_CMD();
    RUN_CMD(&g_command, );
    END_CMD();
}
