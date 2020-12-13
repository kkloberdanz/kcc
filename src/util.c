#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void err_msg(const char *msg) {
    const size_t len = strlen(msg);
    const char *prog_name = "kcc: ";

    if (write(STDERR_FILENO, prog_name, strlen(prog_name)) < 0) {
        /* if write to stderr failes, something really bad has happened */
        exit(EXIT_FAILURE);
    }
    if (write(STDERR_FILENO, msg, len) < 0) {
        exit(EXIT_FAILURE);
    }
}
