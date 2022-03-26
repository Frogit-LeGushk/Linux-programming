#include <stdint.h>
#include <assert.h>


#include "Buffer.h"
#include "Handlers.h"


#ifndef H_ENCODER
#define H_ENCODER


void encodeI8(Buffer *buffer, int8_t value);
void encodeI16(Buffer *buffer, int16_t value);
void encodeI32(Buffer *buffer, int32_t value);
void encodeI64(Buffer *buffer, int64_t value);
void encodeFloat(Buffer *buffer, float value);
void encodeDouble(Buffer *buffer, double value);

#endif
