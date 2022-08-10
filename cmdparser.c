/* 
Command parser library
Author: XUJINKAI
Url: https://github.com/XUJINKAI/cmdparser
LICENSE: MIT
 */

#include "cmdparser.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// ============================================================================
// utils
// ============================================================================

#define HAS_FLAG(VAR, FLAG) (((VAR) & (FLAG)) == (FLAG))

static bool is_all_zero(void *__p, size_t __l)
{
    char *pt = (char *)__p;
    while (__l--)
    {
        if (*pt++)
        {
            return false;
        }
    }
    return true;
}

static int nonzero_countof(void *array, int element_size)
{
    if (array == NULL)
        return 0;
    int count = 0;
    char *pt  = (char *)array;
    while (!is_all_zero(pt, element_size))
    {
        pt += element_size;
        count++;
    }
    return count;
}

// ============================================================================
// default methods
// ============================================================================

static cmdp_global_config_st g_config = {
    .help_short_option = 'h',
    .help_long_option  = "help",
};

static void reset_global_config()
{
    memset(&g_config, 0, sizeof(g_config));
    g_config.help_short_option = 'h';
    g_config.help_long_option  = "help";
}

#define OUT_STREAM (g_config.out_stream != NULL ? g_config.out_stream : stdout)
#define ERR_STREAM (g_config.err_stream != NULL ? g_config.err_stream : stderr)

#define HELP_PREFIX_LEN 29
#define IS_INPUTABLE_KEY(CH)                                                                                           \
    ((((CH) >= 'a') && ((CH) <= 'z')) || (((CH) >= 'A') && ((CH) <= 'Z')) || (((CH) >= '0') && ((CH) <= '9')))

static void error_unknown_command(cmdp_command_st *cmdp, char *s)
{
    if (g_config.fn_error_unknown_command)
    {
        g_config.fn_error_unknown_command(ERR_STREAM, cmdp, s);
        return;
    }
    fprintf(ERR_STREAM, "Unknown command %s.\n", s);
}
static void error_unknown_short_option(cmdp_command_st *cmdp, char c)
{
    if (g_config.fn_error_unknown_short_option)
    {
        g_config.fn_error_unknown_short_option(ERR_STREAM, cmdp, c);
        return;
    }
    fprintf(ERR_STREAM, "Unknown option -%c.\n", c);
}
static void error_unknown_long_option(cmdp_command_st *cmdp, char *s)
{
    if (g_config.fn_error_unknown_long_option)
    {
        g_config.fn_error_unknown_long_option(ERR_STREAM, cmdp, s);
        return;
    }
    fprintf(ERR_STREAM, "Unknown option --%s.\n", s);
}
static void error_require_short_option_arg(cmdp_command_st *cmdp, char c)
{
    if (g_config.fn_error_require_short_option_arg)
    {
        g_config.fn_error_require_short_option_arg(ERR_STREAM, cmdp, c);
        return;
    }
    fprintf(ERR_STREAM, "Option -%c require args.\n", c);
}
static void error_require_long_option_arg(cmdp_command_st *cmdp, char *s)
{
    if (g_config.fn_error_require_long_option_arg)
    {
        g_config.fn_error_require_long_option_arg(ERR_STREAM, cmdp, s);
        return;
    }
    fprintf(ERR_STREAM, "Option --%s require args.\n", s);
}
static void error_parse_int(cmdp_command_st *cmdp, char *s)
{
    if (g_config.fn_error_parse_int)
    {
        g_config.fn_error_parse_int(ERR_STREAM, cmdp, s);
        return;
    }
    fprintf(ERR_STREAM, "Parse int failed: %s.\n", s);
}
static void error_parse_float(cmdp_command_st *cmdp, char *s)
{
    if (g_config.fn_error_parse_float)
    {
        g_config.fn_error_parse_float(ERR_STREAM, cmdp, s);
        return;
    }
    fprintf(ERR_STREAM, "Parse float failed: %s.\n", s);
}
static void doc_gen_options(FILE *fp, cmdp_option_st *options)
{
    if (g_config.fn_doc_gen_options)
    {
        g_config.fn_doc_gen_options(fp, options);
        return;
    }
    /*
    \s\s[command]\s{29-2-len}doc\n
        -h, --help                 Give this help list
    */
    cmdp_option_st *n = options;
    int count         = nonzero_countof(options, sizeof(cmdp_option_st));
    for (int i = 0; i < count; i++)
    {
        if (n->fn_flag)
        {
            cmdp_flag_t flag = n->fn_flag(n);
            if (HAS_FLAG(flag, CMDP_FLAG_HIDE) || HAS_FLAG(flag, CMDP_FLAG_DISABLE))
            {
                continue;
            }
        }

        fprintf(fp, "  ");
        int pos = 2;

        if (IS_INPUTABLE_KEY(n->short_option))
        {
            fprintf(fp, "%c%c", '-', n->short_option);
            pos += 2;

            if (n->long_option != NULL)
            {
                fprintf(fp, ", ");
                pos += 2;
            }
        }

        if (n->long_option != NULL)
        {
            fprintf(fp, "%s%s", "--", n->long_option);
            pos += 2 + strlen(n->long_option);
        }

        char *alias = n->type_name;
        if (alias == NULL)
        {
            switch (n->type)
            {
                case CMDP_TYPE_STRING_PTR:
                    alias = "<string>";
                    break;
                case CMDP_TYPE_INT4:
                    alias = "<int>";
                    break;
                case CMDP_TYPE_DOUBLE:
                    alias = "<number>";
                    break;
                case CMDP_TYPE_NONE:
                case CMDP_TYPE_BOOL:
                    break;
            }
        }
        if (alias != NULL)
        {
            fprintf(fp, "%s%s", ((n->long_option == NULL) ? " " : "="), alias);
            pos += 1 + strlen(alias);
        }

        long remain_prefix = HELP_PREFIX_LEN - pos;
        long repeat_count  = remain_prefix > 2 ? remain_prefix : 2;
        while (repeat_count--)
        {
            fputc(' ', fp);
        }

        fprintf(fp, "%s\n", n->description);
        n++;
    }
}

