#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"
#include "lex.h"

struct Options {
    char dont_link;
    char assembly_output;
    char *outfile;
    char *infile;
};

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

static void parse_options(int argc, char **argv, struct Options *options) {
    int c;
    options->assembly_output = 0;
    options->infile = NULL;
    options->outfile = "a.out";

    while ((c = getopt(argc, argv, "hSco:")) != -1) {
        switch (c) {
            case 'o':
                options->outfile = optarg;
                break;

            case 'h':
                print_help();
                exit(EXIT_SUCCESS);

            case 'S':
                options->assembly_output = 1;
                break;

            case 'c':
                options->dont_link = 1;
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

    /*
     * TODO: allow for multiple input .c files
     *     for (i = optind; i < argc; i++) {
     *         options->infile = argv[i];
     *         break;
     *     }
     *
     */
    options->infile = argv[optind];

    if (!options->infile) {
        puts("No input file specified");
        print_help();
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    struct Options options;
    TokList *tokens = NULL;

    /* Lexical analysis */
    parse_options(argc, argv, &options);
    tokens = lex(
        options.infile,
        options.outfile,
        options.assembly_output,
        options.dont_link
    );

    if (!tokens) {
        err_msg("failed to get tokens\n");
        exit(EXIT_FAILURE);
    }

    /* Parse tokens to build Abstract Syntax Tree (AST) */
    /* Transform AST into Intermediate Representation (IR) */
    /* Optimize IR */
    /* Transform IR into assembly */
    /* Invoke assembler */
    /* Invoke linker */

    UNUSED(tokens);
    return 0;
}
