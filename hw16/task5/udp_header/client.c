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


int main() {
    int sockfd;
    char buffer[BUFLEN];
    struct sockaddr_in dest_addr;

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct udphdr udp_header;
    udp_header.source = htons(54321); 
    udp_header.dest = htons(12345); 
    udp_header.len = htons(sizeof(struct udphdr) + strlen(SMSG));
    udp_header.check = 0; 

    memset(buffer, 0, BUFLEN);
    memcpy(buffer, &udp_header, sizeof(struct udphdr));

    char *data = buffer + sizeof(struct udphdr);
    strcpy(data, SMSG);

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // udp_header.check = checksum(buffer, sizeof(struct udphdr) + strlen(SMSG));
    // memcpy(buffer + offsetof(struct udphdr, check), &udp_header.check, sizeof(udp_header.check));

    if (sendto(sockfd, buffer, sizeof(struct udphdr) + strlen(SMSG), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
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
            exit(EXIT_FAILURE);
        }

        struct udphdr *udp_header_recv = (struct udphdr *)(buffer + sizeof(struct iphdr));
        if (ntohs(udp_header_recv->dest) == 54321) {
            printf("Received packet: %s\n", buffer + sizeof(struct iphdr) + sizeof(struct udphdr));
            break;
        }
    }

    close(sockfd);
    return 0;
}
