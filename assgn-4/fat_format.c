#include "fat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define FAT_BLOCK_SIZE 4096

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Usage: fat_format filename blocks\n");
        return (-1);
    }
    int fd;
    int N;
    int k;
    /**
     * 4KB for a block 
     */
    uint8_t flush_area[FAT_BLOCK_SIZE];

    fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0644);

    if (fd == 0) {
        printf("Cannot open file %s\n", argv[1]);
        return -1;
    }

    N = atoi(argv[2]);

    /**
     * init the super block
     */
    k = N / 1024;
    if (k == 0)
        k = 1;

    struct fat_superblock super = {
        .fat_magic = 0xCAFED00D,
        .fat_nblocks_total = N,
        .fat_nblocks_file_table = k,
        .fat_block_size = FAT_BLOCK_SIZE,
        .fat_block_start = k + 1
    };
    memset(flush_area, 0, FAT_BLOCK_SIZE);
    memcpy(flush_area, super, 20);
    write(fd, flush_area, FAT_BLOCK_SIZE);

    /**
     * init FAT;
     * in file alloc table, first is super block
     * then k block for table, marked as -1
     */
    u_int32_t* fat_flush_area = (u_int32_t*)flush_area;
    int remain = k;
    for (int i = 0; i < k + 1; i++) {
        memset(flush_area, 0, FAT_BLOCK_SIZE);
        /**
         * use remain to keep track how many FAT pointer we need to set
         */
        if (remain > 0) {
            int count = remain % 1024;
            if (count == 0) count = 1024;
            for (int j = 0; j < count; ++i) fat_flush_area[j] = -1;
            remain -= 1024;
        } 
        write(fd, flush_area, FAT_BLOCK_SIZE);
    }

    /**
     * Initialize the . and .. dir
     */
    time_t t = time(0);

    struct fat_entry dir_current = {
        .fat_dir_atime = t,
        .fat_dir_ctime = t,
        .fat_dir_mtime = t,
        .fat_dir_file_len = 128,
        .fat_dir_start_block = k + 1,
        .fat_dir_flags = 1,
        .fat_dir_unsed = 0,
    };
    struct fat_entry dir_parent = {
        .fat_dir_atime = t,
        .fat_dir_ctime = t,
        .fat_dir_mtime = t,
        .fat_dir_file_len = 128,
        .fat_dir_start_block = k + 1,
        .fat_dir_flags = 1,
        .fat_dir_unsed = 0,
    };
    strcpy(dir_current.fat_dir_name, '.');
    strcpy(dir_parent.fat_dir_name, '..');

    memset(flush_area, 0, FAT_BLOCK_SIZE);
    memcpy(flush_area, dir_current, 64);
    memcpy(flush_area + 64, dir_parent, 64);
    
    write(fd, flush_area, FAT_BLOCK_SIZE);

    memset(flush_area, 0, FAT_BLOCK_SIZE);
    for (int i = 0; i < N - 1; ++i)
        write(fd, flush_area, FAT_BLOCK_SIZE);

    close(fd);
}