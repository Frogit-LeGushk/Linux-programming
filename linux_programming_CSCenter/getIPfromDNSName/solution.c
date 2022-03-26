#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>


bool alertHandleMessages = false;
struct hostent *h;


int main(int c, char **v) {
    if(c != 2) return 1;

    char *DNS_NAME = v[1];
    h = gethostbyname(DNS_NAME);

    if(h == NULL) {
        perror("gethostbyname");
        return 1;
    }


    if(alertHandleMessages) {
        printf("Canonical hostname: %s\n", h->h_name);
        printf("Type: %s, Length: %d\n", (h->h_addrtype == AF_INET) ? "IPv4" : "IPv6", h->h_length);
    }


    int i = 0;
    while(NULL != h->h_addr_list[i]) {
        struct in_addr *naddt = (struct in_addr *)h->h_addr_list[i];

        // transform NEWORK forrmat --to--> ASKII format
        printf("%s\n", inet_ntoa(*naddt));
        i++;
    }


    return 0;
}
