/***************************************************************************
 *
 * Copyright (c) 2020 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

/**
 * @file user/find.c
 * @author zhdaniel(com@baidu.com)
 * @date 2020/01/18 17:20:48
 * @brief
 *
 **/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(const char* path, const char* name) {
    int fd;
    struct stat st;
    struct dirent de;
    char buf[512], *p;

    printf("path: %s\n", path);
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);

        return;
    }

    switch (st.type) {
        case T_FILE:
            fprintf(2, "find: path should be a directory\n");
            break;
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                fprintf(2, "find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof de) {
                printf("de.name: %s\n", de.name);
                if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                    continue;
                }
                if (strlen(name) == strlen(de.name) &&
                    strcmp(name, de.name) == 0) {
                    printf("%s\n", de.name);
                }
            }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        exit(0);
    }
    char* path = argv[1];
    char* name = argv[2];
    find(path, name);
    exit(0);
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
