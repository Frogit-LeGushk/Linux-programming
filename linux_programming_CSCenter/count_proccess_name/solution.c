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

void readName(char *string, char *buffer) {
    int i = 0;
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

void readNameProccess(pid_t child_pid, char *str_name) {
    char *PATH = createPathToStatus(child_pid);
    int fd = openFile(PATH);

    if(-1 == fd)
        return;

    const int str_buffer_len = 1024;
    char str_buffer[str_buffer_len];

    read(fd, (void *)str_buffer, (size_t)str_buffer_len);
    readName(str_buffer, str_name);
    close(fd);
    return;
}

void compareGCString(char *str1, char *str2) {
    int i;
    int l1 = strlen(str1);
    int l2 = strlen(str2);
    int isCollision = true;

    if(l1 != l2) isCollision = false;

    if(isCollision) {
        for(i = 0; i < l1; i++)
            if(str1[i] != str2[i])
                isCollision = false;
    }

    if(isCollision)
        GLOBAL_COUNTER++;
}

void printfResultCompare(char *strName, char *strTemplate) {
    if(OUTPUT_MESSAGES) {
        printf("{str_name: %s, template: %s, GC: %d}\n", strName, strTemplate, GLOBAL_COUNTER);
    }
}

bool isStrIsNumber(char *str) {
    int i;
    bool isNumber = true;

    for(i = 0; i < strlen(str); i++)
        if(str[i] < '0' || str[i] > '9')
            isNumber = false;

    return isNumber;
}

DIR *openDir(void) {
    char *DIR_NAME = "/proc";
    DIR * dp = opendir(DIR_NAME);
    if(NULL == dp) {
        perror("syscall opendir");
        printf("castom message: %s\n", strerror(errno));
        exit(errno);
    }

    return dp;
}



int main(void) {
    int str_name_len = 1024;
    char str_name[str_name_len];
    char *TEMPLATE_NAME = "genenv";


    DIR * dp = openDir();

    struct dirent *SDIR = readdir(dp);
    while ((SDIR = readdir(dp)) != NULL) {
        if(isStrIsNumber(SDIR->d_name))
        {
            pid_t pid = (pid_t)atoi(SDIR->d_name);
            if(OUTPUT_MESSAGES) printf("pid=%d\n", pid);
            readNameProccess(pid, str_name);
            compareGCString(str_name, TEMPLATE_NAME);
            printfResultCompare(str_name, TEMPLATE_NAME);
        }
    }
    closedir(dp);
    printf("%d\n", GLOBAL_COUNTER);
    return 0;
}
