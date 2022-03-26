#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#ifndef H_HANDLERS
#define H_HANDLERS


typedef enum {
    PRIMITIVE,
    ARRAY,
    STRUCT
} WrapperTypes;


typedef enum {
    I8,
    I16,
    I32,
    I64,

    FLOAT,
    DOUBLE
} PrimitiveTypes;

size_t getTypeSize(const PrimitiveTypes type);
bool isLittleEndian(void);
size_t getSizeFile(const char *path);

#endif // H_HANDLERS
