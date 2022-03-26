#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

// FOR SOCK_ADDR Struct
#include <netinet/in.h>
#include <arpa/inet.h>



bool alert = false;
struct sockaddr_in in;


int compare(const void * x1, const void * x2)
{
    return (*(char*)x2 - *(char*)x1);
}


void server(int port) {
    if(alert) printf("port=%d\n", port);


    int ss = socket(AF_INET, SOCK_STREAM, 0);
    if(alert) printf("socket descriptot=%d\n", ss);


    inet_aton("127.0.0.1", &(in.sin_addr));
    in.sin_port = (in_port_t)htons(port);
    in.sin_family = AF_INET;


    int bind_result = bind(ss, (struct sockaddr *)(&in), sizeof(in));
    if(alert) printf("bind_result=%d\n", bind_result);


    int listen_result = listen(ss, 6);
    if(alert) printf("listen_result=%d\n", listen_result);


    int cs = accept(ss, NULL, NULL);
    if(alert) printf("cs=%d\n", cs);


    while(true) {
        char buffer[BUFSIZ];
        int readed = recv(cs, buffer, BUFSIZ, 0);


        if(alert) printf("Readed data: %s", buffer);
        if(strncmp(buffer, "OFF", 3) == 0) return;


        qsort(buffer, readed, sizeof(char), compare);
        send(cs, buffer, readed, 0);
    }


    close(cs);
    close(ss);
    return;
}


int main(int c, char **v) {
    if(c != 2) {
        printf("Input cmd args: <int port>");
        return 1;
    }

    server(atoi(v[1]));
    return 0;
}
