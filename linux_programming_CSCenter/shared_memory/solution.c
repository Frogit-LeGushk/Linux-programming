#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdbool.h>


bool alertHandlerMessages = false;


int main(int argc, char *argv[]) {
    if(3 != argc) exit(EXIT_FAILURE);
    const size_t SHMSZ = 1000;
    const int COUNT_NUMBER = 100;
    int i;

    key_t key1 = atoi(argv[1]);
    key_t key2 = atoi(argv[2]);

    if(alertHandlerMessages)
        printf("key1=%d; key2=%d; SHMSZ=%d\n", key1, key2, (int)SHMSZ);


    int id1 = shmget(key1, SHMSZ, 0666);
    int id2 = shmget(key2, SHMSZ, 0666);

    if(id1 == -1 || id2 == -1) {
        perror("syscall shmget");
        exit(EXIT_FAILURE);
    }

    if(alertHandlerMessages)
        printf("id1=%d; id2=%d\n", id1, id2);


    int *shm1 = (int *)shmat(id1, NULL, 0);
    int *shm2 = (int *)shmat(id2, NULL, 0);

    if(shm1 == (int *)-1 || shm2 == (int *)-1) {
        perror("syscall shmat");
        exit(EXIT_FAILURE);
    }


    /*
     * Create the segment.
     */
    int shmid;
    key_t key = 5678;
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    int *shm;
    if ((shm = shmat(shmid, NULL, 0)) == (int *) -1) {
        perror("shmat");
        exit(1);
    }


    for(i = 0; i < COUNT_NUMBER; i++) {
        shm[i] = shm1[i] + shm2[i];
    }

    printf("%d\n", key);


    return 0;
}
