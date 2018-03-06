#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

//how much do we allocate in MB
#define memsize 10000
//how much do we want to access on each loop
#define access 4096

int randomNumber() {
    return (rand() % access);
}

int main() {
    int i;
    void* m[memsize];
    void* a;
    while (1) {
        //allocate <memsize> megabytes of memory
        for (i = 0; i < memsize; i++) {
            //allocate 1 MB
            m[i] = malloc(1024 * 1024);
            //set all to 0
            memset(m[i], 0, 1024 * 1024);
        }
        //access random chunks
        for (i = 0; i < access; i++) {
            int random = randomNumber();
            a = mmap(m[random], 1024 * 1024, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
        }
        //everything else
        for (i = 0; i < memsize; i++) {
            free(m[i]);
        }
    }
    return 0;
}