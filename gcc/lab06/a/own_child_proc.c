#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/wait.h>
#include "../../lib/m_time.h"


void fnChild(void)
{
    int rv;
    printf("CHAILD>\tSID: %d\tGID: %d\tPID: %d\tPPID: %d\tUID: %d\n",getsid(getpid()),getgid(),getpid(),getppid(),getuid());
    printf("CHILD>:\tВведите мой код возврата (от 0 до 255, где  0 - без ошибки):");
    for (long i=0, j=0; i<100000000; i++) j+=i;
    scanf("%d",&rv);
    printf("CHILD>\tВыход!\n");
    exit(rv);
}

void fnParent(pid_t pid_fork)
{
    int rv;
    printf("PARENT>\tSID: %d\tGID: %d\tPID: %d\tPPID: %d\tUID: %d\n",getsid(getpid()),getgid(),getpid(),getppid(),getuid());
    printf("PARENT>\tPID моего потомка %d\n",pid_fork);
    printf("PARENT>\tжду, пока потомок не вызовет exit()...\n");
    //pid_t pid1=wait(0);
    pid_t pid1=waitpid(pid_fork, &rv, 0);
    //pid_t pid1=waitpid(pid_fork, &rv, WNOHANG);
    //pid_t pid1=waitpid(pid_fork, &rv, WUNTRACED);
    printf("PARENT>\twait(0) = %d\n",pid1);
    for (long i=0, j=0; i<100000000; i++) j+=i;
    fnGetDateTime(NULL, 0, 1, 1);
    fnGetProcTime(1,1);
    printf("PARENT>\tКод возврата WIFEXITED: %d\n",WIFEXITED(rv));
    printf("PARENT>\tКод возврата WEXITSTATUS: %d\n",WEXITSTATUS(rv));
    printf("PARENT>\tВыход!\n");
}

int main(void)
{
    pid_t pid, pid0;
    pid0=getpid();
    switch(pid=fork()){
    case -1:
        perror("fork"); 
        exit(1); 
    case 0:
          fnChild();
    default:
          fnParent(pid);
    }
    return 0;
}
