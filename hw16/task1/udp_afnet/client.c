#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>

#define BUFLEN 81
#define SMSG "HI!"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

int main()
{
    int sock;
    struct sockaddr_in servAddr, clientAddr;
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

    bzero((char *)&clientAddr, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddr.sin_port = 0;

    if (bind(sock, (const struct sockaddr *)&clientAddr, sizeof(clientAddr)) < 0)
    {
        perror("Error binding");
        exit(1);
    }

    if (sendto(sock, SMSG, BUFLEN, 0, (const struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("Error sending");
        exit(1);
    }

    socklen_t len = sizeof(servAddr);
    if (recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *)&servAddr, &len) < 0)
    {
        perror("Error receiving");
        exit(1);
    }

    printf("Received message from server: %s\n", buf);

    close(sock);

    return 0;
}
