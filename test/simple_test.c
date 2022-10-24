#include "common.h"

static struct
{
    char *name;
    bool is_student;
    int age;
    double height;
    char *address;
} g_arg = {0};

static void g_before(cmdp_before_param_st *params);
static cmdp_action_t g_process(cmdp_process_param_st *params);

static cmdp_command_st g_command = {
    .doc = "Student struct.\n\n",
    .options =
        (cmdp_option_st[]){
            {'n', "name", "Input Name String", CMDP_TYPE_STRING_PTR, &g_arg.name},
            {'s', "student", "Indicate is student", CMDP_TYPE_BOOL, &g_arg.is_student},
            {'a', "age", "Input Age", CMDP_TYPE_INT4, &g_arg.age},
            {0, "height", "Input Height", CMDP_TYPE_DOUBLE, &g_arg.height, "<HEIGHT>"},
            {'d', NULL, "Input Address", CMDP_TYPE_STRING_PTR, &g_arg.address, "<ADDRESS>"},
            {0},
        },
    .fn_before  = g_before,
    .fn_process = g_process,
};

static char *g_expect_help = "Student struct.\n\n"
                             "  -n, --name <string>        Input Name String\n"
                             "  -s, --student              Indicate is student\n"
                             "  -a, --age <int>            Input Age\n"
                             "  --height <HEIGHT>          Input Height\n"
                             "  -d <ADDRESS>               Input Address\n";

static void g_before(cmdp_before_param_st *params)
{
    memset(&g_arg, 0, sizeof(g_arg));
}
static cmdp_action_t g_process(cmdp_process_param_st *params)
{
    LOG_INFO("name: %s\n", g_arg.name);
    LOG_INFO("is_student: %d\n", g_arg.is_student);
    LOG_INFO("age: %d\n", g_arg.age);
    LOG_INFO("height: %f\n", g_arg.height);
    LOG_INFO("address: %s\n", g_arg.address);

    if (params->opts != 0)
    {
        return CMDP_ACT_OVER;
    }
    if (params->argc == 0)
    {
        return CMDP_ACT_SHOW_HELP;
    }
    return CMDP_ACT_OVER;
}

// normal input

UTEST(simple, input_space)
{
    START_CMD();
    RUN_CMD(&g_command, "--name", "hello", "-s", "-a", "18", "--height", "1.8", "-d", "beijing");
    EXPECT_STREQ("hello", g_arg.name);
    EXPECT_EQ(true, g_arg.is_student);
    EXPECT_EQ(18, g_arg.age);
    EXPECT_EQ(1.8, g_arg.height);
    EXPECT_STREQ("beijing", g_arg.address);
    EXPECT_CMD(CMDP_OK, "", "");
    END_CMD();
}

UTEST(simple, input_equal_symbol)
{
    START_CMD();
    RUN_CMD(&g_command, "--name=hello", "-s", "-a", "18", "--height=1.8", "-d", "beijing");
    EXPECT_STREQ("hello", g_arg.name);
    EXPECT_EQ(true, g_arg.is_student);
    EXPECT_EQ(18, g_arg.age);
    EXPECT_EQ(1.8, g_arg.height);
    EXPECT_STREQ("beijing", g_arg.address);
    EXPECT_CMD(CMDP_OK, "", "");
    END_CMD();
}

UTEST(simple, continuous_short_opt)
{
    START_CMD();
    RUN_CMD(&g_command, "--name", "hello", "-sa", "18");
    EXPECT_STREQ("hello", g_arg.name);
    EXPECT_EQ(true, g_arg.is_student);
    EXPECT_EQ(18, g_arg.age);
    EXPECT_EQ(0.0, g_arg.height);
    EXPECT_EQ(NULL, g_arg.address);
    EXPECT_CMD(CMDP_OK, "", "");
    END_CMD();
}

UTEST(simple, continuous_short_opt_inverse)
{
    START_CMD();
    RUN_CMD(&g_command, "--name", "hello", "-as", "18");
    EXPECT_STREQ("hello", g_arg.name);
    EXPECT_EQ(true, g_arg.is_student);
    EXPECT_EQ(18, g_arg.age);
    EXPECT_EQ(0.0, g_arg.height);
    EXPECT_EQ(NULL, g_arg.address);
    EXPECT_CMD(CMDP_OK, "", "");
    END_CMD();
}

