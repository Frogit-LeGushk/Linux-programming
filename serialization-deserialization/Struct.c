#include "Struct.h"


void createStruct(Struct **p, const char *name) {
    assert(strlen(name) < 256 && sizeof(char) == getTypeSize(I8));

    *p = (Struct *)malloc(sizeof(Struct));
    assert(p);

    (*p)->wtype = STRUCT;
    (*p)->nameLength = strlen(name);

    (*p)->name = (char *)malloc(strlen(name) * sizeof(char));
    assert((*p)->name);

    for(int16_t i = 0; i < strlen(name); i++)
        (*p)->name[i] = name[i];
    (*p)->name[strlen(name)] = '\0';

    (*p)->fieldsLength = 0;
    (*p)->fields = NULL;
    (*p)->size = getTypeSize(I16) + (strlen(name) + 1) * getTypeSize(I8) + 2 * getTypeSize(I32);
}


void deleteStruct(Struct **p) {
    free((*p)->name);

    if((*p)->fields != NULL) {
        StructItem *tmpPtr = (*p)->fields;

        while(tmpPtr != NULL) {
            free(tmpPtr->field);

            StructItem *prevTmpPtr = tmpPtr;
            tmpPtr = tmpPtr->next;

            free(prevTmpPtr);
        }
    }

    free(*p);
    *p = NULL;
}

void addFieldStruct(Struct *p, int8_t wtype, void *field) {
    if(p->fields == NULL) {
        p->fields = (StructItem *)malloc(sizeof(StructItem));
        p->fields->wtype = wtype;
        p->fields->field = field;
        p->fields->next = NULL;
    }
    else {
        StructItem *tmpPtr = p->fields;
        while(tmpPtr->next != NULL)
            tmpPtr = tmpPtr->next;

        tmpPtr->next = (struct SStructItem *)malloc(sizeof(struct SStructItem));
        assert(tmpPtr->next);

        tmpPtr->next->wtype = wtype;
        tmpPtr->next->field = field;
        tmpPtr->next->next = NULL;
    }

    p->fieldsLength++;

    switch (wtype) {
        case PRIMITIVE:
            p->size += ((Primitive *)field)->size;
            break;
        case ARRAY:
            p->size += ((ArrayPrimitive *)field)->size;
            break;
        case STRUCT:
            p->size += ((Struct *)field)->size;
            break;
        default:
            exit(EXIT_FAILURE);
    }
}

void packStruct(const Struct *p, Buffer *buffer) {
    encodeI8(buffer, p->wtype);
    encodeI16(buffer, p->nameLength);

    assert(sizeof(char) == getTypeSize(I8));
    for(int16_t i = 0; i < p->nameLength; i++)
        encodeI8(buffer, p->name[i]);

    encodeI32(buffer, p->fieldsLength);


    StructItem *tmpPtr = p->fields;
    Buffer *tmpBuffer;

    while(tmpPtr != NULL) {

        switch (tmpPtr->wtype) {
            case PRIMITIVE:
                createBuffer(&tmpBuffer, ((Primitive *)tmpPtr->field)->size);
                packPrimitive(tmpPtr->field, tmpBuffer);

                for(size_t i = 0; i < tmpBuffer->size; i++)
                    buffer->buf[buffer->top++] = tmpBuffer->buf[i];

                deleteBuffer(&tmpBuffer);
                break;
            case ARRAY:
                createBuffer(&tmpBuffer, ((ArrayPrimitive *)tmpPtr->field)->size);
                packArrayPrimitive(tmpPtr->field, tmpBuffer);

                for(size_t i = 0; i < tmpBuffer->size; i++)
                    buffer->buf[buffer->top++] = tmpBuffer->buf[i];

                deleteBuffer(&tmpBuffer);
                break;
            case STRUCT:
                createBuffer(&tmpBuffer, ((Struct *)tmpPtr->field)->size);
                packStruct(tmpPtr->field, tmpBuffer);

                for(size_t i = 0; i < tmpBuffer->size; i++)
                    buffer->buf[buffer->top++] = tmpBuffer->buf[i];

                deleteBuffer(&tmpBuffer);
                break;
            default:
                exit(EXIT_FAILURE);
        }

        tmpPtr = tmpPtr->next;
    }

    encodeI32(buffer, p->size);
}


