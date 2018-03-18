#ifndef FAT_FS_H
#define FAT_FS_H
#include <sys/types.h>

struct fat_entry {
    char fat_entry_name[24];
    
    /*
     * fs creation, modified, access time, 64 bit integer
     */
    time_t fat_entry_ctime;
    time_t fat_entry_mtime;
    time_t fat_entry_atime;
    
    u_int32_t fat_entry_file_len;
    u_int32_t fat_entry_start_block;
    u_int32_t fat_entry_flags;
    u_int32_t fat_entry_unsed;
} __attribute__ ((__packed__));


struct fat_superblock {
    u_int32_t fat_magic;
    u_int32_t fat_nblocks_total;
    u_int32_t fat_nblocks_file_table;
    u_int32_t fat_block_size;
    u_int32_t fat_block_start;
} __attribute__ ((__packed__));



#endif /* FAT_FS_H */
