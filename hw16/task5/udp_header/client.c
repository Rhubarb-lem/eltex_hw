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

    unsigned short src_port = htons(54321); 
    unsigned short dst_port = htons(12345); 
    unsigned short udp_length = htons(sizeof(struct udphdr) + strlen(SMSG));
    unsigned short udp_checksum = 0; 

    memset(buffer, 0, BUFLEN);
    memcpy(buffer, &src_port, 2);
    memcpy(buffer + 2, &dst_port, 2);
    memcpy(buffer + 4, &udp_length, 2);
    memcpy(buffer + 6, &udp_checksum, 2);


    char *data = buffer + sizeof(struct udphdr);
    strcpy(data, SMSG);


    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(SERVER_IP);


    if (sendto(sockfd, buffer, sizeof(struct udphdr) + strlen(SMSG), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("sendto failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    } else {
        printf("Packet Sent\n");
    }

    while (1) {
        int recv_len = recv(sockfd, buffer, BUFLEN, 0);
        if (recv_len < 0) {
            perror("recv failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

       
        unsigned short rdst_port;
        memcpy(&rdst_port, buffer + 20 + 2, 2);
        rdst_port = ntohs(rdst_port);

        if (rdst_port == 54321) {
            printf("Received packet: %s\n", buffer + 20 + 8);
            break;
        }
    }

    close(sockfd);
    return 0;
}
