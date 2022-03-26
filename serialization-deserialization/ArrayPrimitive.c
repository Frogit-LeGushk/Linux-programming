#include "ArrayPrimitive.h"


void createArrayPrimitive   (
                            ArrayPrimitive **p,
                            PrimitiveTypes ptype,
                            int32_t arrayLength,
                            const char *name,
                            const void *data
                            )
{
    assert(strlen(name) < 256 && sizeof(char) == getTypeSize(I8));

    *p = (ArrayPrimitive *)malloc(sizeof(ArrayPrimitive));
    assert(p);

    (*p)->wtype = ARRAY;
    (*p)->nameLength = strlen(name);

    (*p)->name = (char *)malloc(strlen(name) * sizeof(char));
    assert((*p)->name);

    for(int16_t i = 0; i < strlen(name); i++)
        (*p)->name[i] = name[i];
    (*p)->name[strlen(name)] = '\0';


    (*p)->ptype = ptype;
    (*p)->arrayLength = arrayLength;

    int32_t size = getTypeSize(I16) + (strlen(name) + 2) * getTypeSize(I8) + getTypeSize(I32);
    switch (ptype) {
        case I8:
            (*p)->data = malloc(arrayLength * getTypeSize(I8));
            assert((*p)->data);

            for(int32_t i = 0; i < arrayLength; i++)
                *((int8_t *)(*p)->data + i) = *((int8_t *)data + i);

            size += arrayLength * getTypeSize(I8);
            break;
        case I16:
            (*p)->data = malloc(arrayLength * getTypeSize(I16));
            assert((*p)->data);

            for(int32_t i = 0; i < arrayLength; i++)
                *((int16_t *)(*p)->data + i) = *((int16_t *)data + i);

            size += arrayLength * getTypeSize(I16);
            break;
        case I32:
            redirect_to_I32:
            (*p)->data = malloc(arrayLength * getTypeSize(I32));
            assert((*p)->data);

            for(int32_t i = 0; i < arrayLength; i++)
                *((int32_t *)(*p)->data + i) = *((int32_t *)data + i);

            size += arrayLength * getTypeSize(I32);
            break;
        case I64:
            redirect_to_I64:
            (*p)->data = malloc(arrayLength * getTypeSize(I64));
            assert((*p)->data);

            for(int32_t i = 0; i < arrayLength; i++)
                *((int64_t *)(*p)->data + i) = *((int64_t *)data + i);

            size += arrayLength * getTypeSize(I64);
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

void deleteArrayPrimitive(ArrayPrimitive **p) {
    free((*p)->name);
    free((*p)->data);
    free(*p);
    *p = NULL;
}

void packArrayPrimitive(const ArrayPrimitive *p, Buffer *buffer) {
    encodeI8(buffer, p->wtype);
    encodeI16(buffer, p->nameLength);

    assert(sizeof(char) == getTypeSize(I8));
    for(int16_t i = 0; i < p->nameLength; i++)
        encodeI8(buffer, p->name[i]);

    encodeI8(buffer, p->ptype);
    encodeI32(buffer, p->arrayLength);

    switch (p->ptype) {
        case I8:
            for(int32_t i = 0; i < p->arrayLength; i++)
                encodeI8(buffer, *((int8_t *)p->data + i));
            break;
        case I16:
            for(int32_t i = 0; i < p->arrayLength; i++)
                encodeI16(buffer, *((int16_t *)p->data + i));
            break;
        case I32:
            for(int32_t i = 0; i < p->arrayLength; i++)
                encodeI32(buffer, *((int32_t *)p->data + i));
            break;
        case I64:
            for(int32_t i = 0; i < p->arrayLength; i++)
                encodeI64(buffer, *((int64_t *)p->data + i));
            break;
        case FLOAT:
            for(int32_t i = 0; i < p->arrayLength; i++)
                encodeFloat(buffer, *((float *)p->data + i));
            break;
        case DOUBLE:
            for(int32_t i = 0; i < p->arrayLength; i++)
                encodeDouble(buffer, *((double *)p->data + i));
            break;
        default: exit(EXIT_FAILURE);
    }

    encodeI32(buffer, p->size);
}

void unpackArrayPrimitive(ArrayPrimitive **p, Buffer *buffer) {
    int8_t wtype = decodeI8(buffer);
    int16_t nameLength = decodeI16(buffer);

    char *name = (char *)malloc(nameLength * sizeof(char));
    for(int16_t i = 0; i < nameLength; i++)
        name[i] = (char)decodeI8(buffer);
    name[nameLength] = '\0';

    int8_t ptype = decodeI8(buffer);
    int32_t arrayLength = decodeI32(buffer);

    void *data;
    switch (ptype) {
        case I8:
            data = malloc(arrayLength * getTypeSize(I8));

            for(int32_t k = 0; k < arrayLength; k++)
                *((int8_t *)data + k) = decodeI8(buffer);

            break;
        case I16:
            data = malloc(arrayLength * getTypeSize(I16));

            for(int32_t k = 0; k < arrayLength; k++)
                *((int16_t *)data + k) = decodeI16(buffer);

            break;
        case I32:
            data = malloc(arrayLength * getTypeSize(I32));

            for(int32_t k = 0; k < arrayLength; k++)
                *((int32_t *)data + k) = decodeI32(buffer);

            break;
        case I64:
            data = malloc(arrayLength * getTypeSize(I64));

            for(int32_t k = 0; k < arrayLength; k++)
                *((int64_t *)data + k) = decodeI64(buffer);

            break;
        case FLOAT:
            data = malloc(arrayLength * getTypeSize(FLOAT));

            for(int32_t k = 0; k < arrayLength; k++)
                *((float *)data + k) = decodeFloat(buffer);

            break;
        case DOUBLE:
            data = malloc(arrayLength * getTypeSize(DOUBLE));

            for(int32_t k = 0; k < arrayLength; k++)
                *((double *)data + k) = decodeDouble(buffer);

            break;
        default: exit(EXIT_FAILURE);
    }

    createArrayPrimitive(p, ptype, arrayLength, name, data);
    int32_t size = decodeI32(buffer);
    free(data);

    assert(size == (*p)->size);
    assert(nameLength == (*p)->nameLength);
    assert(arrayLength == (*p)->arrayLength);
}

void printArrayPrimitive(const ArrayPrimitive *p) {
    printf("ArrayPrimitive [%s]:\n", p->name);
    printf("\t int8_t wtype = %d\n", p->wtype);
    printf("\t int16_t nameLength = %d\n", p->nameLength);
    printf("\t char *name = %s\n", p->name);
    printf("\t int8_t ptype = %d\n", p->ptype);
    printf("\t int32_t arrayLength = %d\n", p->arrayLength);

    switch (p->ptype) {
        case I8:
            printf("\t int8_t data[%d] = { ", p->arrayLength);
            if(p->arrayLength == 0) printf("empty ");

            for(int32_t i = 0; i < p->arrayLength; i++)
                printf("%d ", *((int8_t *)p->data + i));

            printf("}\n");
            break;
        case I16:
            printf("\t int16_t data[%d] = { ", p->arrayLength);
            if(p->arrayLength == 0) printf("empty ");

            for(int32_t i = 0; i < p->arrayLength; i++)
                printf("%hd ", *((int16_t *)p->data + i));

            printf("}\n");
            break;
        case I32:
            printf("\t int32_t data[%d] = { ", p->arrayLength);
            if(p->arrayLength == 0) printf("empty ");

            for(int32_t i = 0; i < p->arrayLength; i++)
                printf("%d ", *((int32_t *)p->data + i));

            printf("}\n");
            break;
        case I64:
            printf("\t int64_t data[%d] = { ", p->arrayLength);
            if(p->arrayLength == 0) printf("empty ");

            for(int32_t i = 0; i < p->arrayLength; i++)
                printf("%ld ", *((int64_t *)p->data + i));

            printf("}\n");
            break;
        case FLOAT:
            printf("\t float data[%d] = { ", p->arrayLength);
            if(p->arrayLength == 0) printf("empty ");

            for(int32_t i = 0; i < p->arrayLength; i++)
                printf("%f ", *((float *)p->data + i));

            printf("}\n");
            break;
        case DOUBLE:
            printf("\t double data[%d] = { ", p->arrayLength);
            if(p->arrayLength == 0) printf("empty ");

            for(int32_t i = 0; i < p->arrayLength; i++)
                printf("%lf ", *((double *)p->data + i));

            printf("}\n");
            break;
        default: exit(EXIT_FAILURE);
    }

    printf("\t int32_t size = %d\n", p->size);
}
