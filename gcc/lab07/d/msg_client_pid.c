#include "msg_pid.h"

int main()
{
    int msgid;
    message_buffer buf;
    
    msgid = msgget(MSG_KEY, PERM);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }
    
    buf.mtype = 1;
    buf.data.pid = getpid();
    
    printf("Введите сообщение для сервера: ");
    fgets(buf.data.text, MSG_SIZE, stdin);
    
    if (msgsnd(msgid, &buf, sizeof(buf.data), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }
    
    printf("Сообщение отправлено (PID=%d)\n", buf.data.pid);
    
    return 0;
}
