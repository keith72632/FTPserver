#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define PORT 2000

int socketCreate()
{
    int hsocket = -1;
    hsocket = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket created/n");
    return hsocket;
}

int socketConnect(int hsocket)
{
    int iRetval = -1;
    struct sockaddr_in remote = {0};
    remote.sin_addr.s_addr = inet_addr("127.0.0.1");
    remote.sin_family = AF_INET;
    remote.sin_port = htons(PORT);
    iRetval = connect(hsocket, (struct sockaddr *)&remote, sizeof(remote));
    return iRetval;
}

int socketSend(int hsocket, char *Rqst, int lenRqst)
{
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20;
    tv.tv_usec = 0;

    if (setsockopt(hsocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv)) < 0)
    {
        printf("Time out\n");
        return -1;
    }

    shortRetval = send(hsocket, Rqst, lenRqst, 0);
    return shortRetval;
}

int socketRecv(int hsocket, char *Rsp, int lenRsp)
{
    int iRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20;
    tv.tv_usec = 0;

    if (setsockopt(hsocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv)) < 0)
    {
        printf("Reception Time Out\n");
        return -1;
    }

    iRetval = recv(hsocket, Rsp, lenRsp, 0);
    return iRetval;
}

int main(int argc, char *argv[])
{
    int hsocket = 0;
    char message[100];

    hsocket = socketCreate();

    if (hsocket == -1)
    {
        printf("socket creation failed on port %d\n", PORT);
        return 1;
    }
    printf("Socket Succressfully created on port %d\n", PORT);

    if (socketConnect(hsocket) < 0)
    {
        printf("Socket not able to connect to port %d\n", PORT);
        return 1;
    }
    printf("Socket connection successful on port %d\n", PORT);

    while (1)
    {
        printf("Enter message: ");
        fgets(message, 100, stdin);
        if (strcmp(message, "0") == 0)
        {
            printf("quitting...\n");
            break;
        }
        printf("message: %s\n", message);

        socketSend(hsocket, message, strlen(message));
        printf("Message sent successfully\n");
        remove(message);
    }

    close(hsocket);
    return 0;
}