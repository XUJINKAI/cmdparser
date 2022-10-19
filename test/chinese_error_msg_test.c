#include "common.h"

static cmdp_command_st g_command = {
    .doc = "中文测试\n\n",
    .options =
        (cmdp_option_st[]){
            {'n', "name", "姓名字符串", CMDP_TYPE_STRING_PTR},
            {'s', "student", "是否学生", CMDP_TYPE_BOOL},
            {'a', "age", "年龄（整数）", CMDP_TYPE_INT4},
            {0, "height", "身高（米）", CMDP_TYPE_DOUBLE},
            {0},
        },
    .sub_commands =
        (cmdp_command_st *[]){
            CMDP_DOC("\n"),
            &(cmdp_command_st){"init", "初始化"},
            &(cmdp_command_st){"exit", "退出"},
            NULL,
        },
};

static char *g_expect_help = "中文测试\n"
                             "\n"
                             "  -n, --name <string>        姓名字符串\n"
                             "  -s, --student              是否学生\n"
                             "  -a, --age <int>            年龄（整数）\n"
                             "  --height <number>          身高（米）\n"
                             "\n"
                             "  init                       初始化\n"
                             "  exit                       退出\n";

void cmdp_configure_chinese();
#define __START()                                                                                                      \
    cmdp_configure_chinese();                                                                                          \
    START_CMD();
#define __END()                                                                                                        \
    END_CMD();                                                                                                         \
    cmdp_reset_global_config();

UTEST(chinese, doc)
{
    __START();
    RUN_CMD(&g_command, );
    EXPECT_CMD(CMDP_OK, g_expect_help, "");
    __END();
}

UTEST(chinese, error_unknown_command)
{
    __START();
    RUN_CMD(&g_command, "unknown");
    EXPECT_CMD(CMDP_FAIL, "", "未知命令: unknown.\n");
    __END();
}

UTEST(chinese, error_unknown_short_opt)
{
    __START();
    RUN_CMD(&g_command, "-u");
    EXPECT_CMD(CMDP_FAIL, "", "未知选项: -u.\n");
    __END();
}

UTEST(chinese, error_unknown_long_opt)
{
    __START();
    RUN_CMD(&g_command, "--unknown");
    EXPECT_CMD(CMDP_FAIL, "", "未知选项: --unknown.\n");
    __END();
}

UTEST(chinese, error_require_arg_short)
{
    __START();
    RUN_CMD(&g_command, "-n");
    EXPECT_CMD(CMDP_FAIL, "", "选项-n需要参数.\n");
    __END();
}

UTEST(chinese, error_require_arg_long)
{
    __START();
    RUN_CMD(&g_command, "--name");
    EXPECT_CMD(CMDP_FAIL, "", "选项--name需要参数.\n");
    __END();
}

UTEST(chinese, error_parse_int)
{
    __START();
    RUN_CMD(&g_command, "--age", "1.234");
    EXPECT_CMD(CMDP_FAIL, "", "解析整数失败: 1.234.\n");
    __END();
}

UTEST(chinese, error_parse_float)
{
    __START();
    RUN_CMD(&g_command, "--height", "1.234a");
    EXPECT_CMD(CMDP_FAIL, "", "解析数字失败: 1.234a.\n");
    __END();
}
