#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define BUFLEN 200
#define SMSG "HI!"
#define SERVER_IP "127.0.0.1"
#define SRC_IP "127.0.0.2"
#define DST_PORT 12345
#define SRC_PORT 54321

int main() {
    int sockfd;
    char buffer[BUFLEN];
    struct sockaddr_in dest_addr;

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }


    int one = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("Error set options");
        close(sockfd);
        exit(1);
    }

    struct iphdr *iph = (struct iphdr *)buffer;
    iph->version = 4;
    iph->ihl = 5;
    iph->tos = 0;
    iph->tot_len = htons(20 + 8 + strlen(SMSG));
    iph->id = htons(SRC_PORT);
    iph->frag_off = 0;
    iph->ttl = 255;//MAX
    iph->protocol = 17;//IPPROTO_UDP
    iph->check = 0;  
    iph->saddr = inet_addr(SRC_IP);
    iph->daddr = inet_addr(SERVER_IP);

 
    struct udphdr *udph = (struct udphdr *)(buffer + sizeof(struct iphdr));
    udph->source = htons(SRC_PORT);
    udph->dest = htons(DST_PORT);
    udph->len = htons(sizeof(struct udphdr) + strlen(SMSG));
    udph->check = 0;  

  
    char *data = buffer + sizeof(struct iphdr) + sizeof(struct udphdr);
    strcpy(data, SMSG);

    
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(SERVER_IP);


    if (sendto(sockfd, buffer, ntohs(iph->tot_len), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("Error sending message");
        close(sockfd);
        exit(EXIT_FAILURE);
    } else {
        printf("Packet Sended!!!\n");
    }

    while (1) {
        int recv_len = recv(sockfd, buffer, BUFLEN, 0);
        if (recv_len < 0) {
            perror("Error receiving message");
            close(sockfd);
            exit(1);
        }

        unsigned short rdst_port;
        memcpy(&rdst_port, buffer + 20 + 2, sizeof(rdst_port));
        rdst_port = ntohs(rdst_port);

        if (rdst_port == SRC_PORT) {
            printf("Received packet: %s\n", buffer + 20 + 8);
            break;
        }
    }

    close(sockfd);
    return 0;
}
