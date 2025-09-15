#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define BUF_SIZE 1024
#define MAX_CLIENTS 5

int clients[MAX_CLIENTS];
int client_count = 0;

void log_clients(){
    if (client_count == 0) {
        printf("[]\n");
        return;
    }

    if (client_count == 1) {
        printf("[ %d ]\n", clients[0]); 
        return;
    }

    printf("clients: [ %d", clients[0]);
    for (int i=1; i<client_count; i++) {
        printf(", %d", clients[i]);
    }
    printf(" ]\n");

    return;
}

int main(){
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t server_addrlen = sizeof(server_addr);

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

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addrlen = sizeof(client_addr);

        int *clientfd = malloc(sizeof(int));
        *clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addrlen);  
        if (*clientfd < 0) {
            perror("accept FAILED\n");
            free(clientfd);
            exit(EXIT_FAILURE);
        }
        printf("accepted connection from: %s\n", inet_ntoa(client_addr.sin_addr));

        if (client_count < MAX_CLIENTS) {
            clients[client_count++] = *clientfd;
        }

        log_clients();
    }

    close(sockfd);
    return 0;
}
