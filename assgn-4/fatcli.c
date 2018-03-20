#include "fat.h"
#include "fat_format.h"
#include <stdio.h>
#include <termios.h>

// modified by
// https://stackoverflow.com/questions/7469139/what-is-equivalent-to-getch-getche-in-linux
static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    new = old; /* make new settings same as old settings */
    new.c_lflag &= ~ICANON; /* disable buffered i/o */
    new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
    tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

/* Read 1 character without echo */
char getch(void)
{
    return getch_(0);
}

/* Read 1 character with echo */
char getche(void)
{
    return getch_(1);
}


/* Let's test it out */
int main(int argc, char** argv)
{
    if (argc != 3 && argc != 4) {
        printf("Usage: fatcli <-r | -f> <image> [size]\n");
        return -1;
    }

    if (strcmp(argv[1], "-r") == 0) {
        char c;
        int fd = open(argv[2], O_RDONLY);
        fat_init(fd);
        fat_entry_t entries[64];
        int k = fat_super.fat_super_fat_blocks;
        int block = k + 1;
        while (1) {

            if (getch() == '\033') { // if the first value is esc
                getch(); // skip the [
                switch (getch()) { // the real value
                case 'D':
                case 'A':
                    block -= 1;
                    if (block < (k + 1)) {
                        block = k + 1;
                    }
                    printf("@block %d:\n", block);
                    fat_block_read(block, entries);
                    for (int i = 0; i < 64 && entries[i].fat_entry_start_block != 0; ++i) {
                        printf("%s -> %d\n", entries[i].fat_entry_name, entries[i].fat_entry_start_block);
                    }
                    // code for arrow left
                    // code for arrow up
                    break;
                case 'C':
                case 'B':
                    block += 1;
                    if (block > (fat_super.fat_super_block_size + k + 1)) {
                        block -= 1;
                    }
                    printf("@block %d:\n", block);
                    fat_block_read(block, entries);
                    for (int i = 0; i < 64 && entries[i].fat_entry_start_block != 0; ++i) {
                        printf("%s to %d\n", entries[i].fat_entry_name, entries[i].fat_entry_start_block);
                    }
                    // code for arrow down
                    // code for arrow right
                    break;
                }
            }
        }
    } else if (strcmp(argv[1], "-f") == 0) {
        if (argc != 4) {
            printf("Usage: fatcli -f <image> <size>\n");
            return -1;
        }
        int fd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
        int N = atoi(argv[3]);
        fat_format(fd, N);
        close(fd);
    } else {
        printf("Usage: fatcli <-r-f> <image> <size>\n");
        return -1;
    }

    return 0;
}