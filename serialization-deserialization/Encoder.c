#include "Encoder.h"

void encodeI8(Buffer *buffer, int8_t value) {
    // <-- check overflow buffer -->
    assert(buffer->top + (getTypeSize(I8) / sizeof(uint8_t)) <= buffer->size);

    buffer->buf[buffer->top++] = value >> 0;
}

void encodeI16(Buffer *buffer, int16_t value) {
    assert(buffer->top + (getTypeSize(I16) / sizeof(uint8_t)) <= buffer->size);

    /*
        For example take the value = (1 << 8) - 1, then:
            value 16-bit representing on memory: 0b00000000 0b11111111

            1) We must put 0b00000000 into buffer[top];
            2)     and put 0b11111111 into buffer[top + 1];

        When we put 16-bit value in 8-bit array => high 8-bit throws away
        If we want put high 8-bit in buffer, we must done bit-shift by 8 in right (for little-endian)
    */

    buffer->buf[buffer->top++] = value >> 8;
    buffer->buf[buffer->top++] = value >> 0;
}

void encodeI32(Buffer *buffer, int32_t value) {
    assert(buffer->top + (getTypeSize(I32) / sizeof(uint8_t)) <= buffer->size);

    /*
        By analogy algoritm discribed above, we write 32-bit value in buffer
    */

    buffer->buf[buffer->top++] = value >> 24;
    buffer->buf[buffer->top++] = value >> 16;
    buffer->buf[buffer->top++] = value >> 8;
    buffer->buf[buffer->top++] = value >> 0;
}

void encodeI64(Buffer *buffer, int64_t value) {
    assert(buffer->top + (getTypeSize(I64) / sizeof(uint8_t)) <= buffer->size);

    buffer->buf[buffer->top++] = value >> 56;
    buffer->buf[buffer->top++] = value >> 48;
    buffer->buf[buffer->top++] = value >> 40;
    buffer->buf[buffer->top++] = value >> 32;
    buffer->buf[buffer->top++] = value >> 24;
    buffer->buf[buffer->top++] = value >> 16;
    buffer->buf[buffer->top++] = value >> 8;
    buffer->buf[buffer->top++] = value >> 0;
}

void encodeFloat(Buffer *buffer, float value) {
    assert(getTypeSize(I32) == getTypeSize(FLOAT));
    // decause fload <=> 32-bit number, we do cast types: float --in--> int32_t

    encodeI32(buffer, *(int32_t *)(&value));
}

void encodeDouble(Buffer *buffer, double value) {
    assert(getTypeSize(I64) == getTypeSize(DOUBLE));
    // by analogy with float we do same redirect

    encodeI64(buffer, *(int64_t *)(&value));
}
