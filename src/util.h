#ifndef UTIL_H
#define UTIL_H

#define UNUSED(X) (void)(X)

void err_msg(const char *msg);

void syntax_error(const char *code, size_t lineno, size_t col);

#endif /* UTIL_H */
