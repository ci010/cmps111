#ifndef FAT_FS_H
#define FAT_FS_H
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <assert.h>
#include <time.h>

#define FAT_BLOCK_SIZE 4096
#define FAT_ENCRT 0x0010
#define FAT_ENDIR 0x0001

/**
 * helper func to calculate last index of / in a string
 */ 
int last_idx(const char* path) {
    int cur = 0;
    int last = -1;
    int res = 0;

    while (path[cur] != '\0') {
        if (path[cur] == '/')
            last = cur;
        ++cur;
    }
    return last;
}

/**
 * helper func to split the str by / into parent and child
 * 
 * e.g. /a/b/c -> parent: /a/b, child: c
 */ 
void path_par(const char* str, char *parent, char *child) {
    char *cpy = strdup(str);
    int last = last_idx(str);
    if (last == -1) {
        parent[0] = '/';
        parent[1] = '\0';
        strcpy(child, str);
    } else {
        strcpy(child, cpy + last + 1);
        if (last == 0) {
            cpy[1] = '\0';
        } else {
            cpy[last] = '\0';
        }
        strcpy(parent, cpy);
    }
    free(cpy);
}

/**
 * these are all by pdf definitions
 */ 
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
} __attribute__((__packed__));

struct fat_superblock {
    u_int32_t fat_magic;
    u_int32_t fat_super_blocks;
    u_int32_t fat_super_fat_blocks;
    u_int32_t fat_super_block_size;
    u_int32_t fat_super_block_start;
} __attribute__((__packed__));


typedef struct fat_superblock fat_superblock_t;
typedef struct fat_entry fat_entry_t;


int fat_dev;
fat_superblock_t fat_super;
/**
 * The root dir '.'
 */ 
fat_entry_t fat_entry_root;
int *FAT;


/*****************************
 * Block manipulation section
 *****************************/ 

/**
 * read block to buff
 */ 
static int
fat_block_read(int block_num, void* buf) {
    lseek(fat_dev, block_num * FAT_BLOCK_SIZE, SEEK_SET);
    read(fat_dev, buf, FAT_BLOCK_SIZE);
    return 0;
}

/**
 * write block from buff with size
 */
static void
fat_block_write(int block_num, const void* buf, size_t size, off_t off) {
    if (buf != NULL) 
    lseek(fat_dev, block_num * FAT_BLOCK_SIZE + off, SEEK_SET);
    write(fat_dev, buf, size);
}

/**
 * find next block by current block through FAT
 */ 
static int
fat_block_next(int block_num) {
    if (block_num == -2) return 0;
    return FAT[block_num];
}

/**
 * allocate a block by linear search
 */ 
static int
fat_block_alloc() {
    int block = -1;
    for (int i = fat_super.fat_super_block_start; i < fat_super.fat_super_block_size; ++i) {
        if (FAT[i] == 0) {
            block = i;
            break;
        }
    }
    /**
     * if there is no free block at all,
     * then... out of memory
     */ 
    if (block == -1) {
        return 0;
    }
    FAT[block] = -2;
    return block;
}

/**
 * find next block; if not exist, allocate by linear search
 * this operation has side-effect that it will directly change the FAT as the new block is allocated  
 * 
 * return free block id if allocation success
 * return 0 if out of mem 
 */ 
static int
fat_block_next_alloc(int block_num) {
    if (block_num == -2) return 0;
    if (FAT[block_num] == -2) {
        int block = fat_block_alloc();
        if (block == 0) {
            return 0;
        }
        FAT[block_num] = block;
    }
    return FAT[block_num];
}

/****************************
 * Entry manipulation section 
 ****************************/

/**
 * allocate new entry by name, the passin entry is the parent entry,
 * and it will be reset to the child entry if the allocation success.
 * 
 * flags represent if the entry is a folder
 * 
 * return the offset of this entry block in the image
 * return 0 if allocation failed
 */ 
