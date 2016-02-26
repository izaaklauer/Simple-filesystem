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
int read_t( int inode_number, int offset, void *buf, int count) {
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
