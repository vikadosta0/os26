#include <memory.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "../../lib/m_time.h"


void fnDaemon(void)
{   
    umask(0);
    pid_t sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    if ((chdir("/")) < 0)  exit(EXIT_FAILURE);


    char buf[512], sDateTime[64];
    const char* msg00="Daemon is alive!\n";
    const char* msg01="Daemon\t";
    const char* file_name="/tmp/daemon_timer.log";

    FILE* fd_journal=fopen(file_name,"a+");
    if (fd_journal)
    {    fputs(msg00,fd_journal);
         fclose(fd_journal);
    }
    while (1)
    {
        memset(buf,0,512);
        fnGetDateTime(sDateTime, 64, 0, 0);
        fnGetProcTime(0, 0);
        sprintf(buf,"%s\t%sGID: %d\tPID: %d\tPPID: %d\tUID: %d\tSID:%d\tSID: %d\n",
                    sDateTime,msg01,getgid(),getpid(),getppid(),getuid(),getsid(getpid()),sid);
        FILE* fd_journal=fopen(file_name,"a+");
        if (fd_journal)
        {   fputs(buf,fd_journal);
            fclose(fd_journal);
        }    
        sleep(10);
    }     
    exit(0);
}


int main(void)
{
    pid_t pid=0, pid0;
    pid0=getpid();
    switch(pid=fork()){
    case -1:
        perror("fork"); 
        exit(1); 
    case 0:
          fnDaemon();
    default:
        sleep(1);
    }
    return 0;
}
