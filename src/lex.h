#ifndef LEX_H
#define LEX_H

#include "token.h"

TokList *lex(
    const char *infile,
    const char *outfile,
    char assembly_output,
    char dont_link
);

#endif /* LEX_H */
