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
    printf("\nkcc -- Kyle's C Compiler\n");
    printf("\n%s: [-c] [-h] [-S] [-o outfile] infile\n\n", prg_name);
    printf(
        "        -c    Don't invoke the linker\n\n"
        "        -h    Display this help menu, then exit\n\n"
        "        -o outfile\n"
        "              write binary to outfile\n\n"
        "        -S    output raw assembly file\n\n"
    );
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
                break;

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
    FILE *cpp_stream = NULL;
    struct Options options;
    TokList *tokens = NULL;
    int status = 0;
    char cpp_cmd[1024];

    parse_options(argc, argv, &options);

    sprintf(cpp_cmd, "/usr/bin/cpp -std=c90 %s", options.infile);

    /* Invoke cpp for preprocesser */
    cpp_stream = popen(cpp_cmd, "r");

    /* Lexical analysis */
    tokens = lex(cpp_stream);

    if (!tokens) {
        status = 1;
        err_msg("failed to get tokens\n");
        goto cleanup;
    }

    puts("tokens:");
    toklist_print(tokens);

    /* Parse tokens to build Abstract Syntax Tree (AST) */
    /* Transform AST into Intermediate Representation (IR) */
    /* Optimize IR */
    /* Transform IR into assembly */
    /* Invoke assembler */
    /* Invoke linker */

cleanup:
    if (cpp_stream) {
        fclose(cpp_stream);
        cpp_stream = NULL;
    }
    if (tokens) {
        toklist_free(tokens);
        tokens = NULL;
    }
    return status;
}
