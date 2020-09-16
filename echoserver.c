#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
#include <string.h>
#define listenQ 5

typedef struct sockaddr SA;

int main(int argc, char **argv)
{
    int     listenfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cliaddr;
    char    addr_buff[INET_ADDRSTRLEN];
    time_t  ticks;
    char buff[1024];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons( 3000);

    if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(listenfd, listenQ) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    for ( ; ; ) {
        len = sizeof(cliaddr);
        connfd = accept(listenfd, (SA *) &cliaddr, &len);
        if (connfd == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        inet_ntop(AF_INET, &cliaddr.sin_addr, addr_buff, INET_ADDRSTRLEN);

        memset(buff, 0, sizeof(buff));
        int size = read(connfd, buff, sizeof(buff));
        if ( size < 0 ) {
            perror("read error");
            exit(5);
        }

        write(connfd, buff, strlen(buff));

        close(connfd);
    }
}