UTEST(simple, input_x2)
{
    START_CMD();

    RUN_CMD(&g_command, "--name", "hello", "--student", "-a", "18", "--height", "1.8", "-d", "beijing");
    EXPECT_STREQ("hello", g_arg.name);
    EXPECT_EQ(true, g_arg.is_student);
    EXPECT_EQ(18, g_arg.age);
    EXPECT_EQ(1.8, g_arg.height);
    EXPECT_STREQ("beijing", g_arg.address);
    EXPECT_CMD(CMDP_OK, "", "");

    RUN_CMD(&g_command, "--height", "1.7", "-d", "shanghai");
    EXPECT_EQ(NULL, g_arg.name);
    EXPECT_EQ(false, g_arg.is_student);
    EXPECT_EQ(0, g_arg.age);
    EXPECT_EQ(1.7, g_arg.height);
    EXPECT_STREQ("shanghai", g_arg.address);
    EXPECT_CMD(CMDP_OK, "", "");

    END_CMD();
}

UTEST(simple, dash_name)
{
    START_CMD();
    RUN_CMD(&g_command, "--name", "--hello");
    EXPECT_STREQ("--hello", g_arg.name);
    EXPECT_CMD(CMDP_OK, "", "");
    END_CMD();
}

// error

UTEST(simple, unknown_short_option)
{
    START_CMD();
    RUN_CMD(&g_command, "-u");
    EXPECT_CMD(CMDP_FAIL, "", "Unknown option -u.\n");
    END_CMD();
}

UTEST(simple, unknown_long_option)
{
    START_CMD();
    RUN_CMD(&g_command, "--unknown");
    EXPECT_CMD(CMDP_FAIL, "", "Unknown option --unknown.\n");
    END_CMD();
}

UTEST(simple, require_short_arg)
{
    START_CMD();
    RUN_CMD(&g_command, "-a");
    EXPECT_CMD(CMDP_FAIL, "", "Option -a require args.\n");
    END_CMD();
}

UTEST(simple, require_short_arg_cont)
{
    START_CMD();
    RUN_CMD(&g_command, "-sa");
    EXPECT_CMD(CMDP_FAIL, "", "Option -a require args.\n");
    END_CMD();
}

UTEST(simple, require_long_arg)
{
    START_CMD();
    RUN_CMD(&g_command, "--name");
    EXPECT_CMD(CMDP_FAIL, "", "Option --name require args.\n");
    END_CMD();
}

UTEST(simple, parse_not_int)
{
    START_CMD();
    RUN_CMD(&g_command, "--age", "123abc");
    EXPECT_CMD(CMDP_FAIL, "", "Parse int failed: 123abc.\n");
    END_CMD();
}

UTEST(simple, parse_not_float)
{
    START_CMD();
    RUN_CMD(&g_command, "--height", "123.abc");
    EXPECT_CMD(CMDP_FAIL, "", "Parse float failed: 123.abc.\n");
    END_CMD();
}

UTEST(simple, parse_repeat_option_short)
{
    START_CMD();
    RUN_CMD(&g_command, "-s", "-s");
    EXPECT_CMD(CMDP_FAIL, "", "Option -s repeat.\n");
    END_CMD();
}

UTEST(simple, parse_repeat_option_long)
{
    START_CMD();
    RUN_CMD(&g_command, "--student", "--student");
    EXPECT_CMD(CMDP_FAIL, "", "Option --student repeat.\n");
    END_CMD();
}

// help doc

UTEST(simple, help__void)
{
    START_CMD();
    RUN_CMD(&g_command, );
    EXPECT_CMD(CMDP_OK, g_expect_help, "");
    END_CMD();
}

UTEST(simple, help__h)
{
    START_CMD();
    RUN_CMD(&g_command, "-h");
    EXPECT_CMD(CMDP_OK, g_expect_help, "");
    END_CMD();
}

UTEST(simple, help__help)
{
    START_CMD();
    RUN_CMD(&g_command, "--help");
    EXPECT_CMD(CMDP_OK, g_expect_help, "");
    END_CMD();
}