static off_t
fat_entry_alloc(const char *name, fat_entry_t *entry, int flags) {
    fat_entry_t entries[64];
    int block = entry->fat_entry_start_block;
    int free_pos = -1;
    while (free_pos == -1) {
        /**
         * try to find free entry in this block
         */ 
        fat_block_read(block, entries);
        for (int i = 0; i < 64; ++i) {
            if (entries[i].fat_entry_start_block == 0) {
                free_pos = i;
                break;
            }
        }
        if (free_pos != -1) break;
        /**
         * if there is not block for this dir,
         * allocate one
         */ 
        int block = fat_block_next_alloc(block);
        
        if (block == 0) {
            /**
             * if there is no free block at all,
             * then... out of memory
             */ 
            if (block == 0) {
                return 0;
            }
        }
    }
    
    time_t t = time(0);
    int child_start_block = fat_block_alloc();
    if (child_start_block == 0) {
        /**
         * out of memory
         */ 
        return 0;
    }
    entries[free_pos].fat_entry_atime = t;
    entries[free_pos].fat_entry_ctime = t;
    entries[free_pos].fat_entry_mtime = t;
    entries[free_pos].fat_entry_start_block = child_start_block;
    entries[free_pos].fat_entry_file_len = 0;
    entries[free_pos].fat_entry_flags = flags;
    entries[free_pos].fat_entry_unsed = 0;

    strcpy(entries[free_pos].fat_entry_name, name);
    lseek(fat_dev, block * 4096, SEEK_SET);
    write(fat_dev, entries, FAT_BLOCK_SIZE);

    off_t offset = block * FAT_BLOCK_SIZE + free_pos * 64;
    // fat_entry_t * e = (fat_entry_t *) mmap(NULL, 64, PROT_READ | PROT_WRITE, MAP_PRIVATE, fat_dev, offset);
    // memcpy(entry, &entries[free_pos], 64);

    /** 
     * if it's a dir, create . and ..
     */
    if (flags & 1) {
        
        /**
         * we known the start block is empty... so i'm lazy to check
         */ 
        fat_block_read(child_start_block, entries);
        entries[0].fat_entry_atime = t;
        entries[0].fat_entry_ctime = t;
        entries[0].fat_entry_mtime = t;
        entries[0].fat_entry_start_block = child_start_block;
        entries[0].fat_entry_file_len = 0;
        entries[0].fat_entry_flags = flags;
        entries[0].fat_entry_unsed = 0;
        strcpy(entries[0].fat_entry_name, ".");

        entries[1].fat_entry_atime = t;
        entries[1].fat_entry_ctime = t;
        entries[1].fat_entry_mtime = t;
        entries[1].fat_entry_start_block = entry->fat_entry_start_block;
        entries[1].fat_entry_file_len = 0;
        entries[1].fat_entry_flags = flags;
        entries[1].fat_entry_unsed = 0;
        strcpy(entries[1].fat_entry_name, "..");

        lseek(fat_dev, child_start_block * 4096, SEEK_SET);
        write(fat_dev, entries, FAT_BLOCK_SIZE);
    }
    
    return offset;
}

/**
 * open file or dir
 * return the file or dir entry if found
 * return NULL if not found
 */ 
static fat_entry_t *
fat_entry_open(const char *path, char dir, char alloc) {
    if (strcmp("/", path) == 0) {
        fat_entry_t *e = (fat_entry_t *) mmap(NULL, 64, PROT_READ | PROT_WRITE, MAP_SHARED, fat_dev, fat_super.fat_super_block_start * FAT_BLOCK_SIZE);
        e->fat_entry_atime = time(0);
        return e;
    }

    fat_entry_t sub_entries[64];
    int block, i;
    char found = 0;
    char *token;
    char parent[strlen(path) + 1];
    char child[strlen(path) + 1];
    char *rest = parent;
    fat_entry_t entry = fat_entry_root;

    path_par(path, parent, child);
    /**
     * little a bit duplicated code here...
     * here i try to found the parent folder first
     */ 
    while ((token = strtok_r(rest, "/", &rest))) {
        if ((entry.fat_entry_flags & 1) == 0) {
            return NULL;
        }
        block = entry.fat_entry_start_block;
        /**
         * iterate block "horizontally"
         */ 
        while (block != 0) {
            fat_block_read(block, (void*)sub_entries);
            for (i = 0; i < 64; ++i) {
                if (sub_entries[i].fat_entry_start_block == 0) continue;
                if (strcmp(token, sub_entries[i].fat_entry_name) == 0) {
                    if ((sub_entries[i].fat_entry_flags & 1)  == 0)
                        continue;
                    entry = sub_entries[i];
                    found = 1;
                    break;
                }
            }
            if (found) break;
            block = fat_block_next(block);
        }
        if (!found) return NULL;
    }
    /**
     * duplicated code....
     * similarly, find the child file
     */  
    block = entry.fat_entry_start_block;
    found = 0;
    while (block != 0) {
        fat_block_read(block, (void*)sub_entries);
        for (i = 0; i < 64; ++i) {
            if (sub_entries[i].fat_entry_start_block == 0) continue;
            if (strcmp(child, sub_entries[i].fat_entry_name) == 0) {
                entry = sub_entries[i];
                found = 1;
                break;
            }
        }
        if (found)
            break;
        block = fat_block_next(block);
    }
    
    off_t offset;
    if (!found) {
        if (!alloc) return NULL;
        /**
         * if not found, we alloc one new
         */ 
        offset = fat_entry_alloc(child, &entry, dir == -1 ? 0 : dir);
    } else {
        offset = block * FAT_BLOCK_SIZE + i * 64;
    }
    /**
     * use mem mapping to get this entry
     * mmap really helps me that i do not need context (which block, which index) of the entry to do the deletion, modification case  
     */
    // printf("%s @ %ld\n", path, offset);
    fat_entry_t *e = (fat_entry_t *) mmap(NULL, 64, PROT_READ | PROT_WRITE, MAP_SHARED, fat_dev, offset);
    e->fat_entry_atime = time(0);
    return e;
}

