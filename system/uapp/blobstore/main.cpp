// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <dirent.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "blobstore-private.h"
#include "fs/vfs.h"

// TODO(smklein): Implement fsck for blobstore

int do_blobstore_mount(int fd, int argc, char** argv) {
    vnode_t* vn = 0;
    if (blobstore::blobstore_mount(&vn, fd) < 0) {
        return -1;
    }
    vfs_rpc_server(vn);
    return 0;
}

int do_blobstore_mkfs(int fd, int argc, char** argv) {
    return blobstore::blobstore_mkfs(fd);
}

struct {
    const char* name;
    int (*func)(int fd, int argc, char**argv);
    const char* help;
} CMDS[] = {
    {"create", do_blobstore_mkfs, "initialize filesystem"},
    {"mkfs", do_blobstore_mkfs, "initialize filesystem"},
    {"mount", do_blobstore_mount, "mount filesystem"},
};

int usage(void) {
    fprintf(stderr,
            "usage: blobstore <command> [ <arg>* ]\n"
            "\n"
            "On Fuchsia, blobstore takes the block device argument by handle.\n"
            "This can make 'blobstore' commands hard to invoke from command line.\n"
            "Try using the [mkfs,fsck,mount,umount] commands instead\n"
            "\n");
    for (unsigned n = 0; n < (sizeof(CMDS) / sizeof(CMDS[0])); n++) {
        fprintf(stderr, "%9s %-10s %s\n", n ? "" : "commands:",
                CMDS[n].name, CMDS[n].help);
    }
    fprintf(stderr, "\n");
    return -1;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return usage();
    }
    char* cmd = argv[1];

    // Block device passed by handle
    int fd = FS_FD_BLOCKDEVICE;

    for (unsigned i = 0; i < sizeof(CMDS) / sizeof(CMDS[0]); i++) {
        if (!strcmp(cmd, CMDS[i].name)) {
            return CMDS[i].func(fd, argc - 3, argv + 3);
        }
    }
    return usage();
}
