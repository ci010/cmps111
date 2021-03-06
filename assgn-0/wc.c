#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFF_SIZE 1024
char buffer[BUFF_SIZE];

void wc(int fd, int* countArray) {
    int count = 0;
    char inSpace = 1;
    while ((count = read(fd, buffer, BUFF_SIZE)))
        for (int i = 0; i < count; ++i) {
            char c = buffer[i];
            if (c == EOF)
                return;
            ++countArray[0];
            switch (c) {
            case '\n':
                ++countArray[2];
            case '\t':
            case ' ':
                inSpace = 1;
                break;
            default:
                if (inSpace) {
                    ++countArray[1];
                    inSpace = 0;
                }
                break;
            }
        }
}

int main(int argc, char** argv) {
    int totalArray[3];
    int* countArray = (int*)malloc(sizeof(int) * 3);
    if (argc == 1) {
        wc(0, countArray);
        printf("%d\t%d\t%d\n", countArray[0], countArray[1], countArray[2]);
    } else {
        for (int ai = 1; ai < argc; ++ai) {
            const char* fileName = argv[ai];
            int fd = open(fileName, O_RDONLY);
            if (fd == -1) {
                fprintf(stderr, "wc: %s: open: No such file or directory\n", fileName);
                continue;
            }
            countArray[0] = countArray[1] = countArray[2] = 0;
            wc(fd, countArray);
            totalArray[0] += countArray[0];
            totalArray[1] += countArray[1];
            totalArray[2] += countArray[2];
            printf("%d\t%d\t%d %s\n", countArray[0], countArray[1], countArray[2], fileName);
            close(fd);
        }
        if (argc > 2)
            printf("%d\t%d\t%d total\n", totalArray[0], totalArray[1], totalArray[2]);
    }
    free(countArray);
    return 0;
}