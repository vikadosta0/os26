#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

#define LOG_FILE "/tmp/file_monitor.log"
#define DIR_PATH "/home/dosta_va/os26/test"
#define CHECK_INTERVAL 20

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

void monitor_files() {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char fullpath[512];
    
    static char filenames[100][256];
    static unsigned long crcs[100];
    static int count = 0;
    static int first = 1;
    
    dir = opendir(DIR_PATH);
    if (!dir) return;
    
    if (first) {
        count = 0;
        while ((entry = readdir(dir)) != NULL && count < 100) {
            if (entry->d_name[0] == '.') continue;
            snprintf(fullpath, sizeof(fullpath), "%s/%s", DIR_PATH, entry->d_name);
            if (stat(fullpath, &st) == 0 && S_ISREG(st.st_mode)) {
                strcpy(filenames[count], entry->d_name);
                crcs[count] = crc32_file(fullpath);
                count++;
            }
        }
        first = 0;
    } else {
        rewinddir(dir);
        int idx = 0;
        while ((entry = readdir(dir)) != NULL && idx < count) {
            if (entry->d_name[0] == '.') continue;
            snprintf(fullpath, sizeof(fullpath), "%s/%s", DIR_PATH, entry->d_name);
            if (stat(fullpath, &st) == 0 && S_ISREG(st.st_mode)) {
                for (int i = 0; i < count; i++) {
                    if (strcmp(filenames[i], entry->d_name) == 0) {
                        unsigned long new_crc = crc32_file(fullpath);
                        if (new_crc != crcs[i]) {
                            log_change(entry->d_name, crcs[i], new_crc);
                            crcs[i] = new_crc;
                        }
                        break;
                    }
                }
            }
        }
    }
    closedir(dir);
}

int main() {
    pid_t pid = fork();
    if (pid < 0) exit(1);
    if (pid > 0) exit(0);
    
    umask(0);
    setsid();
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    chdir("/");
    
    while (1) {
        monitor_files();
        sleep(CHECK_INTERVAL);
    }
    return 0;
}
