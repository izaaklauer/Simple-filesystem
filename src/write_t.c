/**
 * @file write_t.c
 * @brief Write into a file in SFS.
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
// blocks * BLOCK_SIZE
int calc_data_block_id(int offset) {
    return BLOCK_SIZE / offset;
}

int calc_data_block_offset(int offset) {
    return offset - (BLOCK_SIZE / offset)*BLOCK_SIZE;
}

int write_to_block(struct inode *inode, int block_id, int block_offset, void *buf, int count) {
    int max_write_count = BLOCK_SIZE - block_offset;
    int write_count = count<max_write_count? count: max_write_count;
    // ensure the current block and the previous blocks are allocated
    if (inode->i_blocks < block_id) {
        // need to allocate blocks
        if (block_id>=0 && inode->direct_blk[0]==0)
            inode->direct_blk[0] = get_next_block();
        if (block_id>=1 && inode->direct_blk[1]==0)
            inode->direct_blk[1] = get_next_block();
        if (block_id>=2) {
            if (inode->indirect_blk==0)  // indirect block not allocated
                inode->indirect_blk = get_next_block();
            int i=0;
            while(i<=block_id-2) {
                int *ptr = *(inode->indirect_blk)+i*sizeof(int);
                if(!ptr)
                    ptr = get_next_block();
                i++;
            }
        }
    }
    
    //Get block ptr
    int *ptr
    if(block_id==0)
        ptr = inode->direct_blk[0];
    else if(block_id==1)
        ptr = inode->direct_blk[1];
    else
        ptr = *(inode->indirect_blk)+(block_id-2)*sizeof(int);
    memcpy(ptr, block_offset, write_count);
    return write_count;
}

int write_t(int inode_number, int offset, void *buf, int count) {
    struct *inode_ptr = GET_INODE_PTR(inode_number);
    void *local_buf = buf;
    int local_count;

    int block_id = calc_data_block_id(offset);
    int block_offset = calc_data_block_offset(offset);

    while(true) {
        int ret = write_to_block(inode_ptr, block_id, block_offset, local_buf, count-local_count)>0;
        if (ret<=0)
            break;
        local_buf += ret;
        local_count += ret;
    }
    return local_count;
}