static void doc_gen_command(FILE *fp, cmdp_command_st *command)
{
    if (g_config.fn_doc_gen_command)
    {
        g_config.fn_doc_gen_command(fp, command);
        return;
    }

    if (command->doc)
    {
        fputs(command->doc, fp);
    }

    if (command->options != NULL)
    {
        cmdp_fprint_options_doc(fp, command->options);
    }

    if (command->sub_commands != NULL)
    {
        int sub_command_count = nonzero_countof(command->sub_commands, sizeof(cmdp_command_st));
        for (int i = 0; i < sub_command_count; i++)
        {
            cmdp_command_st *n = command->sub_commands + i;
            if (n->fn_flag)
            {
                cmdp_flag_t flag = n->fn_flag(n);
                if (HAS_FLAG(flag, CMDP_FLAG_HIDE) || HAS_FLAG(flag, CMDP_FLAG_DISABLE))
                {
                    continue;
                }
            }

            if (n->name == NULL)
            {
                if (n->desc)
                {
                    fputs(n->desc, fp);
                }
                if (n->doc)
                {
                    fputs(n->doc, fp);
                }
            }
            else
            {
                fprintf(fp, "  ");
                int pos = 2;
                if (n->alias_name != NULL)
                {
                    fprintf(fp, "%s, \n", n->alias_name);
                    pos += strlen(n->alias_name) + 2;
                }
                fprintf(fp, "%s", n->name);
                pos += strlen(n->name);
                long remain_prefix = HELP_PREFIX_LEN - pos;
                while (remain_prefix--)
                {
                    fputc(' ', fp);
                }
                fprintf(fp, "%s\n", n->desc);
            }
            n++;
        }
    }
}

// ============================================================================
// main logic
// ============================================================================

