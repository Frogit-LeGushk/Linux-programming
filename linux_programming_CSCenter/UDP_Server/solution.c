#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
 * LIBS FOR SOCKETS
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>



bool alert = false;
struct sockaddr_in local;


void server(int port) {
    if(alert) printf("Got port=%d\n", port);

    int ss;
    if((ss = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        return;
    }

    if(alert) printf("soket fd=%d\n", ss);


    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(port);
    local.sin_family = AF_INET;


    int result = bind(ss, (struct sockaddr *)(&local), sizeof(local));
    if(alert)printf("bind result=%d\n", result);


    int BUF_SIZE = (1 << 7);
    char BUF_MESSAGE[BUF_SIZE];
    int bytes = read(ss, BUF_MESSAGE, BUF_SIZE);
    BUF_MESSAGE[bytes] = '\0';


    if(strncmp(BUF_MESSAGE, "OFF\n", 4) == 0) return;


    printf("%s\n", BUF_MESSAGE);
    close(ss);

    server(port);
}


int main(int c, char **cv) {
    if(c != 2) {
        printf("Cmd line args: <int port>\n");
        return 1;
    }

    server(atoi(cv[1]));
    return 0;
}
