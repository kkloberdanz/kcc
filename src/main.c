#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

static const char *prg_name = "kcc";

static void print_help() {
    const char *msg = \
        "        -c    Don't invoke the linker\n\n"
        "        -h    Display this help menu, then exit\n\n"
        "        -o outfile\n"
        "              write binary to outfile\n\n"
        "        -S    output raw assembly file\n\n"
        ;

    printf("\n%s: [-c] [-h] [-S] [-o outfile] infile\n\n", prg_name);
    printf("%s\n", msg);
}

int main(int argc, char **argv) {
    int c;
    int i;
    int dont_link;
    int assembly_output = 0;
    char *outfile = "a.out";
    char *infile = NULL;

    while ((c = getopt(argc, argv, "hSco:")) != -1) {
        switch (c) {
            case 'o':
                outfile = optarg;
                break;

            case 'h':
                print_help();
                exit(EXIT_SUCCESS);

            case 'S':
                assembly_output = 1;
                break;

            case 'c':
                dont_link = 1;
                break;

            case '?':
                puts("unknown option");
                print_help();
                exit(EXIT_FAILURE);

            default:
              print_help();
              exit(EXIT_FAILURE);
        }
    }

    for (i = optind; i < argc; i++) {
        /* TODO: allow for multiple input .c files */
        infile = argv[i];
        break;
    }

    if (!infile) {
        puts("No input file specified");
        print_help();
        exit(EXIT_FAILURE);
    }

    UNUSED(dont_link);
    UNUSED(outfile);
    UNUSED(assembly_output);
    return 0;
}
