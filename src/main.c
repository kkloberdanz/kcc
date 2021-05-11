#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"
#include "lex.h"
#include "codegen.h"

AST *parse(TokList *tokens);

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
        "              Write binary to outfile\n\n"
        "        -S    Output raw assembly file\n\n"
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
    FILE *output = NULL;
    struct Options options;
    TokList *tokens = NULL;
    int status = 1;
    int cpp_err;
    char cpp_cmd[1024];
    AST *ast = NULL;

    parse_options(argc, argv, &options);

    sprintf(cpp_cmd, "/usr/bin/cpp -D__STDC__ -std=c90 %s", options.infile);

    /* Invoke cpp for preprocesser */
    cpp_stream = popen(cpp_cmd, "r");
    if (!cpp_stream) {
        goto cleanup;
    }

    /* Lexical analysis */
    tokens = lex(cpp_stream);
    toklist_print(tokens);
    cpp_err = pclose(cpp_stream);
    cpp_stream = NULL;
    if (cpp_err) {
        err_msg("failed to preprocess\n");
        goto cleanup;
    }

    if (!tokens) {
        err_msg("failed to get tokens\n");
        goto cleanup;
    }

    /* Parse tokens to build Abstract Syntax Tree (AST) */
    ast = parse(tokens);
    if (!ast) {
        goto cleanup;
    }
    toklist_free(tokens);
    tokens = NULL;

    /* Transform AST into assembly */
    output = fopen(options.outfile, "w");
    if (!output) {
        goto cleanup;
    }
    codegen(ast, output);
    ast_free(ast);
    ast = NULL;

    /* Invoke assembler */
    /* TODO */

    /* Invoke linker */
    /* TODO */

    status = 0;

cleanup:
    if (cpp_stream) {
        pclose(cpp_stream);
        cpp_stream = NULL;
    }
    if (tokens) {
        toklist_free(tokens);
        tokens = NULL;
    }
    if (ast) {
        ast_free(ast);
        ast = NULL;
    }
    if (output) {
        fclose(output);
        output = NULL;
    }
    return status;
}
