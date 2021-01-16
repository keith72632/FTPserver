#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#define PORT 2000

int socketCreate()
{
    int hsocket = -1;
    hsocket = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket created\n");
    return hsocket;
}

int bindSocket(int hsocket)
{
    short iRetval = -1;
    struct sockaddr_in remote = {0};
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = htons(PORT);
    iRetval = bind(hsocket, (struct sockaddr *)&remote, sizeof(remote));
    return iRetval;
}

int main(int argc, char *argv[])
{
    int sock_desc = 0, sock = 0, clientLen = 0;
    struct sockaddr_in client;
    char client_message[200];
    char message[100];

    sock_desc = socketCreate();

    if (sock_desc == -1)
    {
        printf("Socket could not be created\n");
        return 1;
    }

    if (bindSocket(sock_desc) < 0)
    {
        printf("Socket could not be bound to port %d\n", PORT);
        return 1;
    }
    printf("Socket bind successful\n");

    //listening for 3 people
    listen(sock_desc, 3);
    printf("Listening on port %d\n", PORT);

    while (1)
    {
        clientLen = sizeof(struct sockaddr_in);

        sock = accept(sock_desc, (struct sockaddr *)&client, (socklen_t *)&clientLen);

        if (sock < 0)
        {
            printf("Socket accept failed\n");
            break;
        }
        printf("Socket accepted\n");

        printf("Connection Accepted\n");
        memset(client_message, '\0', sizeof(client_message));
        memset(message, '\0', sizeof(message));

        if (recv(sock, client_message, 200, 0) < 0)
        {
            printf("Could not recieve\n");
            return 1;
        }
        printf("Recieved from client: %s\n", client_message);

        //switch string to number
        //int i = atoi(client_message);
        //printf("Client message before increment: %d\n", i);
        //i++;
        //printf("Client message after increment: %d\n", i);

        ////convert int i back to string
        //sprintf(message, "%d", i);
        //printf("Server message: %s\n", message);
        strcpy(message, client_message);
        printf("%s\n", message);
        close(sock);

        printf("Waiting for another connection...\n");

        sock = accept(sock_desc, (struct sockaddr *)&client, (socklen_t *)&clientLen);

        if (sock < 0)
        {
            printf("Socket accept failed\n");
            break;
        }

        if (send(sock, message, strlen(message), 0) < 0)
        {
            printf("Message could not be sent to client\n");
            return 1;
        }

        printf("Message sent: %s\n", message);
        memset(message, '\0', sizeof(message));
        //strcpy(message, NULL);
        close(sock);
    }
    close(sock);
    return 0;
}