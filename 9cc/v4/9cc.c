#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// token kind
// use typedef to give a new name to an already existing data type
// typedef [existing data type] [new name];
// enum is a type of integer constant that can be handled by naming it.
typedef enum {
    TK_RESERVED,    // symbol
    TK_NUM,         // integer token
    TK_EOF,         // token representing the end of the input
} TokenKind;

typedef struct Token Token;

// token type
// A structure is a data type, and its type frame must be declared first. A structure is a data type,
//   and its type frame must be declared first, and then an entity (object) of the structure can be declared
//   and used by declaring a variable of the type of the frame.
// The declaration of a structure's typecode and the declaration of a structure variable with that typecode
//   are done as follows.
//     struct [struct tag name] {order of members}; /* declaration of typecode */
//     struct ]struct tag name] [struct variable name]; /* declaration of struct variable */
struct Token {
    TokenKind kind; // token type
    Token *next;    // next input token
    int val;        // if kind is TK_NUM, the number
    char *str;      // token string(ASCII Code)
};

// input program
char *user_input;

// tokens that are currently in focus
Token *token;

// function to report an error
// takes the same arguments as printf
// void type is a type that indicates that there is no type.
void error (char *fmt, ...) {
    // va_list: type for storing information for handling variable number of real arguments
    va_list ap;
    // va_start: initialize va_list and start using variable arguments
    va_start(ap, fmt);
    // vprintf: outputs the data of a variable length argument list to a stream according to the format string
    vfprintf(stderr, fmt, ap);
    // fprintf: output to a stream according to a format string
    fprintf(stderr, "\n");
    exit(1);
}

// reports an error location and exit.
void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s\n", pos, ""); // print pos spaces.
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}



// if the next token is the expected symbol,
// read the token one more time and return true. Otherwise, return false.
bool consume(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op)
        return false;
    token = token->next;
    return true;
}

// if the next token is the expected symbol, read the token one more time.
// otherwise, report an error.
void expect(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op)
        error_at(token->str, "expected '%c'", op);
    token = token->next;
}

// if the next token is a number, read the token one more time and return the number.
// otherwise, report an error.
int expect_number() {
    if (token->kind != TK_NUM)
        error_at(token->str, "expected a number");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

// create a new token and connect it to cur
Token *new_token(TokenKind kind, Token *cur, char *str) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

// tokenize `user_input` and returns new tokens
Token *tokenize() {
    char *p = user_input;
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while(*p) {
        // skip whitespace characters
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-') {
            // in the case of postponement, operations other than those using the increment operator
            //   will be performed first.
            cur = new_token(TK_RESERVED, cur, p++);
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        error(p, "expected a number");
    }

    new_token(TK_EOF, cur, p);
    return head.next;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "The number of arguments is incorrect.\n");
        return 1;
    }

    // do tokenize
    user_input = argv[1];
    token = tokenize();

    // output the first half of the assembly
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    // the first part of the expression must be a number,
    //   so check for that and output the first mov instruction
    printf("  mov rax, %d\n", expect_number());

    // output the assembly while consuming the sequence of tokens `+ <number>` or `- <number>
    while (!at_eof()) {
        if (consume('+')) {
            printf("  add rax, %d\n", expect_number());
            continue;
        }

        expect('-');
        printf("  sub rax, %d\n", expect_number());
    }

    printf("  ret\n");
    return 0;
}