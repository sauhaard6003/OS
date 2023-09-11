#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
 
int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int)getpid());
    int op = open("text.txt", O_CREAT|O_RDWR);
    if(op == -1 ){
        printf("File can't open\n");
        return 0;
    }
    
    int rc = fork();
    if (rc < 0)
    {
        // fork failed; exit`
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        // child process
        printf("hello, I am child (pid:%d)\n", (int)getpid());
        char temp='a';
        write(op, &temp,1);
        printf("writes %c in file from child\n", temp);
    }
    else
    {
        //wait(NULL);
        // parent goes down this path (original process)
        printf("hello, I am parent of %d (pid:%d)\n",
               rc, (int)getpid());
        char temp='z';
        write(op, &temp,1);
        printf("writes %c in file from parent\n", temp);
    }
    return 0;
}
