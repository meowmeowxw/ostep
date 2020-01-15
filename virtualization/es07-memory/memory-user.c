#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int mem;
    char *array;
    if (argc != 2) {
        fprintf(stderr, "use: ./memory-user membytes\n");
        exit(1);
    }
    printf("pid: %u\n", (int)getpid());
    mem = atoi(argv[1]);
    array = malloc(mem);
    while (1) {
        for (int i = 0; i < mem; i++) {
            array[i] = 'b';
        }
    }
    free(array);
    return 0;
}
