
#include "common.h"

UTEST_STATE();
bool g_log_switch = false;

/* 
make test args="--log --filter=*.*"
make memory args="--log --filter=*.*"
 */

int main(int argc, const char *const argv[])
{
    if (argc > 1)
    {
        if (strcmp(argv[1], "--help") == 0)
        {
            printf("Usage: [--log] [--filter=<pattern>]\n\n");
        }
        if (strcmp(argv[1], "--log") == 0)
        {
            g_log_switch = true;
            argc--;
            argv++;
        }
    }
    return utest_main(argc, argv);
}

UTEST(main, Makefile)
{
    // strdup("Hello World");
    // ASSERT_EQ(1, 2);
}
