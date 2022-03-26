#include "Buffer.h"


void createBuffer(Buffer **buffer, size_t size) {
    *buffer = (Buffer *)malloc(sizeof(Buffer));
    assert(*buffer);

    (*buffer)->buf = (uint8_t *)malloc(size * sizeof(uint8_t));
    assert((*buffer)->buf);

    (*buffer)->top = 0;
    (*buffer)->size = size;
}

void deleteBuffer(Buffer **buffer) {
    free((*buffer)->buf);
    free(*buffer);
    *buffer = NULL;
}

void saveInFileBuffer(const Buffer *buffer, const char *path) {
    FILE *fp = fopen(path, "w");
    assert(fp);

    size_t writed = fwrite(buffer->buf, sizeof(uint8_t), buffer->size, fp);
    assert(writed == buffer->size);

    fclose(fp);
}

void loadFlomFileBuffer(const Buffer *buffer, const char *path) {
    FILE *fp = fopen(path, "r");
    assert(fp);

    size_t readed = fread(buffer->buf, sizeof(uint8_t), buffer->size, fp);
    assert(readed == buffer->size);


    fclose(fp);
}

void printBuffer(const Buffer *buffer) {
    printf("Buffer:\n");
    printf("\t size_t size = %lu\n", buffer->size);

    printf("\t uint8_t *buf =");
    if(buffer->size > 0) {
        for(size_t i = 0; i < buffer->size - 1; i++)
            printf(" %02x", buffer->buf[i]);
        printf(" %02x\n", buffer->buf[buffer->size - 1]);
    }
    else {
        printf(" empty\n");
    }

    printf("\t size_t top = %lu\n", buffer->top);
}

WrapperTypes getWrapperTypeBuffer(Buffer *buffer) {
    int8_t wtype = buffer->buf[buffer->top];

    assert(wtype == PRIMITIVE || wtype == ARRAY || wtype == STRUCT);
    return wtype;
}




