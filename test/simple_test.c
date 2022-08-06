#include "common.h"

/* 
make memory args="--filter=simple.*"
 */

static struct
{
    char *name;
    bool is_student;
    int age;
    double height;
    char *address;
} g_arg = {0}, _g_arg_zero = {0};

static void g_before(int argc, char **argv, cmdp_command_st *self);
static cmdp_action_t g_action(int argc, char **argv);

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
    .fn_before = g_before,
    .fn_action = g_action,
};

static char *g_expect_help = "Student struct.\n\n"
                             "  -n, --name=<string>        Input Name String\n"
                             "  -s, --student              Indicate is student\n"
                             "  -a, --age=<int>            Input Age\n"
                             "  --height=<HEIGHT>          Input Height\n"
                             "  -d <ADDRESS>               Input Address\n";

static void g_before(int argc, char **argv, cmdp_command_st *self)
{
    memset(&g_arg, 0, sizeof(g_arg));
}
static cmdp_action_t g_action(int argc, char **argv)
{
    LOG_INFO("name: %s\n", g_arg.name);
    LOG_INFO("is_student: %d\n", g_arg.is_student);
    LOG_INFO("age: %d\n", g_arg.age);
    LOG_INFO("height: %f\n", g_arg.height);
    LOG_INFO("address: %s\n", g_arg.address);

    if (memcmp(&g_arg, &_g_arg_zero, sizeof(_g_arg_zero)) != 0)
    {
        return CMDP_ACT_OVER;
    }
    if (argc == 0)
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
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("", r_err);
    EXPECT_EQ(0, r_code);
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
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("", r_err);
    EXPECT_EQ(0, r_code);
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
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("", r_err);
    EXPECT_EQ(0, r_code);
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
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("", r_err);
    EXPECT_EQ(0, r_code);
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
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("", r_err);
    EXPECT_EQ(0, r_code);

    RUN_CMD(&g_command, "--height", "1.7", "-d", "shanghai");
    EXPECT_EQ(NULL, g_arg.name);
    EXPECT_EQ(false, g_arg.is_student);
    EXPECT_EQ(0, g_arg.age);
    EXPECT_EQ(1.7, g_arg.height);
    EXPECT_STREQ("shanghai", g_arg.address);
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("", r_err);
    EXPECT_EQ(0, r_code);

    END_CMD();
}

UTEST(simple, dash_name)
{
    START_CMD();
    RUN_CMD(&g_command, "--name", "--hello");
    EXPECT_STREQ("--hello", g_arg.name);
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("", r_err);
    EXPECT_EQ(0, r_code);
    END_CMD();
}

// error

UTEST(simple, unknown_short_option)
{
    START_CMD();
    RUN_CMD(&g_command, "-u");
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("Unknown option -u.\n", r_err);
    EXPECT_NE(0, r_code);
    END_CMD();
}

UTEST(simple, unknown_long_option)
{
    START_CMD();
    RUN_CMD(&g_command, "--unknown");
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("Unknown option --unknown.\n", r_err);
    EXPECT_NE(0, r_code);
    END_CMD();
}

UTEST(simple, require_short_arg)
{
    START_CMD();
    RUN_CMD(&g_command, "-a");
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("Option -a require args.\n", r_err);
    EXPECT_NE(0, r_code);
    END_CMD();
}

UTEST(simple, require_short_arg_cont)
{
    START_CMD();
    RUN_CMD(&g_command, "-sa");
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("Option -a require args.\n", r_err);
    EXPECT_NE(0, r_code);
    END_CMD();
}

UTEST(simple, require_long_arg)
{
    START_CMD();
    RUN_CMD(&g_command, "--name");
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("Option --name require args.\n", r_err);
    EXPECT_NE(0, r_code);
    END_CMD();
}

UTEST(simple, parse_not_int)
{
    START_CMD();
    RUN_CMD(&g_command, "--age", "123abc");
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("Parse int failed: 123abc.\n", r_err);
    EXPECT_NE(0, r_code);
    END_CMD();
}

UTEST(simple, parse_not_float)
{
    START_CMD();
    RUN_CMD(&g_command, "--height", "123.abc");
    EXPECT_STREQ("", r_out);
    EXPECT_STREQ("Parse float failed: 123.abc.\n", r_err);
    EXPECT_NE(0, r_code);
    END_CMD();
}

// help doc

UTEST(simple, help__void)
{
    START_CMD();
    RUN_CMD(&g_command, );
    EXPECT_STREQ(g_expect_help, r_out);
    EXPECT_STREQ("", r_err);
    EXPECT_EQ(0, r_code);
    END_CMD();
}

UTEST(simple, help__h)
{
    START_CMD();
    RUN_CMD(&g_command, "-h");
    EXPECT_STREQ(g_expect_help, r_out);
    EXPECT_STREQ("", r_err);
    EXPECT_EQ(0, r_code);
    END_CMD();
}

UTEST(simple, help__help)
{
    START_CMD();
    RUN_CMD(&g_command, "--help");
    EXPECT_STREQ(g_expect_help, r_out);
    EXPECT_STREQ("", r_err);
    EXPECT_EQ(0, r_code);
    END_CMD();
}
