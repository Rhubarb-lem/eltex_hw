#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>

#define BUFLEN 10
#define SMSG "Hello!"
#define SOCKET_PATH "/tmp/udp_socket"

int main()
{
    int sockMain, newSock, length, msgLength;
    struct sockaddr_un servAddr, clientAddr;
    char buf[BUFLEN];

    if ((sockMain = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0)
    {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sun_family = AF_LOCAL;
    strncpy(servAddr.sun_path, SOCKET_PATH, sizeof(servAddr.sun_path) - 1);


    unlink(SOCKET_PATH); // Delete old socket(if it exist)
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
    unlink(SOCKET_PATH); 

    return 0;
}
