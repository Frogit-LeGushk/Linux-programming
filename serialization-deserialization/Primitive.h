#include "Handlers.h"
#include "Buffer.h"
#include "Encoder.h"
#include "Decoder.h"


#ifndef H_PRIMITIVE
#define H_PRIMITIVE

typedef struct {
    int8_t wtype; // <-- wrapper type

    int16_t nameLength;
    char *name;

    int8_t ptype; // <-- primitive type
    void *data;

    int32_t size; // <-- size of all
} Primitive;


void createPrimitive(Primitive **p, PrimitiveTypes ptype, const char *name, const void *data);
void deletePrimitive(Primitive **p);
void packPrimitive(const Primitive *p, Buffer *buffer);
void unpackPrimitive(Primitive **p, Buffer *buffer);
void printPrimitive(const Primitive *p);

#endif
