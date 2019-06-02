#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <signal.h>
#include <sys/fcntl.h>

int pidChild[5]; 
int indexPid;

// function to activate when a signal is caught
int sigCatcher()
{
    signal(SIGINT, (__sighandler_t)sigCatcher); 
    printf("PID %d caught one\n", getpid());
    if (indexPid > -1)
        kill(pidChild[indexPid], SIGINT); 
}
int main()
{
    int zombie;
    int status;
    int pid;
    signal(SIGINT, (__sighandler_t)sigCatcher); 
    for (int i = 0; i < 5; i++)
    {
        if ((pid = fork()) == 0)
        { 
            printf("PID %d ready\n", getpid());
            indexPid = i - 1;
            pause(); 
            exit(0); 
        }
        else 
            pidChild[i] = pid;
    }
    sleep(2);              
    kill(pidChild[4], SIGINT);
    sleep(2);            
    for (int i = 0; i < 5; i++)
    {
        zombie = wait(&status); 
        printf("Process %d is dead\n", zombie);
    }
    exit(0);
}