#include <stdio.h>
#include <string.h>

#include "ping.h"

static void print_usage(void)
{
    printf("usage:\n");
    printf(" ./cnet --help\n");
    printf(" ./cnet ping <address>\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        print_usage();
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0) {
        print_usage();
        return 0;
    }

    if (strcmp(argv[1], "ping") == 0) {
        return tool_ping(argc - 1, argv + 1);
    }

    fprintf(stderr, "unknown command: %s\n", argv[1]);
    print_usage();
    return 1;
}
