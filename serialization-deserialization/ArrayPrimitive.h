#include "Handlers.h"
#include "Buffer.h"
#include "Encoder.h"
#include "Primitive.h"


#ifndef H_ARRAY_PRIMITIVE
#define H_ARRAY_PRIMITIVE


typedef struct {
    int8_t wtype; // <-- wrapper type

    int16_t nameLength;
    char *name;

    int8_t ptype; // <-- primitive type
    int32_t arrayLength;

    void *data;

    int32_t size;
} ArrayPrimitive;


void createArrayPrimitive(ArrayPrimitive **p, PrimitiveTypes ptype, int32_t arrayLength, const char *name, const void *data);
void deleteArrayPrimitive(ArrayPrimitive **p);
void packArrayPrimitive(const ArrayPrimitive *p, Buffer *buffer);
void unpackArrayPrimitive(ArrayPrimitive **p, Buffer *buffer);
void printArrayPrimitive(const ArrayPrimitive *p);

#endif
