#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 8080
#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in serv_addr;
    socklen_t serv_addrlen = sizeof(serv_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0 serv_addrlen);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
}
