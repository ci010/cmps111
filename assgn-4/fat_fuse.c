#define FUSE_USE_VERSION 26

#include "fat.h"
#include <fuse.h>

static int 
fat_getattr(const char* path, struct stat* stbuf) {
    int res = 0;
    memset(stbuf, 0, sizeof(struct stat));

    // if (strcmp(path, "/folder/b") == 0) {
    //     stbuf->st_mode = S_IFDIR | 0755;
    //     stbuf->st_nlink = 1;
    //     stbuf->st_size = 0;
    //     return 0;
    // }
    fat_entry_t *entry = fat_entry_open(path, 0, 0);

    if (entry != NULL) {
        stbuf->st_atim.tv_sec = entry->fat_entry_atime;
        stbuf->st_mtim.tv_sec = entry->fat_entry_mtime;
        stbuf->st_birthtim.tv_sec = entry->fat_entry_ctime;
        if (entry->fat_entry_flags & 1) {
            stbuf->st_mode = S_IFDIR | 0755;
            stbuf->st_nlink = 1;
            stbuf->st_size = entry->fat_entry_file_len;
        } else {
            stbuf->st_mode = S_IFREG | 0444;
            stbuf->st_nlink = 1;
            stbuf->st_size = entry->fat_entry_file_len;
        }
        fat_entry_close(entry);
        
    } else {
        res = -ENOENT;
    }

    return res;
}


/**
 * the most tricky one in common syscalls.... 
 */ 
static int
fat_rename(const char *path, const char *target_path) {
    // fat_entry_t *src = fat_entry_open(path, 0, 0);
    // if (src == NULL) {
    //     return ENOENT;
    // }
    // fat_entry_t place_holder;
    // memcpy(&place_holder, src, 64);
    // src->fat_entry_start_block = 0;
    // fat_entry_close(src);

    // fat_entry_t *tar = fat_entry_open(target_path, place_holder.fat_entry_flags & 1, 1);
    // if (tar == NULL) {
    //     return EISDIR; // this is means either the parent doesn't exist, or out of mem
    // }
    // if ((tar->fat_entry_flags & 1) != (place_holder.fat_entry_flags & 1)) {
    //     return EISDIR; // from doc: newpath is an existing directory, but oldpath is not a directory, or reversed
    // }
    // /**
    //  * handle replace or normal move here
    //  * 
    //  * (there might be the optimization for renaming two entries in same dir... impl by just renaming... maybe i will do that if i have time...)
    //  */
    // fat_entry_delete(target_path);
    // time_t t = time(0);
    // tar->fat_entry_start_block = place_holder.fat_entry_start_block;
    // tar->fat_entry_file_len = place_holder.fat_entry_file_len;
    // tar->fat_entry_flags = place_holder.fat_entry_flags;
    // tar->fat_entry_ctime = place_holder.fat_entry_ctime;
    // tar->fat_entry_mtime = t;
    // fat_entry_close(tar);
    return fat_entry_rename(path, target_path);
}

/**************************
 * dir operations section
 **************************/ 

static int
fat_rmdir(const char* path) {
    fat_entry_t *entry = fat_entry_open(path, 1, 0);
    if (entry != NULL) {
        if (strcmp(".", entry->fat_entry_name) != 0 && strcmp("..", entry->fat_entry_name) != 0) {
            entry->fat_entry_start_block = 0; // set start block to 0 to invalidate it
        } 
        fat_entry_close(entry);
    }
    return 0;
}

static int 
fat_mkdir(const char* path, mode_t mode) {
    fat_entry_t *entry = fat_entry_open(path, 1, 1);
    if (entry != NULL) {
        fat_entry_close(entry);
    }
    return 0;
}

static int 
fat_opendir(const char *path, struct fuse_file_info *fi) {
    fat_entry_t *entry = fat_entry_open(path, 1, 0);
    if (!entry) {
        return -1;
    }
    /**
     * Save this founded entry result
     */
    fi->fh = (uintptr_t) entry;
    return 0;
}

static int
fat_releasedir(const char *path, struct fuse_file_info *fi) {
    fat_entry_close((fat_entry_t *) fi->fh);
    return 0;
}

/**
 * According to fuse document:
 * 
 * The readdir implementation ignores the offset parameter, and passes zero to the filler function's offset. 
 * The filler function will not return '1' (unless an error happens), so the whole directory is read in a single readdir operation.
 */ 
static int 
fat_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
    off_t offset, struct fuse_file_info* fi) {

    fat_entry_t *entry = (fat_entry_t *) fi->fh;
    fat_entry_t sub_entries[64];
    int block = entry->fat_entry_start_block;

    while (block != 0 && block != -2) {
        fat_block_read(block, (void *) sub_entries);
        for (int i = 0; i < 64; ++i) {
            if (sub_entries[i].fat_entry_start_block != 0) {
                filler(buf, sub_entries[i].fat_entry_name, NULL, 0);
            }
        }
        block = fat_block_next(block);
    } 

    return 0;
}

/**************************
 * file operations section
 **************************/ 

static int
fat_unlink(const char* path) {
    fat_entry_t *entry = fat_entry_open(path, 0, 0);
    if (entry != NULL) {
        fat_entry_delete(entry);
        fat_entry_close(entry);
    }
    return 0;
}
static int
fat_write(const char *path, const char *buf, size_t size, off_t off, struct fuse_file_info *fi) {
    fat_entry_t *entry = (fat_entry_t *) fi->fh;
    return fat_entry_write(entry, buf, size, off);
}
static int 
fat_read(const char* path, char* buf, size_t size, off_t offset,
    struct fuse_file_info* fi) {
    fat_entry_t *entry = (fat_entry_t *) fi->fh;
    return fat_entry_read(entry, buf, size, offset);
}
static int
fat_create(const char* path, mode_t mode, struct fuse_file_info *fi) {
    fat_entry_t *entry = fat_entry_open(path, 0, 1);
    if (!entry) {
        return 0;
    }
    fi->fh = (uintptr_t) entry;
    return 0;
}
static int 
fat_open(const char* path, struct fuse_file_info* fi) {
    fat_entry_t *entry = fat_entry_open(path, 0, 1);
    if (!entry) {
        return 0;
    }
    fi->fh = (uintptr_t) entry;
    return 0;
}
static int
fat_release(const char *path, struct fuse_file_info *fi) {
	fat_entry_t *file = (fat_entry_t *)(uintptr_t)fi->fh;
    fat_entry_close(file);
	return 0;
}


static struct fuse_operations oper = {
    .getattr = fat_getattr,
    .readdir = fat_readdir,
    .opendir = fat_opendir,
    .mkdir = fat_mkdir,
    .create = fat_create,
    .rename = fat_rename,
    .open = fat_open,
    .read = fat_read,
    .release = fat_release,
    .unlink = fat_unlink,
    .releasedir = fat_releasedir,
    .write = fat_write,
    .rmdir = fat_rmdir,
};

int 
main(int argc, char* argv[]) {
    int fd; 
    char *fuse_args[4];

    if (argc != 3) {
        printf("Usage: ./fat_fuse <mount folder> <image>\n");
        return -1;
    }
    fd = open(argv[2], O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);

    if (fd == -1) {
        printf("Cannot load image %s\n", argv[2]);
        return -1;
    }

    fuse_args[0] = argv[0];
    fuse_args[1] = argv[1];
    // fuse_args[2] = "-d";
    fuse_args[3] = NULL;

    fat_init(fd);
    printf("Start to run the fuse\n");
    return fuse_main(2, fuse_args, &oper, NULL);
}
