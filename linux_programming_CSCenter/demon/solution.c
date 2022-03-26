#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#define CHILD_PROCCESS 0


int main(void) {
    pid_t fork_pid = fork();


    if(CHILD_PROCCESS == fork_pid) {
        chdir("/");
        setsid();
        //pid_t sid = setsid();
        pid_t pid = getpid();
        //printf("sid=%d; pid=%d\n", sid, pid);
        printf("%d\n", (int)pid);


        fclose(stdin);
        fclose(stdout);
        fclose(stderr);

        while(1) {};
    }

    return 0;
}