void unpackStruct(Struct **p, Buffer *buffer) {
    int8_t wtype = decodeI8(buffer);
    int16_t nameLength = decodeI16(buffer);

    char *name = (char *)malloc(nameLength * sizeof(char));
    for(int16_t i = 0; i < nameLength; i++)
        name[i] = (char)decodeI8(buffer);
    name[nameLength] = '\0';

    createStruct(p, name);
    assert(nameLength == (*p)->nameLength);


    int32_t fieldsLength = decodeI32(buffer);
    (*p)->fieldsLength = fieldsLength;


    StructItem *fields = NULL;
    if(fieldsLength > 0) {
        fields = (StructItem *)malloc(sizeof(StructItem));
        fields->next = NULL;
        assert(fields);
        (*p)->fields = fields;
    }

    while(fieldsLength > 0) {
        while(fields->next != NULL)
            fields = fields->next;

        fields->wtype = decodeI8(buffer);
        buffer->top--;


        void *field;
        switch (fields->wtype) {
            case PRIMITIVE:
                unpackPrimitive((Primitive **)(&field), buffer);
                //((Primitive *)field)->name[((Primitive *)field)->nameLength] = '\0';
                fields->field = field;
                break;
            case ARRAY:
                unpackArrayPrimitive((ArrayPrimitive **)(&field), buffer);
                fields->field = field;
                break;
            case STRUCT:
                unpackStruct((Struct **)(&field), buffer);

                fields->field = field;
                break;
            default:
                exit(EXIT_FAILURE);
        }

        fieldsLength--;

        // create StructItem container for next iteration
        if(fieldsLength > 0) {
            fields->next = (StructItem *)malloc(sizeof(StructItem));
            assert(fields->next);
            fields->next->next = NULL;
        }
    }


    // recalc size of struct
    if((*p)->fieldsLength  > 0) {
        StructItem *tmpStructItemPtr = (*p)->fields;
        int32_t fieldsSize = 0;

        while(tmpStructItemPtr != NULL) {
            switch (tmpStructItemPtr->wtype) {
                case PRIMITIVE:
                    fieldsSize += ((Primitive *)(tmpStructItemPtr->field))->size;
                    break;
                case ARRAY:
                    fieldsSize += ((ArrayPrimitive *)(tmpStructItemPtr->field))->size;
                    break;
                case STRUCT:
                    fieldsSize += ((Struct *)(tmpStructItemPtr->field))->size;
                    break;
                default:
                    exit(EXIT_FAILURE);
            }

            tmpStructItemPtr = tmpStructItemPtr->next;
        }

        int32_t size = decodeI32(buffer);
        (*p)->size += fieldsSize;
        assert(size == (*p)->size);
    }
}


void printStruct(const Struct *p) {
    printf("Struct [%s]:\n", p->name);
    printf("\t int8_t wtype = %d\n", p->wtype);
    printf("\t int16_t nameLength = %d\n", p->nameLength);
    printf("\t char *name = %s\n", p->name);
    printf("\t int32_t fieldsLength = %d\n", p->fieldsLength);


    if(p->fields != NULL) {
        printf("\t StructItem *fields: (enumeration below)\n\n");
        int32_t counter = 1;

        StructItem *tmpPtr = p->fields;
        while(tmpPtr != NULL) {
            printf("field[%d] ", counter);
            switch (tmpPtr->wtype) {
                case PRIMITIVE:
                    printPrimitive(tmpPtr->field);
                    break;
                case ARRAY:
                    printArrayPrimitive(tmpPtr->field);
                    break;
                case STRUCT:
                    printStruct(tmpPtr->field);
                    break;
                default:
                    exit(EXIT_FAILURE);
            }

            tmpPtr = tmpPtr->next;
            counter++;
            printf("\n");
        }
    }
    else {
        printf("\t StructItem *fields = null\n");
    }

    printf("\t int32_t size = %d\n", p->size);
    printf("End Struct [%s]\n", p->name);
}
