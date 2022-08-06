#include "common.h"

static void __err_unknown_command(FILE *fp, cmdp_command_st *cmdp, char *s)
{
    (void)cmdp;
    fprintf(fp, "未知命令: %s.\n", s);
}
static void __err_unknown_short_opt(FILE *fp, cmdp_command_st *cmdp, char c)
{
    (void)cmdp;
    fprintf(fp, "未知选项: -%c.\n", c);
}
static void __err_unknown_long_opt(FILE *fp, cmdp_command_st *cmdp, char *s)
{
    (void)cmdp;
    fprintf(fp, "未知选项: --%s.\n", s);
}
static void __err_require_short_opt_arg(FILE *fp, cmdp_command_st *cmdp, char c)
{
    (void)cmdp;
    fprintf(fp, "选项-%c需要参数.\n", c);
}
static void __err_require_long_opt_arg(FILE *fp, cmdp_command_st *cmdp, char *s)
{
    (void)cmdp;
    fprintf(fp, "选项--%s需要参数.\n", s);
}
static void __err_parse_int(FILE *fp, cmdp_command_st *cmdp, char *s)
{
    (void)cmdp;
    fprintf(fp, "解析整数失败: %s.\n", s);
}
static void __err_parse_float(FILE *fp, cmdp_command_st *cmdp, char *s)
{
    (void)cmdp;
    fprintf(fp, "解析数字失败: %s.\n", s);
}

void cmdp_configure_chinese()
{
    cmdp_global_config_st *config             = cmdp_get_global_config();
    config->fn_error_unknown_command          = __err_unknown_command;
    config->fn_error_unknown_short_option     = __err_unknown_short_opt;
    config->fn_error_unknown_long_option      = __err_unknown_long_opt;
    config->fn_error_require_short_option_arg = __err_require_short_opt_arg;
    config->fn_error_require_long_option_arg  = __err_require_long_opt_arg;
    config->fn_error_parse_int                = __err_parse_int;
    config->fn_error_parse_float              = __err_parse_float;
}
