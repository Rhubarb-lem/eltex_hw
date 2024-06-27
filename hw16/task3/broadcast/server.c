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
#define BROADCAST_ADDR "255.255.255.255"


int main()
{
    int sockMain, broadcastEnable = 1;
    struct sockaddr_in broadcastAddr;

    if ((sockMain = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Error opening socket");
        exit(1);
    }

    if (setsockopt(sockMain, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0)
    {
        perror("Error setting socket option");
        close(sockMain);
        exit(1);
    }

    bzero((char *)&broadcastAddr, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = inet_addr(BROADCAST_ADDR);
    broadcastAddr.sin_port = htons(SERVER_PORT);


    while (1)
    {
        if (sendto(sockMain, SMSG, strlen(SMSG), 0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) < 0)
        {
            perror("Error sending broadcast message");
        }
        printf("Broadcast message sent: %s\n", SMSG);
        sleep(5);
    }

    close(sockMain);
    return 0;
}
