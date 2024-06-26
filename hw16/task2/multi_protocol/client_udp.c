#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFLEN 10
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 54321
#define CLIENT_MSG "HI!"

int main()
{
    int sock;
    struct sockaddr_in servAddr;
    char buf[BUFLEN];

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    servAddr.sin_port = htons(SERVER_PORT);

    for (int i = 0; i < 10; i++)
    {
        if (sendto(sock, CLIENT_MSG, strlen(CLIENT_MSG), 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
        {
            perror("Error sending message");
            exit(1);
        }

        socklen_t addrLen = sizeof(servAddr);
        if (recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *)&servAddr, &addrLen) <= 0)
        {
            perror("Error receiving message");
            exit(1);
        }

        printf("Received message from server: %s\n", buf);
        sleep(2);
    }

    close(sock);

    return 0;
}
