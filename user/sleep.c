#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char* argv[])
{
    if (argc <= 1) {
        printf("usage: %s seconds\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);
    if (n > 0) {
        sleep(n);
    }

    exit(0);
}
