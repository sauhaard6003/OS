#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
 
int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int)getpid());
    int rc = fork();
    if (rc < 0)
    {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        // child process
        printf("hello, I am child (pid:%d)\n", (int)getpid());
        int wt = wait(NULL); //this returns -1;
        printf("wait from child is %d\n", wt);
    }
    else
    {
        // parent process
        int wt = wait(NULL); //this returns PID of child
        printf("hello, I am parent of %d (wait : %d) (pid:%d)\n",
               rc, wt, (int)getpid());
    }
    return 0;
}
