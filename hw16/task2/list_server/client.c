#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFLEN 10
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define CLIENT_MSG "HI!"

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

    if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("Error conecting");
        exit(1);
    }


    for (int i = 0; i < 10; i++)
    {
        if (send(sock, CLIENT_MSG, strlen(CLIENT_MSG), 0) < 0)
        {
            perror("Error sending message");
            exit(1);
        }

        if (recv(sock, buf, BUFLEN, 0) <= 0)
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
