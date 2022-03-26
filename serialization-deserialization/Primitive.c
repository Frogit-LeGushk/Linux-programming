#include "Primitive.h"


void createPrimitive(Primitive **p, PrimitiveTypes ptype, const char *name, const void *data) {
    assert(strlen(name) < 256 && sizeof(char) == getTypeSize(I8));

    *p = (Primitive *)malloc(sizeof(Primitive));
    assert(p);

    (*p)->wtype = PRIMITIVE;
    (*p)->nameLength = strlen(name);

    (*p)->name = (char *)malloc(strlen(name) * sizeof(char));
    assert((*p)->name);

    for(int16_t i = 0; i < strlen(name); i++)
        (*p)->name[i] = name[i];
    (*p)->name[strlen(name)] = '\0';

    (*p)->ptype = ptype;

    int32_t size = getTypeSize(I16) + (strlen(name) + 2) * getTypeSize(I8);
    switch (ptype) {
        case I8:
            (*p)->data = malloc(getTypeSize(I8));
            assert((*p)->data);
            *(int8_t *)((*p)->data) = *((int8_t *)data);
            size += getTypeSize(I8);
            break;
        case I16:
            (*p)->data = malloc(getTypeSize(I16));
            assert((*p)->data);
            *(int16_t *)((*p)->data) = *((int16_t *)data);
            size += getTypeSize(I16);
            break;
        case I32:
            redirect_to_I32:
            (*p)->data = malloc(getTypeSize(I32));
            assert((*p)->data);
            *(int32_t *)((*p)->data) = *((int32_t *)data);
            size += getTypeSize(I32);
            break;
        case I64:
            redirect_to_I64:
            (*p)->data = malloc(getTypeSize(I64));
            assert((*p)->data);
            *(int64_t *)((*p)->data) = *((int64_t *)data);
            size += getTypeSize(I64);
            break;
        case FLOAT:
            goto redirect_to_I32;
        case DOUBLE:
            goto redirect_to_I64;
        default: exit(EXIT_FAILURE);
    }

    size += getTypeSize(I32);
    (*p)->size = size;
}


void deletePrimitive(Primitive **p) {
    free((*p)->name);
    free((*p)->data);
    free(*p);
    *p = NULL;
}

void packPrimitive(const Primitive *p, Buffer *buffer) {
    encodeI8(buffer, p->wtype);
    encodeI16(buffer, p->nameLength);

    assert(sizeof(char) == getTypeSize(I8));
    for(int16_t i = 0; i < p->nameLength; i++)
        encodeI8(buffer, p->name[i]);

    encodeI8(buffer, p->ptype);

    switch (p->ptype) {
        case I8:
            encodeI8(buffer, *(int8_t *)(p->data));
            break;
        case I16:
            encodeI16(buffer, *(int16_t *)(p->data));
            break;
        case I32:
            encodeI32(buffer, *(int32_t *)(p->data));
            break;
        case I64:
            encodeI64(buffer, *(int64_t *)(p->data));
            break;
        case FLOAT:
            encodeFloat(buffer, *(float *)(p->data));
            break;
        case DOUBLE:
            encodeDouble(buffer, *(double *)(p->data));
            break;
        default: exit(EXIT_FAILURE);
    }

    encodeI32(buffer, p->size);
}

void unpackPrimitive(Primitive **p, Buffer *buffer) {
    int8_t wtype = decodeI8(buffer);
    int16_t nameLength = decodeI16(buffer);

    char *name = (char *)malloc(nameLength * sizeof(char));
    for(int16_t i = 0; i < nameLength; i++)
        name[i] = (char)decodeI8(buffer);
    name[nameLength] = '\0';

    int8_t ptype = decodeI8(buffer);
    void *data;

    switch (ptype) {
        case I8:
            data = malloc(getTypeSize(I8));
            *(int8_t *)data = decodeI8(buffer);
            break;
        case I16:
            data = malloc(getTypeSize(I16));
            *(int16_t *)data = decodeI16(buffer);
            break;
        case I32:
            data = malloc(getTypeSize(I32));
            *(int32_t *)data = decodeI32(buffer);
            break;
        case I64:
            data = malloc(getTypeSize(I64));
            *(int64_t *)data = decodeI64(buffer);
            break;
        case FLOAT:
            data = malloc(getTypeSize(FLOAT));
            *(float *)data = decodeFloat(buffer);
            break;
        case DOUBLE:
            data = malloc(getTypeSize(DOUBLE));
            *(double *)data = decodeDouble(buffer);
            break;
        default: exit(EXIT_FAILURE);
    }

    createPrimitive(p, ptype, name, data);
    int32_t size = decodeI32(buffer);
    free(data);

    assert(size == (*p)->size);
    assert(nameLength == (*p)->nameLength);
}

void printPrimitive(const Primitive *p) {
    printf("Primitive [%s]:\n", p->name);
    printf("\t int8_t wtype = %d\n", p->wtype);
    printf("\t int16_t nameLength = %d\n", p->nameLength);
    printf("\t char *name = %s\n", p->name);
    printf("\t int8_t ptype = %d\n", p->ptype);

    switch (p->ptype) {
        case I8:
            printf("\t int8_t data = %d\n", *(int8_t *)(p->data));
            break;
        case I16:
            printf("\t int16_t data = %hd\n", *(int16_t *)(p->data));
            break;
        case I32:
            printf("\t int32_t data = %d\n", *(int32_t *)(p->data));
            break;
        case I64:
            printf("\t int64_t data = %ld\n", *(int64_t *)(p->data));
            break;
        case FLOAT:
            printf("\t float data = %f\n", *(float *)(p->data));
            break;
        case DOUBLE:
            printf("\t double data = %lf\n", *(double *)(p->data));
            break;
        default: exit(EXIT_FAILURE);
    }

    printf("\t int32_t size = %d\n", p->size);
}











