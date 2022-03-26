#include <stdint.h>
#include <assert.h>


#include "Buffer.h"
#include "Handlers.h"


#ifndef H_DECODER
#define H_DECODER


int8_t decodeI8(Buffer *buffer);
int16_t decodeI16(Buffer *buffer);
int32_t decodeI32(Buffer *buffer);
int64_t decodeI64(Buffer *buffer);
float decodeFloat(Buffer *buffer);
double decodeDouble(Buffer *buffer);

#endif
