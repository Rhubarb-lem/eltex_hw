#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>

#define BUFLEN 10
#define SMSG "HI!"
#define SOCKET_PATH "/tmp/uds_socket"

int main()
{
    int sock;
    struct sockaddr_un servAddr;
    char buf[BUFLEN];


    if ((sock = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0)
    {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sun_family = AF_LOCAL;
    strncpy(servAddr.sun_path, SOCKET_PATH, sizeof(servAddr.sun_path) - 1);


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

    printf("CLIENT: Received message from server: %s\n", buf);

    close(sock);

    return 0;
}
