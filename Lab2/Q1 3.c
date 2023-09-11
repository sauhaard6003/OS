#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int)getpid());
    int rc = fork();
    if (rc < 0)
    {
        // fork failed; exit`
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        // child (new process)
        printf("hello  :  ");
        printf("I am child (pid:%d)\n", (int)getpid());
    }
    else
    {
        int count=0;
        while (count!=100000){
	    count++;        
        }
        //usleep(100000);
        // parent process
        printf("goodbye  :  ");
        printf("I was the parent of %d (pid:%d)\n",
               rc, (int)getpid());
    }
    return 0;
}
