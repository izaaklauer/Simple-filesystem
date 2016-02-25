/**
 * @file SFS.c
 * @brief A simple filesystem (SFS)
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


struct inode* GET_INODE_PTR(int inode_number) {
    return (struct inode*)global_sb->inode_offset+inode_number*(sizeof(struct inode));
}

int load_SFS(const char *hd_file) {
    // open and mmap the harddisk file
    int fd = open(hd_file, O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "Failed to load harddisk file '%s'.\n", hd_file);
        fprintf(stderr, "%s\n", strerror(errno));
        return -1;
    }
    char *map = mmap(0, MAX_FILESYSTEM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        close(fd);
        fprintf(stderr, "Failed to map harddisk file '%s'.\n", hd_file);
        perror("Error mmapping the file");
        return -1;
    }
    // initialize the global variables
    strncpy(global_hd_file, hd_file, sizeof(global_hd_file));
    global_fd = fd;
    global_map = map;
    return 0;
}

int get_next_inode() {
    char *p = global_superblock.next_available_inode;
    global_superblock.next_available_inode++;
    return *p;
}

int create_directory(int parent_inode, const char *dir_name) {
    // allocate inode
    int child_inode = get_next_inode();

    // initialize the inode structure
    struct inode *inode_ptr = GET_INODE_PTR(inode_number);
    inode_ptr->i_number = child_inode;
    time(&inode_ptr->i_mtime);
    inode_ptr->i_type = 0;
    inode_ptr->i_size = 0;
    inode_ptr->i_blocks = -1;
    inode_ptr->direct_blk[0] = 0;
    inode_ptr->direct_blk[1] = 0;
    inode_ptr->indirect_blk = 0;
    inode_ptr->file_num = 0;

    // add item into parent_inode
    add_entry(parent_inode, dir_name, child_inode);
    // add . and .. into the child_inode
    add_entry(child_inode, ".", child_inode);
    add_entry(child_inode, "..", parent_inode);
    return child_inode;
}

int create_file(int parent_inode, const char *filename) {
    int child_inode = get_next_inode();

    // initialize the inode structure
    struct *inode_ptr = GET_INODE_PTR(inode_number);
    inode_ptr->i_number = inode_number;
    time(&inode_ptr->i_mtime);
    inode_ptr->i_type = 1;
    inode_ptr->i_size = 0;
    inode_ptr->i_blocks = -1;
    inode_ptr->direct_blk[0] = 0;
    inode_ptr->direct_blk[1] = 0;
    inode_ptr->indirect_blk = 0;
    inode_ptr->file_num = 0;

    add_entry(parent_inode, filename, child_inode);
    return child_inode;
}

/**
 * @brief Find file or directory in a directory
 * @retval -1 File not found
 * @retval -2 Not a directory
 */
int _find_inode(int inode, char *dir_name) {
    struct *inode_ptr = GET_INODE_PTR(inode);
    // check if the inode is a directory
    if(inode_ptr.i_type != 0) {
        return -2;      // error: this is not a directory
    }
    // loop each dir_mapping record and compare the name
    int i = 0;
    do {
        struct dir_mapping map;
        int ret = read_t(inode, i, &map, sizeof(struct dir_mapping));
        if (ret<=0) { // nothing to read
            return -1;  // error: not found
        }
        if (strcmp(map.dir, dir_name)==0) {
            return map.inode_number;    // found
        }
        i++;
    } while (true);
}

int find_inode(const char *path) {
    // root dir
    int inode = 0;
    char inode_name[10] = "/";
    char *pch;
    pch = strtok(path, "/");
    while (pch != NULL) {
        printf("DEBUG: PCH = %s\n", pch);
        // find the child item in the current directory *inode
        inode = _find_inode(inode, path_part);
        if (inode==-1) {
            perror("Error: cannot find '%s' in '%s'.\n", pch, inode_name);
            return -1;
        } else if (inode==-2) {
            perror("Error: '%s' is not a directory.\n", pch, inode_name);
            return -1;
        }
        pch = strtok(NULL, "/\\");
    }
    return inode;
}

void add_entry(int dir_inode, const char *entry_name, int entry_inode) {
}

/*****************************************************
 * String helper functions
 ****************************************************/
char* trim_path(char *str) {
    char *end = str + strlen(str) - 1;
    if (*end == '/')
        *end = '\0';
    return str;
}

char* path_get_path(char *dst, const char *str, int len) {
    strncpy(dst, str, len);
    trim_path(dst);
    char *ptr = strrchr(dst, '/');
    if (!ptr) {
        perror("Error: Invalid path '%s'\n", dst);
    }
    if (ptr!=dst)
        *ptr = '\0';
    return *dst;
}

char* path_get_filename(char *dst, const char *str, int len) {
    strncpy(dst, str, len);
    trim_path(dst);
    char *ptr = strrchr(dst, '/');
    if (!ptr) {
        perror("Error: Invalid path '%s'\n", dst);
    }
    strncpy(dst, ptr+1, len);
    return *ptr;
}
