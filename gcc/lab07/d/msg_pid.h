#ifndef MSG_PID_H
#define MSG_PID_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MSG_KEY 12345
#define MSG_SIZE 256
#define PERM 0666

typedef struct {
    int pid;
    char text[MSG_SIZE];
} message_data;

typedef struct {
    long mtype;
    message_data data;
} message_buffer;

#endif
