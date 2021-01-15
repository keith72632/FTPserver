#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 2000

int socketCreate()
{
    int hsocket = -1;
    hsocket = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket created\n");
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

int socketRcv(int hsocket, char *Rsp, int lenRsp)
{
    int iRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20;
    tv.tv_usec = 0;

    if (setsockopt(hsocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)) < 0)
    {
        printf("Reception failed\n");
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
        printf("Socket creation on port %d unsuccessful\n", PORT);
        return 1;
    }
    printf("Socket created successful on port %d\n", PORT);

    if (socketConnect(hsocket) < 0)
    {
        printf("Could not connect on port %d\n", PORT);
        return 1;
    }
    printf("Socket connected on port %d\n", PORT);

    memset(message, '\0', 100);

    while (1)
    {
        socketRcv(hsocket, message, 200);
        if (strcmp(message, "0") == 0)
        {
            break;
        }
        printf("Message: %s\n", message);
        remove(message);
    }

    close(hsocket);
    return 0;
}