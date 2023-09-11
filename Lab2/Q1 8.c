#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/types.h> 

int main(int argc, char *argv[])
{   int pipe1[2];
    int x=pipe(pipe1);
    if (x == -1)
    {
        fprintf(stderr, "pipe failed");
        return 1;
    }
    int rc1 = fork();
    if (rc1 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc1 == 0) { //child 1
        dup2(pipe1[1], 1);
        printf("child 1 (pid:%d)\n", (int) getpid());   
        
    } else {
        int rc2 = fork();
        if (rc2 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
        } 
        else if (rc2 == 0) { //child 2
            char temp[1000];
            read(pipe1[0], temp, 1000);
            printf("pipe() of child 2 of parent :  %s\n", temp);  
            usleep(10); 
        } 
        else { //parent
            int wt=wait(NULL);
        }
    }
}