static void cmdp_fprint_all_documents_recursive(FILE *fp, cmdp_command_st *cmdp, char *command_name, bool colored)
{
#define GREEN(STRING) "\x1b[32m" STRING "\x1b[0m"
#define CYAN(STRING)  "\x1b[36m" STRING "\x1b[0m"

    fprintf(fp, (colored ? CYAN(">>") " " GREEN("%s") "\n" : ">> %s\n"), command_name);
    cmdp_fprint_command_doc(fp, cmdp);

    int count = nonzero_countof(cmdp->sub_commands, sizeof(cmdp_command_st));
    for (int i = 0; i < count; i++)
    {
        cmdp_command_st *p = cmdp->sub_commands + i;

        if (p->fn_flag)
        {
            cmdp_flag_t flag = p->fn_flag(p);
            if (HAS_FLAG(flag, CMDP_FLAG_HIDE) || HAS_FLAG(flag, CMDP_FLAG_DISABLE))
            {
                continue;
            }
        }

        if (p->name == NULL)
        {
            continue;
        }
        char cmd_name[strlen(command_name) + strlen(p->name) + 2];
        snprintf(cmd_name, sizeof(cmd_name), "%s %s", command_name, p->name);

        fprintf(fp, "\n");
        cmdp_fprint_all_documents_recursive(fp, p, cmd_name, colored);
    }
}

static cmdp_option_st *find_short_option(int short_option, cmdp_option_st *options, int count)
{
    for (int i = 0; i < count; i++)
    {
        cmdp_option_st *p = options + i;
        if (p->short_option != 0 && p->short_option == short_option)
        {
            if (p->fn_flag && HAS_FLAG(p->fn_flag(p), CMDP_FLAG_DISABLE))
            {
                return NULL;
            }
            return p;
        }
    }
    return NULL;
}

static cmdp_option_st *find_long_option(char *long_option, cmdp_option_st *options, int count)
{
    for (int i = 0; i < count; i++)
    {
        cmdp_option_st *p = options + i;
        if (p->long_option != 0 && strcmp(p->long_option, long_option) == 0)
        {
            if (p->fn_flag && HAS_FLAG(p->fn_flag(p), CMDP_FLAG_DISABLE))
            {
                return NULL;
            }
            return p;
        }
    }
    return NULL;
}

static cmdp_command_st *find_command(char *command_name, cmdp_command_st *commands, int count)
{
    for (int i = 0; i < count; i++)
    {
        cmdp_command_st *p = commands + i;
        if (p->name == NULL)
        {
            continue;
        }

        if (strcmp(command_name, p->name) == 0 || (p->alias_name != NULL && (strcmp(command_name, p->alias_name) == 0)))
        {
            if (p->fn_flag && HAS_FLAG(p->fn_flag(p), CMDP_FLAG_DISABLE))
            {
                return NULL;
            }
            return p;
        }
    }
    return NULL;
}

#define _EC_ERROR_MASK_ 0x100

typedef enum
{
    EVAL_CODE_DONE,
    EVAL_CODE_ARG_USED,

    EVAL_CODE_ERROR_ARG_NULL = _EC_ERROR_MASK_,
    EVAL_CODE_ERROR_PARSE_INT,
    EVAL_CODE_ERROR_PARSE_FLOAT,
} EVAL_CODE;

static EVAL_CODE eval_option_output(cmdp_option_st *option, char *arg)
{
    switch (option->type)
    {
        case CMDP_TYPE_BOOL:
            if (option->output_ptr != NULL)
            {
                *(bool *)option->output_ptr = true;
            }
            return EVAL_CODE_DONE;

        case CMDP_TYPE_INT4:
            if (arg == NULL)
            {
                return EVAL_CODE_ERROR_ARG_NULL;
            }
            char *endl = NULL;
            long l     = strtol(arg, &endl, 10);
            if (endl == arg || *endl != '\0')
            {
                return EVAL_CODE_ERROR_PARSE_INT;
            }
            if (option->output_ptr != NULL)
            {
                *(int *)option->output_ptr = l;
            }
            return EVAL_CODE_ARG_USED;

        case CMDP_TYPE_DOUBLE:
            if (arg == NULL)
            {
                return EVAL_CODE_ERROR_ARG_NULL;
            }
            char *endd = NULL;
            double d   = strtod(arg, &endd);
            if (endd == arg || *endd != '\0')
            {
                return EVAL_CODE_ERROR_PARSE_FLOAT;
            }
            if (option->output_ptr != NULL)
            {
                *(double *)option->output_ptr = d;
            }
            return EVAL_CODE_ARG_USED;

        case CMDP_TYPE_STRING_PTR:
            if (arg == NULL)
            {
                return EVAL_CODE_ERROR_ARG_NULL;
            }
            if (option->output_ptr != NULL)
            {
                *(char **)option->output_ptr = arg;
            }
            return EVAL_CODE_ARG_USED;

        case CMDP_TYPE_NONE:
        default:
            return 0;
    }
    return 0;
}

