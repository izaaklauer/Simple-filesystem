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

int open_t(const char *pathname, int flags) {
    if (flags==2) {      // find existing file
        return find_inode(pathname);
    } else {            // create new file/dir(override)
        // get the parent_path
        char parent_path[65535];
        path_get_path(parent_path, pathname, sizeof(parent_path));
        // get file/folder name
        char filename[65535];
        path_get_filename(filename, pathname, sizeof(parent_path));
        // add entry
        int parent_inode = find_inode(parent_path);
        if (flags==0) {
            // 0 = create regular file (override)
            create_file(parent_inode, filename);
        } else if (flags==1) {
            // 1 = create directory (override)
            create_directory(parent_inode, filename);
        }
    }
}
