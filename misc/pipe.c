/***************************************************************************
 * 
 * Copyright (c) 2020 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file pipe.c
 * @author zhdaniel(com@baidu.com)
 * @date 2020/01/17 02:52:45
 * @brief 
 *  
 **/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    int fildes[2];
    if (0 > pipe(fildes)) {
        printf("failed to call pipe\n");
        exit(1);
    }

    pid_t pid = fork();
    switch (pid) {
        case 0:
            close(fildes[1]);
            while (1) {
                int i;
                printf("blocking...\n");
                int n = read(fildes[0], &i, sizeof(i));
                printf("recv %d bytes: %02d\n", n, i);
                if (n < 4) { printf("eof\n"); break; }
            }
            exit(0);
        case -1:
            exit(-1);
        default:
            close(fildes[0]);
            for (int i = 2; i < 36; i++) {
                int n = write(fildes[1], &i, sizeof(i));
                printf("send %d bytes: %02d\n", n, i);
            }
            printf("close\n");
            close(fildes[1]);
            break;
    }

    printf("waiting\n");
    int status;
    wait(&status);
    printf("done\n");
}




















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
