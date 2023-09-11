#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
int main(int argc, char *argv[])
{
    int temp = 100;
    //initialize the variable
    printf("hello world (pid:%d)\n", (int)getpid());
    printf("temp before fork method is called is %d\n", temp);
    int rc = fork();
    if (rc < 0)
    {
        // fork failed so exit`
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        // child process
        printf("hello, I am child (pid:%d)\n", (int)getpid());
        printf("temp entering child process is %d\n", temp);
        temp = 500;
        printf("temp exiting the child process is %d\n", temp);
    }
    else
    {
        // parent process
        printf("hello, I am parent of %d (pid:%d)\n",
               rc, (int)getpid());
        printf("temp entering the parent process is %d\n", temp);
        temp = 1000;
        printf("temp exiting the parent process is %d\n", temp);
    }
    printf("the final value of temp is %d\n", temp);
    return 0;
}
