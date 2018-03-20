#ifndef FAT_FORMAT_H
#define FAT_FORMAT_H

#include "fat.h"

void fat_format(int fd, int N)
{
    int k;
    /**
     * 4KB for a block 
     */
    uint8_t flush_area[FAT_BLOCK_SIZE];

    /**
     * init the super block
     */
    k = N / 1024;
    if (k % 1024 == 0)
        k += 1;

    printf("Generate img for %d block, %d FAT block\n", N, k);

    struct fat_superblock super = {
        .fat_magic = 0xCAFED00D,
        .fat_super_blocks = N,
        .fat_super_fat_blocks = k,
        .fat_super_block_size = FAT_BLOCK_SIZE,
        .fat_super_block_start = k + 1
    };
    printf("Write super block\n");
    printf("@ file offset %ld\n", lseek(fd, 0, SEEK_CUR));

    memset(flush_area, 0, FAT_BLOCK_SIZE);
    memcpy(flush_area, &super, 20);
    write(fd, flush_area, FAT_BLOCK_SIZE);

    /**
     * init FAT;
     * in file alloc table, first is super block
     * then k block for table, marked as -1
     */
    u_int32_t* fat_flush_area = (u_int32_t*)flush_area;
    int remain = k + 1;
    printf("Write FAT\n");
    for (int i = 0; i < k; i++) {
        printf("Write %d FAT block\n", i);
        printf("@ file offset %ld\n", lseek(fd, 0, SEEK_CUR));
        memset(flush_area, 0, FAT_BLOCK_SIZE);
        /**
         * use remain to keep track how many FAT pointer we need to set
         */
        if (remain > 0) {
            int count = remain % 1024;
            printf("Invalidating the first k (%d) bit\n", count);
            if (count == 0)
                count = 1024;
            for (int j = 0; j < count; ++j)
                fat_flush_area[j] = -1;
            if (count < 1024) {
                fat_flush_area[count] = -2;
            }
            // debug start
            // fat_flush_area[k + 2] = -2;
            // fat_flush_area[k + 3] = -2;
            // debug end

            remain -= 1024;
        }
        write(fd, flush_area, FAT_BLOCK_SIZE);
    }

    /**
     * Initialize the . and .. dir
     */
    time_t t = time(0);

    struct fat_entry dir_current = {
        .fat_entry_atime = t,
        .fat_entry_ctime = t,
        .fat_entry_mtime = t,
        .fat_entry_file_len = 0,
        .fat_entry_start_block = k + 1,
        .fat_entry_flags = 1,
        .fat_entry_unsed = 0,
    };
    struct fat_entry dir_parent = {
        .fat_entry_atime = t,
        .fat_entry_ctime = t,
        .fat_entry_mtime = t,
        .fat_entry_file_len = 0,
        .fat_entry_start_block = k + 1,
        .fat_entry_flags = 1,
        .fat_entry_unsed = 0,
    };
    struct fat_entry dir_hello = {
        .fat_entry_atime = t,
        .fat_entry_ctime = t,
        .fat_entry_mtime = t,
        .fat_entry_file_len = 0,
        .fat_entry_start_block = k + 2,
        .fat_entry_flags = 1,
        .fat_entry_unsed = 0,
    };
    struct fat_entry file_hello = {
        .fat_entry_atime = t,
        .fat_entry_ctime = t,
        .fat_entry_mtime = t,
        .fat_entry_file_len = 0,
        .fat_entry_start_block = k + 3,
        .fat_entry_flags = 1,
        .fat_entry_unsed = 0,
    };
    strcpy(dir_current.fat_entry_name, ".");
    strcpy(dir_parent.fat_entry_name, "..");

    // debug start
    // strcpy(dir_hello.fat_entry_name, "hello_dir");
    // strcpy(file_hello.fat_entry_name, "hello_file");
    // debug end

    memset(flush_area, 0, FAT_BLOCK_SIZE);
    memcpy(flush_area, &dir_current, 64);
    memcpy(flush_area + 64, &dir_parent, 64);

    // debug start
    // memcpy(flush_area + 128, &dir_hello, 64);
    // memcpy(flush_area + 192, &file_hello, 64);
    // debug end

    printf("Writing root dir %s\n", ((fat_entry_t*)flush_area)[0].fat_entry_name);
    printf("Writing root dir %s\n", ((fat_entry_t*)flush_area)[1].fat_entry_name);
    printf("@ file offset %ld\n", lseek(fd, 0, SEEK_CUR));
    write(fd, flush_area, FAT_BLOCK_SIZE);

    printf("Write rest\n");
    printf("@ file offset %ld\n", lseek(fd, 0, SEEK_CUR));

    memset(flush_area, 0, FAT_BLOCK_SIZE);
    for (int i = 0; i < N - 1; ++i)
        write(fd, flush_area, FAT_BLOCK_SIZE);

    printf("all done\n");
    printf("@ file offset %ld\n", lseek(fd, 0, SEEK_CUR));
}

#endif /* FAT_FORMAT_H */
