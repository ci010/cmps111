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

    fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0644);

    if(fd == 0) {
        printf("Cannot open file %s\n", argv[1]);
        return -1;
    }

    N = atoi(argv[2]);

    /**
     * init the super block
     */
    int k = N / 1024;
    if (k == 0)
        k = 1;

    struct fat_superblock super = {
        .fat_magic = 0xCAFED00D,
        .fat_nblocks_total = N,
        .fat_nblocks_file_table = k,
        .fat_block_size = FAT_BLOCK_SIZE,
        .fat_block_start = k + 1
    };

    write(fd, super, 20);

    int32_t file_alloc_table[1024];

    /**
     * In file alloc table, first is super block
     * then k block for table, marked as -1
     */
    memset(file_alloc_table, -1, k + 1);

    /**
     * Initialize the . and .. dir
     */
    time_t t = time(0);

    fat_dir dir_current = {
        .fat_dir_atime = t,
        .fat_dir_ctime = t,
        .fat_dir_mtime = t,
        .fat_dir_file_len = 128,
        .fat_dir_start_block = k + 1,
        .fat_dir_flags = 1,
        .fat_dir_unsed = 0,
    };
    fat_dir dir_parent = {
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

    write(fd, dir_current, 64);
    write(fd, dir_parent, 64);

    close(fd);
}