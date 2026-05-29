#define MAXBUFF 80
#define PERM 0666

typedef struct our_msgbuff
{
    long mtype;
    char buff[MAXBUFF];
} Message;
