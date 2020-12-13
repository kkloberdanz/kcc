#include <stdio.h>

#include "lex.h"
#include "util.h"

TokList *lex(
    const char *infile,
    const char *outfile,
    char assembly_output,
    char dont_link
) {
    UNUSED(infile);
    UNUSED(outfile);
    UNUSED(assembly_output);
    UNUSED(dont_link);

    FILE *fp = fopen(infile, "r");
    if (!fp) {
        return NULL;
    }
    return NULL;
}
