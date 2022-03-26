#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>


#define CHILD_PROCCESS 0


void handler(int n) {
    exit(EXIT_SUCCESS);
}

int main(void) {
    pid_t fork_pid = fork();


    if(CHILD_PROCCESS == fork_pid) {
        chdir("/");
        pid_t sid = setsid();

        if(-1 == sid) {
            perror("syscall setsid");
            exit(EXIT_FAILURE);
        }

        pid_t pid = getpid();
        printf("pid=%d, sid=%d\n", (int)pid, (int)sid);



        if(SIG_ERR == signal(SIGURG, handler)) {
            perror("syscall signal");
            exit(EXIT_FAILURE);
        }

        fclose(stdin);
        fclose(stdout);
        fclose(stderr);

        while(1) {
            sleep(10);
        };
    }

    return 0;
}
