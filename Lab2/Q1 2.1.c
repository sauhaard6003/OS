#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
 
int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int)getpid());
    int op = open("code.c", O_RDONLY);
    if(op == -1 ){
        printf("Unable to open the file\n");
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
        char temp;
        read(op, &temp,1);
        printf("Read and print a child file char: %c\n", temp);
        //
    }
    else
    {
        wait(NULL);
        //parent process
        printf("hello, I am parent of %d (pid:%d)\n", rc, (int)getpid());
        char temp;
        read(op, &temp,1);
        printf("Read and print a parent file char: %c\n", temp);
    }
    return 0;
}
