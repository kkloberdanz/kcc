#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void err_msg(const char *msg) {
    const size_t len = strlen(msg);
    if (write(STDERR_FILENO, msg, len) < 0) {
        /* if write to stderr failes, something really bad has happened */
        exit(EXIT_FAILURE);
    }
}
