#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>

#define BUFLEN 10
#define SMSG "HI!"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

int main()
{
    int sock;
    struct sockaddr_in servAddr;
    char buf[BUFLEN];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    servAddr.sin_port = htons(SERVER_PORT);

    if (connect(sock, (const struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("Error connecting to server");
        exit(1);
    }

    if (send(sock, SMSG, BUFLEN, 0) < 0)
    {
        perror("Error sending message");
        exit(1);
    }

    if (recv(sock, buf, BUFLEN, 0) < 0)
    {
        perror("Error receiving message");
        exit(1);
    }

    printf("Received message from server: %s\n", buf);

    close(sock);

    return 0;
}
