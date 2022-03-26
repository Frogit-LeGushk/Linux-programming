#include "Buffer.h"
#include "Handlers.h"
#include "Primitive.h"
#include "Encoder.h"
#include "ArrayPrimitive.h"
#include "Struct.h"
#include "Decoder.h"


/*
    The function emulate a work of server: create some data and send them to client
    In our case data will be saved in file into FS client's
*/
void server(void) {
    /*
        Check on little-endian endianness
    */
    if(!isLittleEndian()) exit(EXIT_FAILURE);


    /*
        we wreate some entities for testing
    */
    Primitive *prim64;
    int64_t value64 = (1 << 16);
    createPrimitive(&prim64, I64, "Primitive_I64", &value64);

    Primitive *primFloat;
    float valueFloat = 3.14;
    createPrimitive(&primFloat, FLOAT, "Primitive_FLOAT", &valueFloat);

    ArrayPrimitive *arrPrim32;
    int32_t arr32[4] = {2, 4, 8, 16};
    int32_t arr32Length = 4;
    createArrayPrimitive(&arrPrim32, I32, arr32Length, "Array_primitive_I32", arr32);

    ArrayPrimitive *arrPrimDouble;
    double arrDouble[8] = {1.01, 2.02, 3.03, 4.04, 5.05, 6.06, 7.07, 8.08};
    int32_t arrDoubleLength = 4;
    createArrayPrimitive(&arrPrimDouble, DOUBLE, arrDoubleLength, "Array_ptimitive_DOUBLE", arrDouble);


    /*
        we create two struct entities, then put entries describe above into them
    */
    Struct *str1;
    createStruct(&str1, "Struct_1");

    Struct *str2;
    createStruct(&str2, "Struct_2");


    /*
        we add fields in structs and nested one struct into another
    */
    addFieldStruct(str1, prim64->wtype, prim64);
    addFieldStruct(str1, arrPrim32->wtype, arrPrim32);

    addFieldStruct(str2, primFloat->wtype, primFloat);
    addFieldStruct(str2, arrPrimDouble->wtype, arrPrimDouble);

    addFieldStruct(str1, str2->wtype, str2);


    /*
        packing struct1 into buffer1 and save in FS
    */
    Buffer *buffer;
    createBuffer(&buffer, str1->size);

    packStruct(str1, buffer);

    saveInFileBuffer(buffer, "./state/bufferStructOne.txt");

    printf("<====== [Sending data to client....] ======>\n");
    printStruct(str1);
    printf("\n");
    printBuffer(buffer);
    printf("<====== [Data have been sent successfully!] ======>\n");
}


/*
    The function get data from server, deserialize them and print
*/
void client(void) {
    /*
        we load a serialized data (storage in file, by know path) into buffer
        ,then we are unpacking them and print them
    */
    Buffer *buffer;
    createBuffer(&buffer, getSizeFile("./state/bufferStructOne.txt"));

    printf("<====== [Reading data from server....] ======>\n");
    loadFlomFileBuffer(buffer, "./state/bufferStructOne.txt");

    int8_t wtype = getWrapperTypeBuffer(buffer);
    void *data;

    /*
        we define data type, and call needed function for deserialize them
    */
    switch (wtype) {
        case PRIMITIVE:
            unpackPrimitive((Primitive **)&data, buffer);
            printPrimitive((Primitive *)data);
            break;
        case ARRAY:
            unpackArrayPrimitive((ArrayPrimitive **)&data, buffer);
            printArrayPrimitive((ArrayPrimitive *)data);
            break;
        case STRUCT:
            unpackStruct((Struct **)&data, buffer);
            printStruct((Struct *)data);
            break;
        default:
            exit(EXIT_FAILURE);
    }

    printf("\n");
    printBuffer(buffer);
    printf("<====== [Data have been got successfully!] ======>\n");
}



int main(void) {
    server();
    printf("\n");
    client();

    return 0;
}






