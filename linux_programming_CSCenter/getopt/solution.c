#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int c, char **v) {
    int opindex = 0;
    int opchar = 0;
    int isCorrect = 1;
    int arg = 0;

    struct option opts[] = {
        {"query", required_argument, 0, 'q'},
        {"longinformationrequest", no_argument, 0, 'i'},
        {"version", no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };


    while(-1 != (opchar = getopt_long(c, v, "qlv:", opts, &opindex)))
    {
        switch(opchar) {
            case 'q':
                break;
            case 'i':
                break;
            case 'v':
                break;
            default:
                isCorrect = 0;
                break;
        }
    }




    if(isCorrect)
        printf("+\n");
    else
        printf("-\n");

    return 0;
}
