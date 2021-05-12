#ifndef LEX_H
#define LEX_H

#include "token.h"

TokList *lex(void);

void lex_set_file(FILE *fp);

char *lex_current_line(void);

char *lex_current_filename(void);

#endif /* LEX_H */
