#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "The number of arguments is incorrect.\n");
        return 1;
    }

    char *p = argv[1];

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    // strtol converts a string to a value of type long
    // long strtol(const char *s, char **endp, int base);
    // converts the string s to a value of type long, assuming it to be a
    //   sequence of base-digit numbers, and returns the value
    // if there is a character that cannot be interpreted as a number,
    //   its address is assigned to endp

    // outputs a double precision integer as a decimal number by %ld
    printf("  mov rax, %ld\n", strtol(p, &p, 10));

    while (*p) {
        if (*p == '+') {
            p++;
            printf("  add rax, %ld\n", strtol(p, &p, 10));
            continue;
        }

        if (*p == '-') {
            p++;
            printf("  sub rax, %ld\n", strtol(p, &p, 10));
            continue;
        }

        fprintf(stderr, "Unexpected characters were detected: '%c'\n", *p);
        return 1;
    }

    printf("  ret\n");
    return 0;
}