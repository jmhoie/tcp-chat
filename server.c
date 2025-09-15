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
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* handle_client(void *arg){
    
}

int main(){
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t server_addrlen;
    server_addrlen = sizeof(server_addr);

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
        struct socakddr_in client_addr;
        socklen_t client_addrlen = sizeof(client_addr);

        int clientfd = malloc(sizeof(int));
        clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addrlen);  
        if (*clientfd < 0) {
            perror("accept FAILED\n");
            free(clientfd)
            exit(EXIT_FAILURE);
        }
        printf("accepted connection from: %s\n", inet_ntoa(client_addr.sin_addr));

        pthread_mutex_lock(&lock);
        if (client_count < MAX_CLIENTS) {
            clients[client_count++] = *clientfd
        } 
        pthread_mutex_unlock(&lock);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, clientfd);
        pthread_detach(&thread_id);
    }

    close(connfd);
    close(sockfd);

    return 0;
}