/**
 * close the entry is just stop map the mem
 */ 
static int
fat_entry_close(fat_entry_t * entry) {
    return munmap(entry, 64);
}

/**
 * write a entry 
 */  
static int
fat_entry_write(fat_entry_t *entry, const char *buf, size_t size, off_t off) {
    int block = entry->fat_entry_start_block;
    size_t total = size;
    off_t off_local = off;
    int start, end, len;
    char *ptr = (char *) buf;

    while (off_local > 4096) {
        if (block < fat_super.fat_super_fat_blocks) {
            return -ENOENT;
        }
        /**
         * emmm... i think lseek should support the pos that greater than the current filesize
         * i directly impl by alloc the block when we write file with a greater offset 
         */ 
        off_local -= 4096;
        block = fat_block_next_alloc(block);
        if (block == 0) return -1;
    }
    printf("Write to block %d\n", block);
    
    while (total > 0) {
        /**
         * exam the range
         */ 
        if (block < fat_super.fat_super_fat_blocks) {
            return -ENOENT;
        }
        /**
         * tricky algorithm....
         * 
         * we can the cases by "graph":
         * 
         *  |__________|__________|
         *     A ^__B__|_____^
         * 
         * where, A is the offset initially, the read range B should terminal at the end of block, not the total length
         * 
         *  |__________|__________|
         *     A ^__^
         * 
         * though, in the normal cases, the read's end should just the total length
         * 
         * therefore, start = offset initial, end = MIN(start + total len, end of block)
         * 
         */ 
        start = off_local; 
        end = 4096 > (total + start) ? (total + start) : 4096;
        len = end - start;
        fat_block_write(block, ptr, len, off);
        ptr += len;
        total -= len;
        off_local = 0;

        block = fat_block_next_alloc(block);
        if (block == 0) {
            // out of mem
            return -1;
        }
    }
    entry->fat_entry_file_len = start + size > entry->fat_entry_file_len ? 
        start + size : entry->fat_entry_file_len;
    
    return size;
}

/**
 * read the entry to buf
 */ 
static int
fat_entry_read(fat_entry_t *entry, char *buf, size_t size, off_t off) {
    if (entry->fat_entry_flags & 1) {
        return -EISDIR;
    }
    int readed_sz = 0;
    u_int32_t block = entry->fat_entry_start_block;
    u_int32_t total = entry->fat_entry_file_len > size ? size : entry->fat_entry_file_len;
    off_t off_local = off;

    /**
     * if the offset is greater than the start block,
     * skip through the target block by transvering the FAT 
     */
    while (off_local > 4096) {
        /**
         * if we reach the end of the block,
         * but the offset still greater than the block,
         * we cannot read... it excees the range
         * 
         * if we find that the next block is in protected range (FAT or super block),
         * emmm... somthing happend..
         */ 
        if (block == -2 && off_local > 4096) {
            return -1;
        }
        if (block < fat_super.fat_super_fat_blocks) {
            return -ENOENT;
        }
        off_local -= 4096;
        block = fat_block_next(block);
    }

    char readed[4096];
    int start, end, len;
    
    while (total > 0 && block != 0) {
        /**
         * exam the range
         */ 
        if (block < fat_super.fat_super_fat_blocks) {
            return -ENOENT;
        }
        fat_block_read(block, (void *) readed);
        /**
         * tricky algorithm....
         * we read from [start] which is the local offset in a block;
         * then, the [end] is the MIN of the end of block and the [start] + [# of bytes we want to read];
         * we cal [len] from [start] and [end];
         * finnaly, we shift the buf pointer, count bytes we readed and reduce the total size we want;
         */ 
        start = off_local; 
        end = 4096 > (total + start) ? (total + start) : 4096;
        len = end - start;
        memcpy(buf, readed + start, len);
        buf += len;
        readed_sz += len;
        total -= len;
        off_local = 0; // local offset is consumed at the first time we read

        block = fat_block_next(block);
    }

    return readed_sz;
}

/**
 * delete an entry, just by invalidate the flag
 * 
 * it doesn't handle the dir's content, i hope the person use this func should first check
 * if the dir is empty, then remove the dir
 */ 
