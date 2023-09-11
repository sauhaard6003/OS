#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
 
int
main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
    // child: redirect standard output to "output.txt"
    close(STDOUT_FILENO); 
    open("output.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
    printf("print from child\n");
    
    } else {
        //parent process
        int wt = wait(NULL);
    	assert(wt >= 0);
    }
    return 0;
}
