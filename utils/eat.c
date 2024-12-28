#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MB (1024 * 1024)

int main(int argc, char *argv[]) {
    char *mem;
    int sz = atoi(argv[1]);

    printf("Allocating %d MB\n", sz);
    mem = (char *)malloc(MB * sz);
    memset(mem, 0, MB * sz);
    sleep(600);
    free(mem);

    return 0;
}