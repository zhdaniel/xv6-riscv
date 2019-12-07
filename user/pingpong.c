/***************************************************************************
 * 
 * Copyright (c) 2019 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file user/pingpong.c
 * @author zhdaniel(com@baidu.com)
 * @date 2019/12/04 20:17:20
 * @brief 
 *  
 **/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char* argv[])
{
    // fd[0]: read
    // fd[1]: write
    int parent_fd[2];
    int child_fd[2];
    if (0 != pipe(parent_fd)) {
        printf("failed to call pipe\n");
        exit(1);
    }
    if (0 != pipe(child_fd)) {
        printf("failed to call pipe\n");
        exit(1);
    }

    char r = '\r';
    char n = '\n';
    int  c = -1;
    char buf;

    int pid = fork();
    switch (pid) {
        case 0:
            close(child_fd[0]);
            int child_pid = getpid();

            c = read(parent_fd[0], &buf, sizeof(buf));
            if (c == 1 && r == buf) {
                printf("%d: receiving ping\n", child_pid);
                write(child_fd[1], &n, sizeof(n));
            }
            break;
        case -1:
            printf("failed to call fork\n");
            exit(1);
            break;
        default:
            close(parent_fd[0]);
            int parent_pid = getpid();

            c = write(parent_fd[1], &r, sizeof(r));
            if (c < 1) {
                printf("failed to call write\n");
                exit(1);
            }
            c = read(child_fd[0], &buf, sizeof(buf));
            if (c == 1 && n == buf) {
                printf("%d: receiving pong\n", parent_pid);
            }
            wait(0);
            break;
    }
    exit(0);
}


















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
