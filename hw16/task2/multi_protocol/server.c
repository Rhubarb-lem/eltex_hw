#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUFLEN 10
#define SERVER_PORT_TCP 12345
#define SERVER_PORT_UDP 54321
#define REPLY_MSG "HELLO!"  

void* TCPClientHandler(void* arg)
{
    int sockClient = *((int*)arg);
    char buf[BUFLEN];
    int msgLength;

    for (int i = 0; i < 10; i++) 
    {
        if ((msgLength = recv(sockClient, buf, BUFLEN, 0)) <= 0)
        {
            perror("Error receiving message");
            close(sockClient);
            return NULL;
        }

        printf("Received TCP message from client: %s\n", buf);

        if (send(sockClient, REPLY_MSG, strlen(REPLY_MSG), 0) < 0)
        {
            perror("Error sending message");
            close(sockClient);
            return NULL;
        }

        printf("Sended TCP message to client: %s\n", REPLY_MSG);
        sleep(2);
    }

    close(sockClient);
    return NULL;
}

void* UDPClientHandler(void* arg)
{
    int sockUDP = *((int*)arg);
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    char buf[BUFLEN];
    int msgLength;

    for (int i = 0; i < 10; i++) 
    {
        if ((msgLength = recvfrom(sockUDP, buf, BUFLEN, 0, (struct sockaddr*)&clientAddr, &clientLen)) < 0)
        {
            perror("Error receiving UDP message");
            close(sockUDP);
            return NULL;
        }

        printf("Received UDP message from client: %s\n", buf);

        if (sendto(sockUDP, REPLY_MSG, strlen(REPLY_MSG), 0, (struct sockaddr*)&clientAddr, clientLen) < 0)
        {
            perror("Error sending UDP message");
            close(sockUDP);
            return NULL;
        }

        printf("Sended UDP message to client: %s\n", REPLY_MSG);
        sleep(2);
    }

    close(sockUDP);
    return NULL;
}

int main()
{
    int sockTCP, sockUDP;
    struct sockaddr_in servAddrTCP, servAddrUDP;
    pthread_t tidTCP, tidUDP;
    char buf[BUFLEN];
    int opt = 1;


    if ((sockTCP = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error creating TCP socket");
        exit(1);
    }


    if (setsockopt(sockTCP, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("Error setting TCP socket option");
        exit(1);
    }


    bzero((char *)&servAddrTCP, sizeof(servAddrTCP));
    servAddrTCP.sin_family = AF_INET;
    servAddrTCP.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddrTCP.sin_port = htons(SERVER_PORT_TCP);

    if (bind(sockTCP, (struct sockaddr*)&servAddrTCP, sizeof(servAddrTCP)) < 0)
    {
        perror("Error binding TCP socket");
        exit(1);
    }


    if (listen(sockTCP, 5) < 0)
    {
        perror("Error listening TCP socket");
        exit(1);
    }


    if ((sockUDP = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Error creating UDP socket");
        exit(1);
    }

    // Allow socket reuse
    if (setsockopt(sockUDP, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("Error setting UDP socket option");
        exit(1);
    }

    // Bind UDP socket
    bzero((char *)&servAddrUDP, sizeof(servAddrUDP));
    servAddrUDP.sin_family = AF_INET;
    servAddrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddrUDP.sin_port = htons(SERVER_PORT_UDP);

    if (bind(sockUDP, (struct sockaddr*)&servAddrUDP, sizeof(servAddrUDP)) < 0)
    {
        perror("Error binding UDP socket");
        exit(1);
    }


    for (;;)
    {
        int sockClientTCP, sockClientUDP;
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);

        if (recvfrom(sockUDP, buf, BUFLEN, 0, (struct sockaddr*)&clientAddr, &clientLen) < 0)
        {
            perror("Error receiving UDP message");
            close(sockUDP);
            exit(1);
        }

        pthread_create(&tidUDP, NULL, UDPClientHandler, (void*)&sockUDP);
        pthread_detach(tidUDP);


        if ((sockClientTCP = accept(sockTCP, (struct sockaddr*)&clientAddr, &clientLen)) < 0)
        {
            perror("Error accepting TCP connection");
            exit(1);
        }

        pthread_create(&tidTCP, NULL, TCPClientHandler, (void*)&sockClientTCP);
        pthread_detach(tidTCP);

        
    }

    close(sockTCP);
    close(sockUDP);
    return 0;
}
