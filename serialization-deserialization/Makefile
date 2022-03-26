buildAndRun: build
	gcc -o exe main.o Buffer.o Handlers.o Primitive.o Encoder.o ArrayPrimitive.o Struct.o Decoder.o && ./exe

buildAndRunDinamicLib: buildDinamicLib
	gcc ./libSD/main.c -L./libSD -lSD -o ./libSD/exe
	export LD_LIBRARY_PATH=./libSD/ && ./libSD/exe

buildDinamicLib: build
	gcc -o libSD.so -shared Buffer.o Handlers.o Primitive.o Encoder.o ArrayPrimitive.o Struct.o Decoder.o
	mv ./libSD.so ./libSD/libSD.so

build: main.o Buffer.o Handlers.o Primitive.o Encoder.o ArrayPrimitive.o Struct.o Decoder.o	

runTemplateWithDinamicLib: ./libSD/main.c 
	gcc ./libSD/main.c -L./libSD -lSD -o exe && ./libSD/exe

main.o: main.c 
	gcc -o main.o -c main.c

Buffer.o: Buffer.c Buffer.h
	gcc -o Buffer.o -c Buffer.c

Handlers.o: Handlers.c Handlers.h
	gcc -o Handlers.o -c Handlers.c

Primitive.o: Primitive.c Primitive.h
	gcc -o Primitive.o -c Primitive.c

Encoder.o: Encoder.c Encoder.h
	gcc -o Encoder.o -c Encoder.c

ArrayPrimitive.o: ArrayPrimitive.c ArrayPrimitive.h
	gcc -o ArrayPrimitive.o -c ArrayPrimitive.c

Struct.o: Struct.c Struct.h
	gcc -o Struct.o -c Struct.c

Decoder.o: Decoder.c Decoder.h
	gcc -o Decoder.o -c Decoder.c

clean: 
	-rm *.o 2>/dev/null
	-rm exe 2>/dev/null
	-rm ./libSD/exe 2>/dev/null
	-rm ./libSD/*.?o 2>/dev/null


