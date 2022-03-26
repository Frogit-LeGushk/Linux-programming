#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>



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



int main(void) {
    pid_t child_pid = getpid();
    pid_t parent_pid = getppid();

    //printf("{child_pid: %d, parent_pid: %d}\n", (int)child_pid, (int)parent_pid);

    char *PATH = createPathToStatus(child_pid);
    //printf("getting PATH=%s\n", PATH);

    int fd = open(PATH, O_RDONLY);
    if(-1 == fd) {
        perror("syscall open");
        printf("castom message: %s\n", strerror(errno));
        exit(errno);
    }

    const str_buffer_len = 1024;
    char str_buffer[str_buffer_len];

    read(fd, (void *)str_buffer, (size_t)str_buffer_len);
    //printf("Readed data:\n%s\n", str_buffer);

    int i = 0;
    int pointer_to_start = 0;
    while(pointer_to_start != 6) {
        if(str_buffer[i] == '\n')
            pointer_to_start++;
        i++;
    }

    int k = 0;
    while(str_buffer[i + k] != ':') {
        k++;
    }

    k += 2;
    while(str_buffer[i + k] != '\n') {
        printf("%c", str_buffer[i + k]);
        k++;
    }

    printf("\n");

    return 0;
}
