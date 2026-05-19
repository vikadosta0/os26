#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char* argv[])
{    
    char *cmd;
    char *arg;
    printf("Thiss is the mustre:\n");
    printf("argc = %d\n",argc);
    printf("argv = %X\t&argv = %X\t\tchar(*)*argv = %s\n\n",argv,&argv,(char*)*argv);
    if (argc>1)
    {   cmd = argv[1];
        pid_t pid = fork();
        if (pid<0)
        {
            perror("fork"); 
            exit(1); 
        }    
        if (pid==0)
        {
            if (argc==2) execvp(cmd, NULL);
            if (argc==3) execlp(cmd, ".", argv[2], (char *)NULL);
            if (argc==4) execlp(cmd, ".", argv[2], argv[3], (char *)NULL);
        }
        else
        {  
            wait(0);
        }
    }    
    printf("\n");
    return 0;
}
