/**
 * @file SFS.h
 * @brief A simple filesystem (SFS)
 * @author Star Poon <star.poon@connect.polyu.hk>
 * @version 1.0
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

//Unit is Byte
#define SB_OFFSET                       512
#define INODE_OFFSET                   4096
#define DATA_OFFSET                   10240
#define MAX_INODE                       100
#define MAX_DATA_BLK                  25600
#define BLOCK_SIZE                     4096
#define MAX_NESTING_DIR                  10
#define MAX_COMMAND_LENGTH               50
#define MAX_FILESYSTEM_SIZE   110*1024*1024

struct superblock {
    int inode_offset;
    int data_offset;
    int max_inode;
    int max_data_blk;
    int next_available_inode;
    int next_available_blk;
    int blk_size;
};

struct inode {
    int i_number;
    time_t i_mtime;
    int i_type;
    int i_size;
    int i_blocks;
    int direct_blk[2];
    int indirect_blk;
    int file_num;
};

typedef struct dir_mapping {
    char dir[10];
    int inode_number;
} DIR_NODE;

/**
 * @brief Open and possibly create a file.
 * @details Returns the corresponding inode number of the file.
 *          The returned inode number will be used in subsequent
 *          functions in read_t() and write_t().
 * @param[in] pathname An absolute path name for a file
 * @param[in] flags 0/1 means a new regular file/directory will 
 *            be created (if one file with the same name exists, 
 *            the new file will replace the old file;
 *            2 means that target is an existing file.
 * @returns The corresponding inode number of the file.
 * @retval >=0 The inode number of the file.
 * @retval -1 Error occured.
 */
int open_t(const char *pathname, int flags);

/**
 * @brief Read bytes from the file.
 * @details Attempts to read up to count bytes from the file
 *          starting at offset (with the inode number inode_number)
 *          into the buffer starting at buf. It commences at the
 *          file offset specified by offset.
 * @retval >0 The number of bytes read.
 * @retval 0 If offset is at or past the end of the file.
 * @retval -1 Error occured.
 */
int read_t(int inode_number, int offset, void *buf, int count);
