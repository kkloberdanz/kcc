#ifndef TYPE_H
#define TYPE_H

typedef enum Type {
    TYPE_NONE,
    TYPE_VOID,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_CHAR_P,
    TYPE_SHORT,
    TYPE_LONG,
    TYPE_DOUBLE,
    TYPE_SIGNED,
    TYPE_UNSIGNED,
    TYPE_BUILTIN_VA_LIST
} Type;

#endif /* TYPE_H */
