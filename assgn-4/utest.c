#include "fat.h"
#include "fat_format.h"
#include <sys/stat.h>


static int
fat_rename(const char* path, const char* target_path)
{
    fat_entry_t* src = fat_entry_open(path, 0, 0);
    if (src == NULL) {
        return ENOENT;
    }
    fat_entry_t place_holder;
    memcpy(&place_holder, src, 64);
    src->fat_entry_start_block = 0;
    fat_entry_close(src);

    fat_entry_t* tar = fat_entry_open(target_path, place_holder.fat_entry_flags & 1, 1);
    if (tar == NULL) {
        return EISDIR;
    }
    if ((tar->fat_entry_flags & 1) != (place_holder.fat_entry_flags & 1)) {
        return EISDIR; //newpath is an existing directory, but oldpath is not a directory, or reversed
    }
    /**
     * handle replace or normal move here
     * 
     * (there might be optimization for renaming two entries in same dir... impl by just renaming... maybe i will do that if i have time...)
     */

    time_t t = time(0);
    tar->fat_entry_start_block = place_holder.fat_entry_start_block;
    tar->fat_entry_file_len = place_holder.fat_entry_file_len;
    tar->fat_entry_flags = place_holder.fat_entry_flags;
    tar->fat_entry_ctime = place_holder.fat_entry_ctime;
    tar->fat_entry_mtime = t;
    fat_entry_close(tar);
    return 0;
}

void readdir0(const char* path)
{
    fat_entry_t* entry = fat_entry_open(path, 1, 0);
    fat_entry_t sub_entries[64];

    int block = entry->fat_entry_start_block;

    while (block != 0 && block != -2) {
        fat_block_read(block, (void*)sub_entries);
        for (int i = 0; i < 64; ++i) {
            if (sub_entries[i].fat_entry_start_block != 0)
                printf("[%s]\t", sub_entries[i].fat_entry_name);
        }
        block = fat_block_next(block);
    }
    printf("\n");
    fat_entry_close(entry);
}

void test_h(int fd)
{
    fat_entry_t pack;
    lseek(fd, 0, SEEK_SET);
    fat_format(fd, 1024);
    lseek(fd, 0, SEEK_SET);
    fat_init(fd);

    assert(NULL == fat_entry_open("/folder", 0, 0));
    assert(NULL == fat_entry_open("folder", 0, 0));

    fat_entry_t* fo = fat_entry_open("/folder", 1, 1);
    readdir0("/");
    assert(fo);
    fat_entry_close(fo);

    fo = fat_entry_open("folder", 1, 1);
    assert(fo);
    readdir0("/");
    assert(fo);
    fat_entry_close(fo);

    fat_entry_t* fob = fat_entry_open("folder/b", 1, 1);
    printf("[%d] [%s]\n", fob->fat_entry_start_block, fob->fat_entry_name);
    assert(fob);
    assert(strcmp(fob->fat_entry_name, "b") == 0);
    fat_entry_close(fob);

    fat_entry_t* root = fat_entry_open(".", -1, 0);
    assert(strcmp(root->fat_entry_name, ".") == 0);
    fat_entry_close(root);

    char readed[64];
    fat_entry_t* file;
    file = fat_entry_open("/file", 0, 1);
    assert(strcmp(file->fat_entry_name, "file") == 0);
    readdir0("/");
    fat_entry_write(file, "abc\n", 4, 0);
    fat_entry_read(file, readed, 4, 0);
    printf("%s", readed);
    assert(file != 0);
    printf("%s\n", file->fat_entry_name);
    assert(0 == file->fat_entry_flags);
    fat_entry_close(file);

    file = fat_entry_open("/bcd", 0, 1);
    fat_entry_close(file);

    fat_entry_rename("/file", "bcd");
    readed[0] = '\0';
    file = fat_entry_open("/bcd", 0, 1);
    fat_entry_read(file, readed, 4, 0);
    printf("bcd: %s", readed);
    
    fat_entry_close(file);

    readdir0("/");
    readdir0("/folder");
    printf("\n");
    fat_entry_rename("bcd", "/folder/bc");
    readdir0("/");
    readdir0("/folder");
    printf("\n");
    fat_entry_rename("folder/b", "/b-moved");

    readdir0("/");
    readdir0("/folder");
}

void test_util()
{
    char par[24], chid[24];
    path_par("/a/b/c", par, chid);
    printf("%s %s\n", par, chid);
}

int main(int argc, char** argv)
{
    int fd = open("utest.img", O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    test_h(fd);
    close(fd);
    // test_util();
}