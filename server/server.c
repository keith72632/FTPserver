#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<ctype.h>
#define PORT 2000

int socketCreate() {
    int hsocket = -1;
    hsocket = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket created\n");
    return hsocket;
}

int bindSocket(int hsocket) {
    short iRetval = -1;
    struct sockaddr_in remote = {0};
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = htons(PORT);
    iRetval = bind(hsocket, (struct sockaddr *)&remote, sizeof(remote));
    return iRetval;
}

int main(int argc, char * argv[]) {
    int sock_desc = 0, sock = 0, clientLen = 0;
    struct sockaddr_in client;
    int message;
    int client_message;
    int server_message;

    sock_desc = socketCreate();

    if(sock_desc < 0) {
        printf("Socket could not be created\n");
        return 1;
    }

    if(bindSocket(sock_desc) < 0) {
        printf("Socket could not be bound to port %d\n", PORT);
        return 1;
    }

    listen(sock_desc, 3);

    while(1) {
        clientLen = sizeof(struct sockaddr_in);

        sock = accept(sock_desc, (struct sockaddr *)&client, (socklen_t *)&clientLen);

        if(sock < 0) {
            printf("Socket accept failed\n");
             return 1;
        }

        printf("Connection Accepted");

        if(isnumber(recv(sock, (int *)client_message, 200, 0))) {
            client_message ++;
            send(sock, client_message, 200, 0);
        }
    }
    close(sock);
    return 0;
}