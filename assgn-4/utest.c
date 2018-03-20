#include "fat.h"
#include "fat_format.h"
#include <sys/stat.h>

void readdir(const char* path)
{
    fat_entry_t* entry = fat_entry_open(path, 0, 0);
    fat_entry_t sub_entries[64];

    int block = entry->fat_entry_start_block;

    while (block != 0 && block != -2) {
        fat_block_read(block, (void*)sub_entries);
        for (int i = 0; i < 64 && sub_entries[i].fat_entry_start_block != 0; ++i) {
            printf("[%s]\t", sub_entries[i].fat_entry_name);
        }
        block = fat_block_next(block);
    }
    printf("\n");
    fat_entry_close(entry);
}

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
        for (int i = 0; i < 64 && sub_entries[i].fat_entry_start_block != 0; ++i) {
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
    fo = fat_entry_open("folder", 1, 1);
    assert(fo);
    readdir0("/");
    assert(fo);
    // assert(strcmp(fo->, "folder") == 0);
    fat_entry_close(fo);
    fat_entry_t* fob = fat_entry_open("folder/b", 1, 1);
    printf("[%d] [%s]\n", fob->fat_entry_start_block, fob->fat_entry_name);
    // readdir0("/folder");

    assert(fob);
    assert(strcmp(fob->fat_entry_name, "b") == 0);
    fat_entry_close(fob);
    // pack = fat_entry_root;
    // assert(fat_entry_search("/folder", &pack, 0) == 0);
    // _mkdir("/folder/a");
    // pack = fat_entry_root;
    // assert(fat_entry_search("/folder/a", &pack, 1));
    // pack = fat_entry_root;
    // assert(fat_entry_search("/folder/a", &pack, 0) == 0);

    // _mkdir("/folder/a/c");
    // pack = fat_entry_root;
    // assert(fat_entry_search("/folder/a/c", &pack, 1));
    // pack = fat_entry_root;
    // assert(fat_entry_search("/folder/a/c", &pack, 0) == 0);

    // fat_entry_t* dir = fat_dir_open("/folder");
    // assert(dir != 0);
    // assert(strcmp(dir->fat_entry_name, "folder") == 0);
    // assert(1 == dir->fat_entry_flags);
    // free(dir);

    // dir = fat_dir_open("/folder/a");
    // assert(dir != 0);
    // assert(strcmp(dir->fat_entry_name, "a") == 0);
    // assert(1 == dir->fat_entry_flags);
    // free(dir);

    fat_entry_t* root = fat_entry_open(".", -1, 0);
    assert(strcmp(root->fat_entry_name, ".") == 0);
    fat_entry_close(root);

    // root = fat_entry_open("/", -1, 0);
    // printf("%s\n", root->fat_entry_name);
    // assert(strcmp(root->fat_entry_name, ".") == 0);
    // fat_entry_close(root);

    // readdir0("/");

    fat_entry_t* file;
    file = fat_entry_open("/file", 0, 1);
    // assert(file != 0);
    assert(strcmp(file->fat_entry_name, "file") == 0);
    // assert(0 == file->fat_entry_flags);
    // file->fat_entry_name[0] = 'a';
    // printf("%s\n", file->fat_entry_name);
    // assert(strcmp(file->fat_entry_name, "aile") == 0);
    // fat_entry_close(file);

    // readdir0("/");

    // file = fat_entry_open("/aile", 0, 0);
    assert(file != 0);
    printf("%s\n", file->fat_entry_name);
    // assert(strcmp(file->fat_entry_name, "aile") == 0);
    assert(0 == file->fat_entry_flags);

    // int wroted = fat_entry_write(file, "abc", 4, 0);
    // char readed[wroted];
    // int read_bytes = fat_entry_read(file, readed, wroted, 0);
    // printf("readed %s\n", readed);
    // assert(strcmp("abc", readed) == 0);
    // assert(read_bytes == wroted);

    fat_rename("/file", "/bcd");
    fat_entry_close(file);

    readdir0("/");

    // fat_entry_write(file, "hello\n", 6, 0);
    // char read[20];
    // fat_entry_read(file, read, 6, 0);
    // printf("%s", read);

    // pack = fat_entry_root;
    // assert(fat_entry_search(".", &pack, -1));
    // assert(fat_entry_search("/", &pack, -1));

    // char parent[24], child[24];
    // path_par("a", parent, child);
    // printf("[%s] [%s]\n", parent, child);

    // file = fat_file_create("a");
    // assert(file);
    // printf("%s\n", file->fat_entry_name);
    // assert(0 == strcmp("a", file->fat_entry_name));
    // free(file);

    // const char* content = "hello";
    // int wroted = fat_entry_write(file, content, 6, 0);
    // printf("wrote %d bytes for [%s]\n", wroted, content);
    // assert(file->fat_entry_file_len == 6);
    // char readbuf[wroted];
    // int readed = fat_entry_read(file, readbuf, 6, 0);
    // assert(file->fat_entry_file_len == 6);
    // printf("read %d bytes for [%s]\n", readed, readbuf);
    // assert(strcmp(content, readbuf) == 0);

    // wroted = fat_entry_write(file, "abc", 3, 0);
    // assert(wroted == 3);
    // assert(file->fat_entry_file_len == 6);
    // fat_entry_read(file, readbuf, 6, 0);
    // assert(strcmp("abclo", readbuf) == 0);
    // printf("%s\n", readbuf);

    // fat_entry_write(file, "abc", 3, 1);
    // fat_entry_read(file, readbuf, 8, 0);
    // printf("%s\n", readbuf);

    // readdir("/");
    // // fat_entry_remove(file);
    // readdir("/");

    // fat_entry_write(file, "abc", 3, 3);

    // fat_entry_search("/", &pack, -1);
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