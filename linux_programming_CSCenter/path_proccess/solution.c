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

pid_t readPPidProccess(pid_t child_pid, char *str_name) {
    int NUMBER_LINE = 6;
    char *PATH = createPathToStatus(child_pid);
    int fd = openFile(PATH);

    if(-1 == fd)
        return (pid_t)1;

    const int str_buffer_len = 1024;
    char str_buffer[str_buffer_len];

    read(fd, (void *)str_buffer, (size_t)str_buffer_len);
    readPPidInLine(str_buffer, str_name, getNumberLine(str_buffer, NUMBER_LINE));
    close(fd);

    return (pid_t)atoi(str_name);
}



void printPidInNumber(pid_t pid) {
    printf("%d\n", (int)pid);
    return;
}


int main(int argc, char *argv[]) {
    int str_name_len = 1024;
    char str_name[str_name_len];


    pid_t pid = getpid();
    if(argc == 2) pid = (pid_t)atoi(argv[1]);


    printf("%d\n", pid);
    while(pid != 1) {
        pid = readPPidProccess(pid, str_name);
        printf("%d\n", pid);
    }


    return 0;
}
