#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: wc <filename>\n");
        return 0;
    }
    const char* fileName = argv[1];
    int fd = open(fileName, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error: Cannot open file %s\n", fileName);
        return 0;
    }
    char buffer[512];
    int count = 0;
    int charCount = 0, wordCount = 0, lineCount = 0;
    char inSpace = 1;
    while ((count = read(fd, buffer, 512))) {
        int i = 0;
        for (; i < count; ++i) {
            char c = buffer[i];
            ++charCount;
            switch (c) {
                case '\n':
                ++lineCount;
                case '\t':
                case ' ':
                inSpace = 1;
                break;

                default:
                if (inSpace) {
                    ++wordCount;
                    inSpace = 0;
                }
                break;
            }
        }
    }
    printf("%d\t%d\t%d\n", charCount, wordCount, lineCount);
    close(fd);
    return 0;
}