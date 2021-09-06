#include "error.h"

void error(char *msg) {
    perror(msg);
    exit(1);
}

void check(int byte_len, char *msg) {
    if (byte_len < 0)
        error(msg);
}