#include "common.h"

static cmdp_command_st g_command = {
    .doc = "中文测试\n\n",
    .sub_commands =
        (cmdp_command_st *[]){
            &(cmdp_command_st){"init", "初始化"},
            &(cmdp_command_st){"exit", "退出"},
            CMDP_DOC("\n"),
            NULL,
        },
    .options =
        (cmdp_option_st[]){
            {'n', "name", "姓名字符串", CMDP_TYPE_STRING_PTR},
            {'s', "student", "是否学生", CMDP_TYPE_BOOL},
            {'a', "age", "年龄（整数）", CMDP_TYPE_INT4},
            {0, "height", "身高（米）", CMDP_TYPE_DOUBLE},
            {0},
        },
};

static char *g_expect_help = "中文测试\n"
                             "\n"
                             "  init                       初始化\n"
                             "  exit                       退出\n"
                             "\n"
                             "  -n, --name <string>        姓名字符串\n"
                             "  -s, --student              是否学生\n"
                             "  -a, --age <int>            年龄（整数）\n"
                             "  --height <number>          身高（米）\n";


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


#define __START()                                                                                                      \
    START_CMD();                                                                                                       \
    _ctx.fn_error_parse = __err_parse_handler;
#define __END() END_CMD();


UTEST(chinese, doc)
{
    __START();
    RUN_CMD(&g_command, );
    EXPECT_CMD(0, g_expect_help, "");
    __END();
}

UTEST(chinese, error_unknown_command)
{
    __START();
    RUN_CMD(&g_command, "unknown");
    EXPECT_CMD(1, "", "未知命令: unknown.\n");
    __END();
}

UTEST(chinese, error_unknown_short_opt)
{
    __START();
    RUN_CMD(&g_command, "-u");
    EXPECT_CMD(1, "", "未知选项: -u.\n");
    __END();
}

UTEST(chinese, error_unknown_long_opt)
{
    __START();
    RUN_CMD(&g_command, "--unknown");
    EXPECT_CMD(1, "", "未知选项: --unknown.\n");
    __END();
}

UTEST(chinese, error_require_arg_short)
{
    __START();
    RUN_CMD(&g_command, "-n");
    EXPECT_CMD(1, "", "选项-n需要参数.\n");
    __END();
}

UTEST(chinese, error_require_arg_long)
{
    __START();
    RUN_CMD(&g_command, "--name");
    EXPECT_CMD(1, "", "选项--name需要参数.\n");
    __END();
}

UTEST(chinese, error_parse_int)
{
    __START();
    RUN_CMD(&g_command, "--age", "1.234");
    EXPECT_CMD(1, "", "解析整数失败: 1.234.\n");
    __END();
}

UTEST(chinese, error_parse_float)
{
    __START();
    RUN_CMD(&g_command, "--height", "1.234a");
    EXPECT_CMD(1, "", "解析数字失败: 1.234a.\n");
    __END();
}

UTEST(chinese, error_repeat_option)
{
    __START();
    RUN_CMD(&g_command, "--name", "name", "--name", "name");
    EXPECT_CMD(1, "", "重复选项: --name.\n");
    __END();
}
