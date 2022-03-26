#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <dirent.h>
#include <stdlib.h>


int GLOBAL_COUNTER = 0;
int OUTPUT_ERRORS = false;
int OUTPUT_MESSAGES = false;


char *createPathToStatus(pid_t child_pid) {
    const char *prefix = "/proc/";
    const char *suffix = "/status";
    char child_pid_str[8];
    int i;

    // number --to--> str_buffer
    sprintf(child_pid_str,"%d",child_pid);

    // allocation memoty for string
    int length = strlen(prefix) + strlen(child_pid_str) + strlen(suffix);
    char *path = (char *)malloc((length + 1) * sizeof(char));
    int pointer = 0;

    // concat prefix--child_pid_str--suffix
    for(i = 0; i < strlen(prefix); i++) {
        path[pointer] = prefix[i];
        pointer++;
    }

    for(i = 0; i < strlen(child_pid_str); i++) {
        path[pointer] = child_pid_str[i];
        pointer++;
    }

    for(i = 0; i < strlen(suffix); i++) {
        path[pointer] = suffix[i];
        pointer++;
    }

    path[pointer] = '\0';
    return path;
}

int openFile(char *PATH) {
    int fd = open(PATH, O_RDONLY);
    if(-1 == fd) {
        if(OUTPUT_ERRORS) {
            printf("ERROR_PATH=%s\n", PATH);
            perror("syscall open");
        }
        return -1;
    }

    return fd;
}

void readPPidInLine(char *string, char *buffer, int startPoint) {
    int i = startPoint;
    while(string[i] != ':') {
        i++;
    }

    i += 2;
    int top = 0;

    while(string[i] != '\n') {
        buffer[top] = string[i];
        top++;
        i++;
    }

    buffer[top] = '\0';
    return;
}

int getNumberLine(char *str, int numberLine) {
    int pointer = 0;
    int i = 0;

    while(pointer != numberLine) {
        if(str[i] == '\n')
            pointer++;
        i++;
    }

    pointer = i;
    return pointer;
}

pid_t readPPidProccess(pid_t child_pid) {
    int NUMBER_LINE = 6;
    char *PATH = createPathToStatus(child_pid);
    char str_name[254];
    int fd = openFile(PATH);

    if(-1 == fd)
        return (pid_t)0;

    const int str_buffer_len = 1024;
    char str_buffer[str_buffer_len];

    read(fd, (void *)str_buffer, (size_t)str_buffer_len);
    readPPidInLine(str_buffer, str_name, getNumberLine(str_buffer, NUMBER_LINE));
    close(fd);

    return (pid_t)atoi(str_name);
}

void calcAndPrintCounter(pid_t arr[512][1024]) {
    int i;
    int j;
    int counter = 0;

    for(i = 0; i < 512; i++)
        for(j = 1; j <= arr[i][0]; j++)
            if(arr[i][j] > 0)
                counter++;

    printf("%d\n", counter);
}

void initArrHandler(pid_t arr[512][1024], pid_t pid) {
    int i;
    int j;

    for(i = 0; i < 512; i++)
        for(j = 0; j < 1024; j++)
            arr[i][j] = 0;

    arr[0][0] = 1;
    arr[0][1] = pid;
}

DIR *openDir(void) {
    char *DIR_NAME = "/proc";
    DIR * dp = opendir(DIR_NAME);
    if(NULL == dp) {
        perror("syscall opendir");
        exit(errno);
    }

    return dp;
}

bool isStrIsNumber(char *str) {
    int i;
    bool isNumber = true;

    for(i = 0; i < strlen(str); i++)
        if(str[i] < '0' || str[i] > '9')
            isNumber = false;

    return isNumber;
}

void getAllArrPid(pid_t arrAllPid[2048]) {
    DIR *dir = openDir();
    struct dirent *SDIR = readdir(dir);

    while ((SDIR = readdir(dir)) != NULL) {
        if(isStrIsNumber(SDIR->d_name))
        {
            pid_t pid = (pid_t)atoi(SDIR->d_name);
            arrAllPid[arrAllPid[0] + 1] = pid;
            arrAllPid[0]++;
        }
    }

    closedir(dir);
}

void getAllArrPPid(pid_t arrAllPid[2048], pid_t arrAllPPid[2048]) {
    int i;
    arrAllPPid[0] = arrAllPid[0];

    for(i = 1; i <= arrAllPid[0]; i++) {
        pid_t ppid = readPPidProccess(arrAllPid[i]);
        arrAllPPid[i] = ppid;
    }
}

void calcDescendants(pid_t arrAllPid[2048], pid_t arrAllPPid[2048], pid_t arr[512][1024]) {
    int k;
    int i;
    int j;

    k = 0;
    while(1) {
        for(j = 1; j <= arr[k][0]; j++) {
            pid_t currentPid = arr[k][j];

            for(i = 1; i <= arrAllPPid[0]; i++) {
                if(arrAllPPid[i] == currentPid) {
                    arr[k + 1][arr[k + 1][0] + 1] = arrAllPid[i];
                    arr[k + 1][0] = arr[k + 1][0] + 1;
                }
            }
        }

        if(arr[k + 1][0] == 0) break;
        k++;
    }
}

void printInfo(pid_t arrAllPid[2048], pid_t arrAllPPid[2048], pid_t arr[512][1024]) {
    int i;
    int j;

    for(i = 1; i <= arrAllPid[0]; i++)
        printf("(pid=%d;ppid=%d)\n", arrAllPid[i], arrAllPPid[i]);

    int k = 0;
    while(arr[k][0] != 0) {
        for(j = 1; j <= arr[k][0]; j++)
            printf("%d ", arr[k][j]);
        printf("\n\n");
        k++;
    }
}



int main(int argc, char *argv[]) {
    int str_name_len = 1024;
    char str_name[str_name_len];
    int i;
    int j;


    pid_t arr[512][1024];
    pid_t arrAllPid[2048];
    pid_t arrAllPPid[2048];


    pid_t pid = getpid();
    if(argc == 2) pid = (pid_t)atoi(argv[1]);


    initArrHandler(arr, pid);
    for(i = 0; i < 2048; i++)
    {
        arrAllPid[i] = 0;
        arrAllPPid[i] = 0;
    }

    getAllArrPid(arrAllPid);
    getAllArrPPid(arrAllPid, arrAllPPid);
    calcDescendants(arrAllPid, arrAllPPid, arr);
    //printInfo(arrAllPid, arrAllPPid, arr);
    calcAndPrintCounter(arr);
    return 0;
}
