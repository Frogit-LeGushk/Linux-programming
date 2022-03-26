#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define LBUF 20

void concatNameUtilitsAndParams(char *name_utilits, char *params, char buffer_to_cmd[256]) {
    int l_nu = strlen(name_utilits);
    int l_pms = strlen(params);
    int top = 0;
    int i;

    for(i = 0; i < l_nu; i++) {
        buffer_to_cmd[top] = name_utilits[i];
        top++;
    }

    buffer_to_cmd[top] = ' ';
    top++;

    for(i = 0; i < l_pms; i++) {
        buffer_to_cmd[top] = params[i];
        top++;
    }

    buffer_to_cmd[top] = '\0';
}


int main(int argc, char *argv[]) {
    char buffer_to_have_read[(1 << LBUF)];
    char buffer_to_cmd[256];
    char *name_utilits = "";
    char *params = "";
    long long int i;


    if(argc == 3) {
        name_utilits = argv[1];
        params = argv[2];
    } else {
        printf("Input cmd line: <string> <string>");
        exit(1);
    }


    concatNameUtilitsAndParams(name_utilits, params, buffer_to_cmd);
    FILE *STREAM_PIPE = popen(buffer_to_cmd, "r");
    if(NULL == STREAM_PIPE) {
        perror("popen syscall");
        exit(errno);
    }



    long long int counter = 0;
    char countingChar = '0';
    char nextChar;

    while((nextChar = (char)fgetc(STREAM_PIPE)) != EOF) {
        if(nextChar == countingChar)
            counter++;
    }



    printf("%lld\n", counter);
    pclose(STREAM_PIPE);
    return 0;
}








