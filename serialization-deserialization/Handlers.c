#include "Handlers.h"


size_t getTypeSize(const PrimitiveTypes type) {
    switch (type) {
        case I8    :    return sizeof(int8_t); // unsigned integer types can be done by analoge
        case I16   :    return sizeof(int16_t);
        case I32   :    return sizeof(int32_t);
        case I64   :    return sizeof(int64_t);

        case FLOAT :    return sizeof(float);
        case DOUBLE:    return sizeof(double);
        default    :    exit(EXIT_FAILURE);
    }
}

bool isLittleEndian(void) {
    assert(sizeof(unsigned int) == 4);

    unsigned int number32bit = (1 << 8) - 1;
    /*
        number = 255 (32-bit <=> unsigned int type) representing on memory like as:
            case LittleEndian: 0x00 0x00 0x00 0b11111111;
            case BigEndian   : 0b11111111 0x00 0x00 0x00;

        0x -> hexadecimal representing
        0b -> binary representing
    */

    unsigned int shiftLeft_8 = number32bit << 8;
    /*
        in case LittleEndian get: 0x00 0x00 0b11111111 0x00;  != 0
        int case BigEndian get  : 0x00 0x00 0x00 0x00;        == 0
    */

    if(shiftLeft_8 << 8 != 0) return true;
    else return false;
}


size_t getSizeFile(const char *path) {
    FILE *fp = fopen(path, "r");
    assert(fp);
    assert(!fseek(fp, 0L, SEEK_END));

    return ftell(fp);
}






