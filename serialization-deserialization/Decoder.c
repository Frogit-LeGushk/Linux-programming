#include "Decoder.h"

int8_t decodeI8(Buffer *buffer) {
    assert(buffer->size - buffer->top >= getTypeSize(I8));

    /*
        We'll be realize solution via bit-mask.

        if value 8-bit, no have problem, cause the buffer same is 8-bit;

        hence we don't need bit-shift <=> (<< 0)
    */

    int8_t result = 0;
    result = result | (((int8_t)buffer->buf[buffer->top++]) << 0);

    return result;
}

int16_t decodeI16(Buffer *buffer) {
    assert(buffer->size - buffer->top >= getTypeSize(I16));

    /*
        In case more then 8-bit number we can go two way:
            1) By iterative, via bit-mask and bit-shift will write 8-bit part in 16-bit result number
            2) we do dinamic cast buffer to 16-bit pointer (in our case) and wrire at one time result
               , then add buffer->top as: [buffer->top += sizeof(int16_t);]

        we use first case, cause this variant can do in cikle
        (but in our example for clarity we realize he in manually)
    */

    /*
        We use for writing into bit-mask operator "|" <-- logical "or"
    */

    /*
        How work translation 8-bit number in buffer to 16-bit number?

        we have 8-bit number = ((1 << 8) - 1) <=> 0b11111111
            --translate to 16-bit-->
        we got: 0b00000000 11111111

        that is, zeros were added for high 8-bit
    */

    int16_t result = 0;
    result = result | (((int16_t)buffer->buf[buffer->top++]) << 8);
    result = result | (((int16_t)buffer->buf[buffer->top++]) << 0);

    return result;
}

int32_t decodeI32(Buffer *buffer) {
    assert(buffer->size - buffer->top >= getTypeSize(I32));
    /*
        By analogy on example describe above do next decode functions
    */

    int32_t result = 0;
    result = result | (((int32_t)buffer->buf[buffer->top++]) << 24);
    result = result | (((int32_t)buffer->buf[buffer->top++]) << 16);
    result = result | (((int32_t)buffer->buf[buffer->top++]) << 8);
    result = result | (((int32_t)buffer->buf[buffer->top++]) << 0);

    return result;
}

int64_t decodeI64(Buffer *buffer) {
    assert(buffer->size - buffer->top >= getTypeSize(I64));

    int64_t result = 0;
    result = result | (((int64_t)buffer->buf[buffer->top++]) << 56);
    result = result | (((int64_t)buffer->buf[buffer->top++]) << 48);
    result = result | (((int64_t)buffer->buf[buffer->top++]) << 40);
    result = result | (((int64_t)buffer->buf[buffer->top++]) << 32);
    result = result | (((int64_t)buffer->buf[buffer->top++]) << 24);
    result = result | (((int64_t)buffer->buf[buffer->top++]) << 16);
    result = result | (((int64_t)buffer->buf[buffer->top++]) << 8);
    result = result | (((int64_t)buffer->buf[buffer->top++]) << 0);

    return result;
}

float decodeFloat(Buffer *buffer) {
    assert(getTypeSize(I32) == getTypeSize(FLOAT));

    int32_t value = decodeI32(buffer);
    return *(float *)(&value);
}

double decodeDouble(Buffer *buffer) {
    assert(getTypeSize(I64) == getTypeSize(DOUBLE));

    int64_t value = decodeI64(buffer);
    return *(double *)(&value);
}
