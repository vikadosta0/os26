#include "msg_pid.h"

int main()
{
    int msgid;
    message_buffer buf;
    
    msgid = msgget(MSG_KEY, PERM | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }
    
    printf("Сервер PID запущен, ожидает сообщения...\n");
    
    while (1) {
        if (msgrcv(msgid, &buf, sizeof(buf.data), 1, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        
        printf("Получено сообщение от PID=%d: %s\n", buf.data.pid, buf.data.text);
    }
    
    return 0;
}
