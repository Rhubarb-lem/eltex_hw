#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <sys/ioctl.h>

#define BUFLEN 200
#define SMSG "HI!"
#define SERVER_IP "192.168.122.153"
#define CLIENT_IP "192.168.122.1"


unsigned short checksum(void *vdata, size_t length) {
    char *data = (char *)vdata;
    unsigned int sum = 0;

 
    for (size_t i = 0; i + 1 < length; i += 2) {
        unsigned short word;
        memcpy(&word, data + i, 2);
        sum += ntohs(word);
        if (sum > 0xffff) {
            sum -= 0xffff;
        }
    }

    if (length & 1) {//Odd/even check
        unsigned short word = 0;
        memcpy(&word, data + length - 1, 1);
        sum += ntohs(word);
        if (sum > 0xffff) {
            sum -= 0xffff;
        }
    }

    return htons(~sum);
}

int main() {
    int sockfd;
    char buffer[BUFLEN];
    struct sockaddr_ll dest_addr;

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }


    struct ethhdr eth_header;
    memset(&eth_header, 0, sizeof(struct ethhdr));
    //src fe:54:00:5b:4b:90
    eth_header.h_source[0] = 0xfe;
    eth_header.h_source[1] = 0x54;
    eth_header.h_source[2] = 0x00;
    eth_header.h_source[3] = 0x5b;
    eth_header.h_source[4] = 0x4b;
    eth_header.h_source[5] = 0x90;
    //dst 52:54:00:0d:cd:f1
    eth_header.h_dest[0] = 0x52;
    eth_header.h_dest[1] = 0x54;
    eth_header.h_dest[2] = 0x00;
    eth_header.h_dest[3] = 0x0d;
    eth_header.h_dest[4] = 0xcd;
    eth_header.h_dest[5] = 0xf1;
    eth_header.h_proto = htons(ETH_P_IP);

    struct iphdr ip_header;
    memset(&ip_header, 0, sizeof(struct iphdr));
    ip_header.ihl = 5;
    ip_header.version = 4;
    ip_header.tos = 0;
    ip_header.tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(SMSG));
    ip_header.id = htons(54321);
    ip_header.frag_off = 0;
    ip_header.ttl = 255;
    ip_header.protocol = IPPROTO_UDP;
    ip_header.saddr = inet_addr(CLIENT_IP);
    ip_header.daddr = inet_addr(SERVER_IP);
    ip_header.check = checksum(&ip_header, sizeof(struct iphdr));


    struct udphdr udp_header;
    udp_header.source = htons(54321);
    udp_header.dest = htons(12345);
    udp_header.len = htons(sizeof(struct udphdr) + strlen(SMSG));
    udp_header.check = 0;

    memset(buffer, 0, BUFLEN);
    memcpy(buffer, &eth_header, sizeof(struct ethhdr));
    memcpy(buffer + sizeof(struct ethhdr), &ip_header, sizeof(struct iphdr));
    memcpy(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr), &udp_header, sizeof(struct udphdr));
    strcpy(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr), SMSG);

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sll_family = AF_PACKET;
    dest_addr.sll_ifindex = if_nametoindex("virbr0");
    dest_addr.sll_halen = ETH_ALEN;
    memcpy(dest_addr.sll_addr, eth_header.h_dest, ETH_ALEN);


    if (sendto(sockfd, buffer, sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(SMSG), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("Error sending message");
        close(sockfd);
        exit(EXIT_FAILURE);
    } else {
        printf("Packet Sended\n");
    }

    while (1) {
        int recv_len = recv(sockfd, buffer, BUFLEN, 0);
        if (recv_len < 0) {
            perror("Error receiving message");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        struct ethhdr *eth_header_recv = (struct ethhdr *)buffer;
        if (ntohs(eth_header_recv->h_proto) == ETH_P_IP) {
            struct iphdr *ip_header_recv = (struct iphdr *)(buffer + sizeof(struct ethhdr));
            if (ip_header_recv->protocol == IPPROTO_UDP) {
                struct udphdr *udp_header_recv = (struct udphdr *)(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));
                if (ntohs(udp_header_recv->dest) == 54321) {
                    printf("Received packet: %s\n", buffer + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr));
                    break;
                }
            }
        }
    }

    close(sockfd);
    return 0;
}
