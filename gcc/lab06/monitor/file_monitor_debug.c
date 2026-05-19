#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

#define LOG_FILE "/tmp/file_monitor.log"
#define DEBUG_FILE "/tmp/file_monitor_debug.log"
#define DIR_PATH "/home/dosta_va/os26/test"
#define CHECK_INTERVAL 10

void debug(const char *msg) {
    FILE *d = fopen(DEBUG_FILE, "a");
    if (d) {
        fprintf(d, "%s\n", msg);
        fclose(d);
    }
}

unsigned long crc32_file(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;
    unsigned long crc = 0xFFFFFFFF;
    int c;
    while ((c = fgetc(f)) != EOF) {
        crc ^= c;
        for (int i = 0; i < 8; i++) {
            if (crc & 1) crc = (crc >> 1) ^ 0xEDB88320;
            else crc >>= 1;
        }
    }
    fclose(f);
    return ~crc;
}

void log_change(const char *filename, unsigned long old_crc, unsigned long new_crc) {
    FILE *log = fopen(LOG_FILE, "a");
    if (log) {
        time_t now = time(NULL);
        fprintf(log, "%s: %s changed (CRC: %08lX -> %08lX)\n", 
                ctime(&now), filename, old_crc, new_crc);
        fclose(log);
    }
}

int main() {
    debug("=== Daemon started ===");
    pid_t pid = fork();
    if (pid < 0) return 1;
    if (pid > 0) return 0;
    umask(0);
    setsid();
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    chdir("/");
    debug("Daemon running");
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char fullpath[512];
    char filenames[100][256];
    unsigned long crcs[100];
    int count = 0;
    dir = opendir(DIR_PATH);
    if (!dir) {
        debug("ERROR: Cannot open directory!");
        return 1;
    }
    debug("Scanning directory...");
    while ((entry = readdir(dir)) != NULL && count < 100) {
        if (entry->d_name[0] == '.') continue;
        snprintf(fullpath, sizeof(fullpath), "%s/%s", DIR_PATH, entry->d_name);
        if (stat(fullpath, &st) == 0 && S_ISREG(st.st_mode)) {
            strcpy(filenames[count], entry->d_name);
            crcs[count] = crc32_file(fullpath);
            char buf[256];
            sprintf(buf, "File: %s CRC: %08lX", entry->d_name, crcs[count]);
            debug(buf);
            count++;
        }
    }
    closedir(dir);
    while (1) {
        sleep(CHECK_INTERVAL);
        debug("Checking...");
        for (int i = 0; i < count; i++) {
            snprintf(fullpath, sizeof(fullpath), "%s/%s", DIR_PATH, filenames[i]);
            unsigned long new_crc = crc32_file(fullpath);
            if (new_crc != crcs[i]) {
                char buf[256];
                sprintf(buf, "CHANGED: %s", filenames[i]);
                debug(buf);
                log_change(filenames[i], crcs[i], new_crc);
                crcs[i] = new_crc;
            }
        }
    }
    return 0;
}