static char
fat_entry_delete(fat_entry_t *entry) {
    int block = entry->fat_entry_start_block;
    char empty[4096];
    /**
     * wipe out all
     */ 
    while (block != 0) {
        fat_block_write(block, empty, 4096, 0);
        int oldblock = block;
        block = fat_block_next(block);
        FAT[oldblock] = 0;
    }
    entry->fat_entry_start_block = 0;
    return 1;
}


static int
fat_entry_rename(const char* from, const char* to) {
    if (strcmp(from, to) == 0) return 0;
    fat_entry_t *e = fat_entry_open(from, 0, 0);
    if (e == NULL) {
        return -ENOENT;
    }
    char from_p[24], from_c[24];
    char to_p[24], to_c[24];
    path_par(from, from_p, from_c);
    path_par(to, to_p, to_c);
    
    if (strcmp(from_p, to_p) == 0) {
        fat_entry_t *tar = fat_entry_open(to, 0, 0);
        if (tar != NULL) {
            if ((tar->fat_entry_flags & 1) != (e->fat_entry_flags & 1)) {
                fat_entry_close(e);
                fat_entry_close(tar);
                return EISDIR; // from doc: newpath is an existing directory, but oldpath is not a directory, or reversed
            }
            fat_entry_delete(tar);
            fat_entry_close(tar);
        }
        strcpy(e->fat_entry_name, to_c);
        e->fat_entry_mtime = time(0);
        fat_entry_close(e);
    } else {
        fat_entry_t *tar = fat_entry_open(to, e->fat_entry_flags & 1, 1);
        if (tar != NULL) {
            if ((tar->fat_entry_flags & 1) != (e->fat_entry_flags & 1)) {
                fat_entry_close(e);
                fat_entry_close(tar);
                return EISDIR; // from doc: newpath is an existing directory, but oldpath is not a directory, or reversed
            }
        }
        /**
         * emmm... we have to modify the origin dir .. point to correct pos
         */ 
        if ((e->fat_entry_flags & 1) == 1) {
            /**
             * it's really a... wasted way to do this...
             */ 
            fat_entry_t entries[64];
            fat_block_read(tar->fat_entry_start_block, entries);
            assert(0 == strcmp(entries[1].fat_entry_name, ".."));
            int parent = entries[1].fat_entry_start_block;
            fat_block_write(tar->fat_entry_start_block, entries, 4096, 0);

            fat_block_read(e->fat_entry_start_block, entries);
            assert(0 == strcmp(entries[1].fat_entry_name, ".."));
            entries[1].fat_entry_start_block = parent;
            fat_block_write(e->fat_entry_start_block, entries, 4096, 0);
        }

        fat_entry_delete(tar);
        tar->fat_entry_start_block = e->fat_entry_start_block;
        tar->fat_entry_ctime = e->fat_entry_ctime;
        tar->fat_entry_atime = e->fat_entry_atime;
        tar->fat_entry_mtime = time(0);
        tar->fat_entry_file_len = e->fat_entry_file_len;
        tar->fat_entry_flags = e->fat_entry_flags;

        

        e->fat_entry_start_block = 0;
        fat_entry_close(e);
        fat_entry_close(tar);
    }
    return 0;
}

/***************
 * init section
 ***************/

void fat_init(int fd) {
    read(fd, &fat_super.fat_magic, 4);
    read(fd, &fat_super.fat_super_blocks, 4);
    read(fd, &fat_super.fat_super_fat_blocks, 4);
    read(fd, &fat_super.fat_super_block_size, 4);
    read(fd, &fat_super.fat_super_block_start, 4);
    fat_dev = fd;

    printf("Load image, total block: %d, total FAT: %d, first block start at %d\n", 
        fat_super.fat_super_blocks, fat_super.fat_super_fat_blocks, fat_super.fat_super_block_start);

    assert(fat_super.fat_magic == 0xCAFED00D);
    assert(fat_super.fat_super_block_size == FAT_BLOCK_SIZE);
    
    fat_entry_t root_block_e[64];
    fat_block_read(fat_super.fat_super_block_start, root_block_e);
    memcpy(&fat_entry_root, root_block_e, 64);

    printf("Load root dir: %s @%d\n", root_block_e[0].fat_entry_name, root_block_e[0].fat_entry_start_block);
    assert(strcmp(root_block_e[0].fat_entry_name, ".") == 0);
    assert(strcmp(root_block_e[1].fat_entry_name, "..") == 0);
    
    lseek(fd, 0, SEEK_SET);
    /**
     * map the FAT
     */ 
    FAT = mmap(NULL, fat_super.fat_super_fat_blocks * 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096);

    assert(FAT != MAP_FAILED);
    assert(FAT[0] == -1);
}

#endif /* FAT_FS_H */
