#include "common.h"

/* 
make memory args="--filter=chinese.*"
 */

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
        (cmdp_command_st[]){
            {.doc = "\n"},
            {"init", "初始化"},
            {"exit", "退出"},
            {0},
        },
};

static char *g_expect_help = "中文测试\n"
                             "\n"
                             "  -n, --name=<string>        姓名字符串\n"
                             "  -s, --student              是否学生\n"
                             "  -a, --age=<int>            年龄（整数）\n"
                             "  --height=<number>          身高（米）\n"
                             "\n"
                             "  init                       初始化\n"
                             "  exit                       退出\n";

void cmdp_configure_chinese();

UTEST(chinese, doc)
{
    cmdp_configure_chinese();
    START_CMD();
    RUN_CMD(&g_command, );
    EXPECT_STREQ(g_expect_help, r_out);
    EXPECT_STREQ("", r_err);
    EXPECT_EQ(0, r_code);
    END_CMD();
    cmdp_reset_global_config();
}

UTEST(chinese, error_unknown_command)
{
    cmdp_configure_chinese();
    START_CMD();
    RUN_CMD(&g_command, "unknown");
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("未知命令: unknown.\n", r_err);
    EXPECT_NE(0, r_code);
    END_CMD();
    cmdp_reset_global_config();
}

UTEST(chinese, error_unknown_short_opt)
{
    cmdp_configure_chinese();
    START_CMD();
    RUN_CMD(&g_command, "-u");
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("未知选项: -u.\n", r_err);
    EXPECT_NE(0, r_code);
    END_CMD();
    cmdp_reset_global_config();
}

UTEST(chinese, error_unknown_long_opt)
{
    cmdp_configure_chinese();
    START_CMD();
    RUN_CMD(&g_command, "--unknown");
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("未知选项: --unknown.\n", r_err);
    EXPECT_NE(0, r_code);
    END_CMD();
    cmdp_reset_global_config();
}

UTEST(chinese, error_require_arg_short)
{
    cmdp_configure_chinese();
    START_CMD();
    RUN_CMD(&g_command, "-n");
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("选项-n需要参数.\n", r_err);
    EXPECT_NE(0, r_code);
    END_CMD();
    cmdp_reset_global_config();
}

UTEST(chinese, error_require_arg_long)
{
    cmdp_configure_chinese();
    START_CMD();
    RUN_CMD(&g_command, "--name");
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("选项--name需要参数.\n", r_err);
    EXPECT_NE(0, r_code);
    END_CMD();
    cmdp_reset_global_config();
}

UTEST(chinese, error_parse_int)
{
    cmdp_configure_chinese();
    START_CMD();
    RUN_CMD(&g_command, "--age", "1.234");
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("解析整数失败: 1.234.\n", r_err);
    EXPECT_NE(0, r_code);
    END_CMD();
    cmdp_reset_global_config();
}

UTEST(chinese, error_parse_float)
{
    cmdp_configure_chinese();
    START_CMD();
    RUN_CMD(&g_command, "--height", "1.234a");
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("解析数字失败: 1.234a.\n", r_err);
    EXPECT_NE(0, r_code);
    END_CMD();
    cmdp_reset_global_config();
}
