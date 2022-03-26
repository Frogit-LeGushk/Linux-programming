#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sysexits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>



typedef void (*sighandler_t)(int);


int COUNT_SIGUSR1 = 0;
int COUNT_SIGUSR2 = 0;
bool alertHandlerMessages = false;


void sighandler_t_SIGUSR1(int n) {
    if(alertHandlerMessages)
        printf("SIGUSR1\n");
    COUNT_SIGUSR1++;
}

void sighandler_t_SIGUSR2(int n) {
    if(alertHandlerMessages)
        printf("SIGUSR2\n");
    COUNT_SIGUSR2++;
}

void sighandler_t_SIGTERM(int n) {
    if(alertHandlerMessages)
        printf("SIGTERM\n");
    printf("%d %d\n", COUNT_SIGUSR1, COUNT_SIGUSR2);
    exit(EX_OK);
}


int main(void) {
    if(alertHandlerMessages)
        printf("pid proccess=%d\n", (int)getpid());


    if(SIG_ERR == signal(SIGUSR1, sighandler_t_SIGUSR1)) {
        perror("signal SIGUSR1");
        exit(EX__BASE);
    }
    if(SIG_ERR == signal(SIGUSR2, sighandler_t_SIGUSR2)) {
        perror("signal SIGUSR2");
        exit(EX__BASE);
    }
    if(SIG_ERR == signal(SIGTERM, sighandler_t_SIGTERM)) {
        perror("signal SIGTERM");
        exit(EX__BASE);
    }


    while(true) {
        if(alertHandlerMessages)
            printf("Wait next signal 10s... \n");
        sleep(10);
    }


    return 0;
}
