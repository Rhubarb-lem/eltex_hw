#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>

#define BUFLEN 10
#define SMSG "Hello!"
#define SERVER_PORT 12345

int main()
{
    int sockMain, newSock, length, msgLength;
    struct sockaddr_in servAddr, clientAddr;
    char buf[BUFLEN];


    if ((sockMain = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERVER_PORT);

    if (bind(sockMain, (const struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("Error binding");
        exit(1);
    }

    if (listen(sockMain, 5) < 0)
    {
        perror("Error listening");
        exit(1);
    }

    length = sizeof(clientAddr);

    if ((newSock = accept(sockMain, (struct sockaddr *)&clientAddr, &length)) < 0)
    {
        perror("Error accepting connection");
        exit(1);
    }

    if ((msgLength = recv(newSock, buf, BUFLEN, 0)) < 0)
    {
        perror("Error receiving message");
        exit(1);
    }

    if (send(newSock, SMSG, sizeof(SMSG), 0) < 0)
    {
        perror("Error sending message");
        exit(1);
    }

    printf("Received message from client: %s\n", buf);

    close(newSock);
    close(sockMain);

    return 0;
}
