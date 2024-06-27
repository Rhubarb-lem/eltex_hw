#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <string.h>

#define BUFLEN 10
#define MULTICAST_PORT 12345
#define MULTICAST_ADDR "239.255.255.250"

int main()
{
    int sock;
    struct sockaddr_in localAddr;
    struct ip_mreq group;
    char buf[BUFLEN];

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char *)&localAddr, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(MULTICAST_PORT);

    if (bind(sock, (const struct sockaddr *)&localAddr, sizeof(localAddr)) < 0)
    {
        perror("Error binding");
        close(sock);
        exit(1);
    }

    group.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
    group.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0)
    {
        perror("Error joining multicast group");
        close(sock);
        exit(1);
    }

    while (1)
    {
        socklen_t len = sizeof(localAddr);
        bzero(buf, BUFLEN);
        if (recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *)&localAddr, &len) < 0)
        {
            perror("Error receiving");
            close(sock);
            exit(1);
        }

        printf("Received multicast message: %s\n", buf);
    }

    close(sock);
    return 0;
}
