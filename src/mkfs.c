/**
 * @file mkfs_t.c
 * @brief Create and format a simple filesystem (SFS)
 * @author Star Poon <star.poon@connect.polyu.hk>
 * @copyright 2016
 *
 * @section LICENSE
 * Copyright (C) 2015 Star Poon
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "SFS.h"

int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 2) {
        fprintf(stderr, "%s: missing file operand\n", argv[0]);
        fprintf(stderr, "Usage: mkfs_t FILE_NAME\n");
        fprintf(stderr, "Create an SFX filesystem on a file with the name FILE_NAME.\n");
        return EXIT_FAILURE;
    }

    // Use dd to create an empty file
    char CMD[512];
    sprintf(CMD, "dd if=/dev/zero of=%s bs=1M count=%d", argv[1], MAX_FILESYSTEM_SIZE/1024/1024);
    int ret = system(CMD);
    if(ret) {
        fprintf(stderr, "Return code: %d\n", ret);
        return EXIT_FAILURE;
    }

    // Mmap the file
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "%s: failed to open file '%s'\n", argv[0], argv[1]);
        return EXIT_FAILURE;
    }
    char *map = mmap(0, MAX_FILESYSTEM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        close(fd);
        fprintf(stderr, "%s: missing file operand\n", argv[0]);
        perror("Error mmapping the file");
        return EXIT_FAILURE;
    }

    // Init superblock
    struct superblock sb = {
        .inode_offset = INODE_OFFSET,
        .data_offset = DATA_OFFSET,
        .max_inode = MAX_INODE,
        .max_data_blk = MAX_DATA_BLK,
        .next_available_inode = 0,
        .next_available_blk = 0,
        .blk_size = BLOCK_SIZE,
    };
    memcpy(map+SB_OFFSET, &sb, sizeof(sb));

    // Init root directory
    int root_inode = get_next_inode();
    add_entry(root_inode, ".", 0);

    // Unmap the file
    if (msync(map, MAX_FILESYSTEM_SIZE, MS_SYNC) == -1) {
        perror("Could not sync the file to disk");
    }
    if (munmap(map, MAX_FILESYSTEM_SIZE) == -1) {
        close(fd);
        perror("Error un-mmapping the file");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
