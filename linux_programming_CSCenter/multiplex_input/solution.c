#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/select.h>


long int sum = 0;
int proccessFifo(int fd) {
    int length = 4096;
    char buffer[length + 2];

    int bytes = read(fd, buffer, length - 1);
    buffer[bytes] = '\0';

    //if(bytes != 0)
    //    sum += atoi(buffer);

    int i;
    for(i = 0; i < bytes; i++)
        if(buffer[i] >= '0' && buffer[i] <= '9')
            sum += (int)buffer[i] - (int)'0';

    //printf("Number=%ld\n", sum);

    return bytes;
}


int main(void) {
    int fd1 = open("in1", O_RDONLY);
    int fd2 = open("in2", O_RDONLY);


    if(fd1 == -1 || fd2 == -1) {
        perror("syscall open");
        exit(errno);
    }


    fd_set read_set;
    int size_select = fd2 + 2;

    bool fd1IsWork = true;
    bool fd2IsWork = true;

    while(fd1IsWork || fd2IsWork) {
        int length = 256;
        char buffer[length + 2];


        FD_ZERO(&read_set);
        FD_SET(fd1, &read_set);
        FD_SET(fd2, &read_set);


        //if(fd1IsWork)
         //   if(0 == read(fd1, buffer, 1))
        //        fd1IsWork = false;
        //if(fd2IsWork)
        //    if(0 == read(fd2, buffer, 1))
        //        fd2IsWork = false;


        if(!fd1IsWork)
            FD_CLR(fd1, &read_set);
        if(!fd2IsWork)
            FD_CLR(fd2, &read_set);
        if(!fd1IsWork && !fd2IsWork)
            break;


        int result = select(size_select, &read_set, NULL, NULL, NULL);
        if(result == 0 || result == -1)
            break;


        if(FD_ISSET(fd1, &read_set))
        {
            if(0 == proccessFifo(fd1))
                fd1IsWork = false;
        }
        if(FD_ISSET(fd2, &read_set))
        {
            if(0 == proccessFifo(fd2))
                fd2IsWork = false;
        }
    };


    printf("%ld\n", sum);
    return 0;
}
