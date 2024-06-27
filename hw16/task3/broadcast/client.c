#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>

#define BUFLEN 10
#define SERVER_PORT 12345

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
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERVER_PORT);

    if (bind(sock, (const struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("Error binding");
        close(sock);
        exit(1);
    }


    while (1)
    {
        socklen_t len = sizeof(servAddr);
        bzero(buf, BUFLEN);
        if (recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *)&servAddr, &len) < 0)
        {
            perror("Error receiving");
            close(sock);
            exit(1);
        }

        printf("Received  message: %s\n", buf);
    }

    close(sock);
    return 0;
}
