#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define BUFLEN 10
#define UPD "S!"
#define SERVER_PORT 12345

int main()
{
    int sockMain, length, msgLength;
    struct sockaddr_in servAddr, clientAddr;
    char buf[BUFLEN];

    if ((sockMain = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
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

    length = sizeof(clientAddr);
    bzero(buf, sizeof(BUFLEN));
    while (1)
    {
        if ((msgLength = recvfrom(sockMain, buf, BUFLEN, 0, (struct sockaddr *restrict)&clientAddr, &length)) < 0)
        {
            perror("Error receiving message");
            exit(1);
        }
        printf("Received message from client: %s\n", buf);
        strcat(buf, UPD);
        sendto(sockMain, buf, BUFLEN, 0, (const struct sockaddr *)&clientAddr, sizeof(clientAddr));
    }

    close(sockMain);
}
