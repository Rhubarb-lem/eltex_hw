#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define SMSG "ATTENTION!"
#define SERVER_PORT 12345
#define MULTICAST_ADDR "239.255.255.250"

int main()
{
    int sockMain;
    struct sockaddr_in multicastAddr;

    if ((sockMain = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char *)&multicastAddr, sizeof(multicastAddr));
    multicastAddr.sin_family = AF_INET;
    multicastAddr.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    multicastAddr.sin_port = htons(SERVER_PORT);


    while (1)
    {
        if (sendto(sockMain, SMSG, strlen(SMSG), 0, (struct sockaddr *)&multicastAddr, sizeof(multicastAddr)) < 0)
        {
            perror("Error sending multicast message");
        }
        printf("Multicast message sent: %s\n", SMSG);
        sleep(5);
    }

    close(sockMain);
    return 0;
}