static cmdp_result_t cmdp_run_recursive(int argc, char **argv, cmdp_command_st *cmdp, int recursive)
{
    cmdp_before_param_st fn_before_param = {
        .argc = argc,
        .argv = argv,
        .cmdp = cmdp,
    };
    if (g_config.fn_global_before)
    {
        g_config.fn_global_before(&fn_before_param);
    }
    if (cmdp->fn_before)
    {
        cmdp->fn_before(&fn_before_param);
    }
    if (argc == 0 && cmdp->fn_process == NULL)
    {
        cmdp_help(cmdp);
        return CMDP_OK;
    }

    int parsed_options = 0;
    int arg_index      = 0;
    for (; arg_index < argc; arg_index++)
    {
        char *cur_arg     = argv[arg_index];
        char *next_arg    = (arg_index + 1) < argc ? argv[arg_index + 1] : NULL;
        int cur_len       = strlen(cur_arg);
        int options_count = nonzero_countof(cmdp->options, sizeof(cmdp_option_st));
        // -short
        if (cur_len >= 2 && cur_arg[0] == '-' && cur_arg[1] != '-')
        {
            EVAL_CODE eval_code = EVAL_CODE_DONE;
            char *eval_arg      = NULL;
            char cur_ch         = '\0';
            for (int ch_idx = 1; ch_idx < cur_len; ch_idx++)
            {
                // e.g. -anop <NAME>. loop char by char
                cur_ch = cur_arg[ch_idx];
                if (g_config.help_short_option != '\0' && cur_ch == g_config.help_short_option)
                {
                    cmdp_help(cmdp);
                    return CMDP_OK;
                }

                cmdp_option_st *find = find_short_option(cur_ch, cmdp->options, options_count);
                if (find == NULL)
                {
                    error_unknown_short_option(cmdp, cur_ch);
                    return CMDP_FAIL;
                }
                eval_arg  = (eval_code == EVAL_CODE_ARG_USED) ? NULL : next_arg;
                eval_code = eval_option_output(find, eval_arg);
                if (HAS_FLAG(eval_code, _EC_ERROR_MASK_))
                {
                    break;
                }
            }

            switch (eval_code)
            {
                case EVAL_CODE_ARG_USED:
                    arg_index++;
                    break;
                case EVAL_CODE_ERROR_ARG_NULL:
                    error_require_short_option_arg(cmdp, cur_ch);
                    return CMDP_FAIL;
                case EVAL_CODE_ERROR_PARSE_INT:
                    error_parse_int(cmdp, eval_arg);
                    return CMDP_FAIL;
                case EVAL_CODE_ERROR_PARSE_FLOAT:
                    error_parse_float(cmdp, eval_arg);
                    return CMDP_FAIL;
                case EVAL_CODE_DONE:
                    break;
            }
            parsed_options += 1;
        }
        // --long
        else if (cur_len >= 3 && cur_arg[0] == '-' && cur_arg[1] == '-')
        {
            /*
            --path=<PATH>   eq_ptr=long_option_start+n
            --path <PATH>   eq_ptr=NULL, next_arg=<PATH>
             */
            char *long_option_start = cur_arg + 2;
            char *eq_ptr            = strchr(long_option_start, '=');
            int long_len_calc       = (eq_ptr == NULL) ? (int)strlen(long_option_start) : (eq_ptr - long_option_start);
            char long_option_real[128] = {0};
            strncpy(long_option_real, long_option_start, long_len_calc);

            if (g_config.help_long_option != NULL && strcmp(g_config.help_long_option, long_option_start) == 0)
            {
                cmdp_help(cmdp);
                return CMDP_OK;
            }

            cmdp_option_st *find = find_long_option(long_option_real, cmdp->options, options_count);
            if (find == NULL)
            {
                error_unknown_long_option(cmdp, long_option_real);
                return CMDP_FAIL;
            }
            char *eval_arg      = (eq_ptr == NULL) ? next_arg : eq_ptr + 1;
            EVAL_CODE eval_code = eval_option_output(find, eval_arg);
            switch (eval_code)
            {
                case EVAL_CODE_ARG_USED:
                    if (eq_ptr == NULL)
                    {
                        arg_index++;
                    }
                    break;
                case EVAL_CODE_ERROR_ARG_NULL:
                    error_require_long_option_arg(cmdp, long_option_real);
                    return CMDP_FAIL;
                case EVAL_CODE_ERROR_PARSE_INT:
                    error_parse_int(cmdp, eval_arg);
                    return CMDP_FAIL;
                case EVAL_CODE_ERROR_PARSE_FLOAT:
                    error_parse_float(cmdp, eval_arg);
                    return CMDP_FAIL;
                case EVAL_CODE_DONE:
                    break;
            }
            parsed_options += 1;
        }
        else
        {
            // not option, break
            break;
        }
    }

    if (cmdp->fn_process != NULL)
    {
        cmdp_process_param_st fn_process_param = {
            .argc = argc - arg_index,
            .argv = argv + arg_index,
            .cmdp = cmdp,
            .opts = parsed_options,
        };
        cmdp_action_t code = cmdp->fn_process(&fn_process_param);
        bool show_help     = HAS_FLAG(code, CMDP_ACT_SHOW_HELP);
        switch (code & 0xFFFF)
        {
            case CMDP_ACT_OVER:
                if (show_help)
                {
                    cmdp_fprint_command_doc(OUT_STREAM, cmdp);
                }
                return CMDP_OK;
            case CMDP_ACT_FAIL:
                if (show_help)
                {
                    cmdp_fprint_command_doc(ERR_STREAM, cmdp);
                }
                return CMDP_FAIL;
            case CMDP_ACT_CONTINUE:
                if (show_help)
                {
                    cmdp_fprint_command_doc(OUT_STREAM, cmdp);
                }
            default:
                break;
        }
    }

    if (arg_index < argc)
    {
        int commands_count    = nonzero_countof(cmdp->sub_commands, sizeof(cmdp_command_st));
        cmdp_command_st *find = find_command(argv[arg_index], cmdp->sub_commands, commands_count);
        if (find == NULL)
        {
            error_unknown_command(cmdp, argv[arg_index]);
            return CMDP_FAIL;
        }
        return cmdp_run_recursive(argc - 1, argv + 1, find, recursive + 1);
    }
    return CMDP_OK;
}

// ============================================================================
// expose
// ============================================================================

cmdp_flag_t cmdp_flag_always_hide(cmdp_option_st *self)
{
    (void)self;
    return CMDP_FLAG_HIDE;
}

void cmdp_fprint_options_doc(FILE *fp, cmdp_option_st *options)
{
    doc_gen_options(fp, options);
}
void cmdp_fprint_command_doc(FILE *fp, cmdp_command_st *command)
{
    doc_gen_command(fp, command);
}
void cmdp_fprint_all_documents(FILE *fp, cmdp_command_st *command)
{
    cmdp_fprint_all_documents_recursive(fp, command, command->name ? command->name : "", isatty(fileno(fp)));
}

void cmdp_help(cmdp_command_st *command)
{
    cmdp_fprint_command_doc(OUT_STREAM, command);
}
cmdp_result_t cmdp_run(int argc, char **argv, cmdp_command_st *root_command)
{
    return cmdp_run_recursive(argc, argv, root_command, 1);
}

cmdp_global_config_st *cmdp_get_global_config()
{
    return &g_config;
}

void cmdp_reset_global_config()
{
    reset_global_config();
}
