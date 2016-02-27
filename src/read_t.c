/**
 * @file read_t.c
 * @brief Read from a file in SFS.
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

int read_from_block(struct inode *inode, int block_id, int block_offset, void *buf, int count) {
    int max_read_count = BLOCK_SIZE - block_offset;
    int read_count = count>max_read_count? max_read_count : count;

    //Get block ptr
    int *p = get_blk_in_inode(inode, block_id);
    void *ptr = GET_BLK_PTR(*p);
    memcpy(buf, ptr+block_offset, read_count);
    return read_count;
}

int read_t( int inode_number, int offset, void *buf, int count) {
    struct inode *inode_ptr = GET_INODE_PTR(inode_number);
    void *local_buf = buf;
    int read_count = 0;

    int block_id = calc_data_block_id(offset);
    int block_offset = calc_data_block_offset(offset);
    int ret = read_from_block(inode_ptr, block_id, block_offset, local_buf, count);

    while(1) {
        local_buf += ret;
        read_count += ret;
        block_id++;
        if(read_count>=count)
            break;
        //no more offset
        ret = read_from_block(inode_ptr, block_id, 0, local_buf, count-read_count);
    }
    return read_count;
}
