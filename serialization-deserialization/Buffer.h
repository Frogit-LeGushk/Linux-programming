#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


#include "Handlers.h"


#ifndef H_BUFFER
#define H_BUFFER


typedef struct SBuffer {
    size_t size;
    uint8_t *buf;
    size_t top;
} Buffer;


void createBuffer(Buffer **buffer, size_t size);
void deleteBuffer(Buffer **buffer);
void saveInFileBuffer(const Buffer *buffer, const char *path);
void loadFlomFileBuffer(const Buffer *buffer, const char *path);
void printBuffer(const Buffer *buffer);
WrapperTypes getWrapperTypeBuffer(Buffer *buffer);

#endif
