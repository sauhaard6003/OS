#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
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
        char *array[3];
        array[0] = strdup("ls");  //function to be applied       
        array[1] = strdup("-a");  //argument     
        array[2] = NULL; //end of file  
        char *const envp[2] = {"", NULL};
        execle("/usr/bin/ls", "-a", NULL, envp);         
        execvp(array[0], array);
    }
    else
    {
        // parent process
        wait(NULL);
        printf("hello, I am parent of %d (pid:%d)\n",
               rc, (int)getpid());
    }
    return 0;
}
