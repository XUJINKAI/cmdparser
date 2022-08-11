/* 
Command parser library
Author: XUJINKAI
Url: https://github.com/XUJINKAI/cmdparser
LICENSE: MIT
 */
#ifndef __CMD_PARSER_H__
#define __CMD_PARSER_H__

#include <stdbool.h>
#include <stdio.h>


typedef enum cmdp_result_t
{
    CMDP_OK   = 0,
    CMDP_FAIL = 1,
} cmdp_result_t;

typedef enum cmdp_action_t
{
    // continue parse sub commands (if have)
    CMDP_ACT_CONTINUE = 0,
    // stop parse sub commands, return OK
    CMDP_ACT_OVER,
    // stop parse sub commands, return FAIL
    CMDP_ACT_FAIL,

    // [FLAG] show current command's message
    CMDP_ACT_SHOW_HELP = 0x10000,
} cmdp_action_t;

typedef enum cmdp_flag_t
{
    CMDP_FLAG_NONE    = 0,
    CMDP_FLAG_HIDE    = 1,
    CMDP_FLAG_DISABLE = 2,
} cmdp_flag_t;

typedef enum cmdp_type_t
{
    CMDP_TYPE_NONE = 0,
    CMDP_TYPE_BOOL = 1,   // bool
    CMDP_TYPE_INT4,       // int
    CMDP_TYPE_DOUBLE,     // double
    CMDP_TYPE_STRING_PTR, // char*
} cmdp_type_t;


typedef struct cmdp_option_st cmdp_option_st;
typedef struct cmdp_command_st cmdp_command_st;

typedef struct cmdp_before_param_st
{
    int argc;
    char **argv;
    cmdp_command_st *cmdp;
    int sub_level; // sub-command level, 0 is main command
} cmdp_before_param_st;

typedef struct cmdp_process_param_st
{
    int argc;
    char **argv;
    cmdp_command_st *cmdp;
    int opts;      // parsed options count
    int sub_level; // sub-command level, 0 is main command
} cmdp_process_param_st;


struct cmdp_command_st
{
    /* (sub) command's name */
    char *name;
    /* one-line subcommand description */
    char *desc;
    /* multi-line document */
    char *doc;

    /* end with {0} */
    cmdp_option_st *options;
    /* end with {0} */
    cmdp_command_st *sub_commands;

    // run before command's options parse
    void (*fn_before)(cmdp_before_param_st *params);
    // run after command's options parse, before sub-commands run
    cmdp_action_t (*fn_process)(cmdp_process_param_st *params);

    // hide or disable
    cmdp_flag_t (*fn_flag)(cmdp_command_st *self);

    // shortcut for name, e.g. "s, state        Show current state"
    char *alias_name;
};

struct cmdp_option_st
{
    // visible char (or just zero)
    int short_option;
    char *long_option;
    char *description;
    // output type
    cmdp_type_t type;
    // variable
    void *output_ptr;
    // (if not bool) e.g. "--long=<type_name>"
    char *type_name;

    // hide or disable
    cmdp_flag_t (*fn_flag)(cmdp_option_st *self);
};

#define CMDP_HIDE .fn_flag = cmdp_flag_always_hide
cmdp_flag_t cmdp_flag_always_hide(cmdp_option_st *self);

void cmdp_fprint_options_doc(FILE *fp, cmdp_option_st *options);
void cmdp_fprint_command_doc(FILE *fp, cmdp_command_st *command);
void cmdp_fprint_all_documents(FILE *fp, cmdp_command_st *command);

// print help message to output stream
void cmdp_help(cmdp_command_st *command);

/* 
run arguments with a cmdp_command_st

first argument is not program name, e.g.
int main(int argc, char**argv) {
    return cmdp_run(argc - 1, argv + 1, &root_command);
}
 */
cmdp_result_t cmdp_run(int argc, char **argv, cmdp_command_st *root_command);


// ===================
// global config
// ===================

typedef struct cmdp_global_config_st
{
    char help_short_option;
    char *help_long_option;

    void (*fn_doc_gen_options)(FILE *fp, cmdp_option_st *options);
    void (*fn_doc_gen_command)(FILE *fp, cmdp_command_st *command);

    void (*fn_error_unknown_command)(FILE *fp, cmdp_command_st *cmdp, char *s);
    void (*fn_error_unknown_short_option)(FILE *fp, cmdp_command_st *cmdp, char c);
    void (*fn_error_unknown_long_option)(FILE *fp, cmdp_command_st *cmdp, char *s);
    void (*fn_error_require_short_option_arg)(FILE *fp, cmdp_command_st *cmdp, char c);
    void (*fn_error_require_long_option_arg)(FILE *fp, cmdp_command_st *cmdp, char *s);
    void (*fn_error_parse_int)(FILE *fp, cmdp_command_st *cmdp, char *s);
    void (*fn_error_parse_float)(FILE *fp, cmdp_command_st *cmdp, char *s);

    FILE *out_stream;
    FILE *err_stream;
} cmdp_global_config_st;

cmdp_global_config_st *cmdp_get_global_config();
void cmdp_reset_global_config();

#endif /* __CMD_PARSER_H__ */
