#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/types.h> 

//The motivation behind this code is to calculate the average time the OS takes to switch between two processes. Here, we do 2000 switchings and take note of the start and end times
int main(int argc, char *argv[])
{
    //int rc=-1;
    int pipe1[2];
    int pipe2[2];
    int pipe3[2];
    char temp1[]="",temp2[2000]; //we need to show the switching of processes and not actually store the data...storing might take even more time
    int i=0;
    struct timeval start,end;
    int x=pipe(pipe1);
    int y=pipe(pipe2);
    int z=pipe(pipe3);
    if (x == -1) 
    {
        fprintf(stderr, "piping failed\n");
        return -1;
    }
    if (y == -1) 
    {
        fprintf(stderr, "piping failed\n");
        return -1;
    }
    if (z == -1) 
    {
        fprintf(stderr, "piping failed\n");
        return -1;
    }
    int rc=fork();
    if(rc==-1){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(rc==0)
    {
        for(int n=0;n<1000;n++)
        {
            read(pipe1[0], temp2, 2000);
            write(pipe2[1], temp1, strlen(temp1)+1);
        }
        gettimeofday(&end,0);
        write(pipe3[1],&end,sizeof(struct timeval));
    }
    else
    {
        double switchcase;
        gettimeofday(&start,0);
        for(int n=0;n<1000;n++)
        {
            write(pipe1[1], temp1, strlen(temp1)+1);
            read(pipe2[0], temp2, 2000);
        }
        read(pipe3[0],&end,sizeof(struct timeval));
        int wt=wait(NULL);
        switchcase = (end.tv_usec-start.tv_usec)/1000.00;
        
        //average time is end_time - start_time/number of switches
        printf("context switch time: %0.5lfms\n",switchcase/(2000));
        printf("waiting time is %d\n",wt);
    }   
}
