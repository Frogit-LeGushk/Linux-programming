## Serialization and deserialization (later S/D).
 
This project contains both S/D functions and wrappers for a handing over data's (sending as example from server and receiving on client), which save state of a some state-mashine into binary file and later it can will be readed from binary file.

If you have come to look only realization of serialization and deserialization then dont be upset. Follow to "Encoder" and "Decoder" files. They is standalone, and have many comments in order to we can understand how this work under hood.

Some about Makefile:
1) for start test example use cmd: `make`
2) If you need the light dinamic lib for S/D then you may use command: `make buildDinamicLib` 
3) if you wand to look how work test example with dinamic library, and make sure what all is work, use command: `make buildAndRunDinamicLib`
4) for cleaning object files use cmd: `make clean`

We can comfortable vieving to look what is in binary file  two methods:
1) use function `printBuffer` in your code
2) download GNOME Hex Editor and use command: `ghex [options] files...`
