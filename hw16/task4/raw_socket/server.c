#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define BUFFER_SIZE 50

int main()
{
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in src_addr;
    socklen_t addr_len = sizeof(src_addr);

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        exit(1);
    }

    while (1)
    {
        ssize_t packet_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&src_addr, &addr_len);
        if (packet_len < 0)
        {
            perror("Error receiving packet");
            exit(1);
        }
        printf("%s\n", buffer);
    }

    close(sockfd);
    return 0;
}
