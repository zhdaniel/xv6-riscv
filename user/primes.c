/***************************************************************************
 *
 * Copyright (c) 2020 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

/**
 * @file prime.c
 * @author zhdaniel(com@baidu.com)
 * @date 2020/01/17 15:01:58
 * @brief
 *
 **/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void filter(int fd_left) __attribute__((noreturn));

void filter(int fd_left) {
    int base;
    if (read(fd_left, &base, sizeof(base)) <= 0) {
        close(fd_left);
        exit(0);
    }

    printf("prime %d\n", base);

    // fd[0] read
    // fd[1] write
    int fildes[2];
    if (pipe(fildes) < 0) {
        printf("failed to call pipe\n");
        exit(-1);
    }
    int fd_right = fildes[1];
    int s = -1;

    int pid = fork();
    switch (pid) {
        case 0:
            close(fd_right);
            filter(fildes[0]);
            break;
        case -1:
            break;
        default:
            close(fildes[0]);
            while (read(fd_left, &s, sizeof(s)) > 0) {
                // printf("recv %d\n", s);
                if (s % base != 0) {
                    // printf("send %d\n", s);
                    write(fd_right, &s, sizeof(s));
                }
            }
            close(fd_left);
            close(fd_right);
    }

    while (wait(0) != -1) {
    }

    exit(0);
}

int main(int argc, char* argv[]) {
    int fildes[2];
    if (pipe(fildes) < 0) {
        printf("failed to call pipe\n");
        return -1;
    }

    int min = 2;
    int max = 35;

    int pid = fork();
    switch (pid) {
        case 0:
            close(fildes[1]);
            filter(fildes[0]);
            break;
        case -1:
            break;
        default:
            close(fildes[0]);
            for (int i = min; i <= max; i++) {
                // printf("send %d\n", i);
                write(fildes[1], &i, sizeof(i));
            }
            close(fildes[1]);
    }

    while (wait(0) != -1) {
    }

    exit(0);
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
