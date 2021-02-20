#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "The number of arguments is incorrect.\n");
        return 1;
    }

    // use intel syntax
    printf(".intel_syntax noprefix\n");
    // define main as a function that can be used by the entire program
    printf(".globl main\n");
    printf("main:\n");
    // get a number from the command line and set it to rax
    printf("  mov rax, %d\n", atoi(argv[1]));
    // do return
    printf("  ret\n");
    return 0;
}