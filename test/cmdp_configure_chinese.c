#include "common.h"

static void __err_parse_handler(cmdp_error_params_st *params)
{
    switch (params->type)
    {
        case CMDP_ERROR_UNKNOWN_COMMAND:
            fprintf(params->err_stream, "未知命令: %s.\n", params->s);
            break;
        case CMDP_ERROR_UNKNOWN_OPTION:
            if (params->c)
            {
                fprintf(params->err_stream, "未知选项: -%c.\n", params->c);
            }
            if (params->s)
            {
                fprintf(params->err_stream, "未知选项: --%s.\n", params->s);
            }
            break;
        case CMDP_ERROR_MISSING_OPTION_ARG:
            if (params->c)
            {
                fprintf(params->err_stream, "选项-%c需要参数.\n", params->c);
            }
            if (params->s)
            {
                fprintf(params->err_stream, "选项--%s需要参数.\n", params->s);
            }
            break;
        case CMDP_ERROR_PARSE_INT:
            fprintf(params->err_stream, "解析整数失败: %s.\n", params->s);
            break;
        case CMDP_ERROR_PARSE_DOUBLE:
            fprintf(params->err_stream, "解析数字失败: %s.\n", params->s);
            break;
        case CMDP_ERROR_REPEAT_OPTION:
            if (params->c)
            {
                fprintf(params->err_stream, "重复选项: -%c.\n", params->c);
            }
            if (params->s)
            {
                fprintf(params->err_stream, "重复选项: --%s.\n", params->s);
            }
            break;
        default:
            break;
    }
}

void cmdp_configure_chinese()
{
    cmdp_global_config_st *config = cmdp_get_global_config();
    config->fn_error_parse        = __err_parse_handler;
}
