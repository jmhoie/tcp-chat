#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define BUF_SIZE 1024

int main(){
    int sockfd, connfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t server_addrlen, client_addrlen;
    server_addrlen = sizeof(server_addr);
    client_addrlen = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation FAILED\n");
        exit(EXIT_FAILURE);
    }
    printf("socket creation SUCCESS\n");
    
    memset(&server_addr, 0, server_addrlen);
    server_addr.sin_family = AF_INET;                   //ipv4
    server_addr.sin_port = htons(PORT);                 //port
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);    //any interface

    if (bind(sockfd, (struct sockaddr*)&server_addr, server_addrlen) < 0) {
        perror("socket bind FAILED\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("socket bind SUCCESS\n");

    if (listen(sockfd, 10) < 0) {
        perror("listen FAILED\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("server listening on %s:%d...\n\n", inet_ntoa(server_addr.sin_addr), PORT);

    connfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addrlen);  
    if (connfd < 0) {
        perror("accept FAILED\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("accepted connection from: %s\n", inet_ntoa(client_addr.sin_addr));

    char buf[BUF_SIZE];
    memset(&buf, 0, BUF_SIZE);

    ssize_t bytes_received = read(connfd, buf, BUF_SIZE);
    printf("received %zd bytes\n", bytes_received);
    printf("from %s: %s", inet_ntoa(client_addr.sin_addr), buf);
    memset(&buf, 0, bytes_received+1);

    char *message = "server reply";
    write(connfd, message, strlen(message));

    close(connfd);
    close(sockfd);

    return 0;
}
