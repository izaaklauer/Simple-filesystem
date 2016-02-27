/**
 * @file tshell.c
 * @brief The main shell for SFS.
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


#include "SFS.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 2) {
        fprintf(stderr, "%s: missing file operand\n", argv[0]);
        fprintf(stderr, "Usage: %s FILE_NAME\n", argv[0]);
        fprintf(stderr, "Main shell to control SFS.\n\n");
        return EXIT_FAILURE;
    }

    // Mmap the file
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "%s: failed to open file '%s'\n", argv[0], argv[1]);
        return EXIT_FAILURE;
    }
    global_map = mmap(0, MAX_FILESYSTEM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (global_map == MAP_FAILED) {
        close(fd);
        fprintf(stderr, "%s: missing file operand\n", argv[0]);
        perror("Error mmapping the file");
        return EXIT_FAILURE;
    }

    // Unmap the file
    if (msync(global_map, MAX_FILESYSTEM_SIZE, MS_SYNC) == -1) {
        perror("Could not sync the file to disk");
    }
    if (munmap(global_map, MAX_FILESYSTEM_SIZE) == -1) {
        close(fd);
        perror("Error un-mmapping the file");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
