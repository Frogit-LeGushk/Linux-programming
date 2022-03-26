#include "Primitive.h"
#include "ArrayPrimitive.h"


#ifndef H_STRUCT
#define H_STRUCT


typedef struct SStructItem {
    int8_t wtype;
    void *field;
    struct SStructItem *next;
} StructItem;


typedef struct {
    int8_t wtype; // <-- wrapper type

    int16_t nameLength;
    char *name;

    int32_t fieldsLength;
    StructItem *fields;

    int32_t size;
} Struct;


void createStruct(Struct **p, const char *name);
void deleteStruct(Struct **p);

void addFieldStruct(Struct *p, int8_t wtype, void *field);
void packStruct(const Struct *p, Buffer *buffer);
void unpackStruct(Struct **p, Buffer *buffer);
void printStruct(const Struct *p);


#endif
