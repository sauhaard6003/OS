#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdbool.h>

#define N 5
//static const int interations=100;
static const int SLEEP_TIME=10000;

int forks[N];    
int request[N];    
int dirty[N];   
int state[N]; // 1 denotes philosopher is thinking, 2 denotes hungry , 3 denotes eating, 4 denotes done 
sem_t sem_fork[N];


void *philosopher(void *philid) {	

	
	int this_ID = *(int*)philid;
	int firstpick = this_ID%N;
	int secondpick = (this_ID+1)%N;
	//printf("forks chosen are %d %d",firstpick,secondpick);

	//philosopher N-1 should pick up fork 0 before fork N-1, otherwise, graph turns cycle
	if (this_ID == N-1) {
		firstpick=0;
        secondpick=N-1;
	}
	
		state[this_ID] = 1;
        printf("%d is thinking\n",this_ID);	
		usleep(rand() % SLEEP_TIME); //every philosopher should think for a random amount of time
		
		state[this_ID] = 2;
        printf("%d is hungry\n",this_ID);
		sem_wait(&sem_fork[firstpick]); //if firstpick fork is not in use, enter critical section.
        forks[firstpick] = this_ID;	
		//if the second fork is not yet available, release the first fork while waiting for the second fork
        //int flag=0;

        if (dirty[firstpick]){
            forks[firstpick]=-1;
            request[firstpick] = -1;
            dirty[firstpick]=0;
            sem_post(&sem_fork[firstpick]);
        }
        if (forks[firstpick] != this_ID) {
            //sem_post(&sem_fork[secondpick]);
			sem_wait(&sem_fork[firstpick]);
            //sem_wait(&sem_fork[secondpick]);
            forks[firstpick] = this_ID;	
		}
		while (sem_trywait(&sem_fork[secondpick])) { //only advantage to release if first fork is clean
        
			if (request[firstpick] == this_ID) {
				forks[firstpick] = -1;
				request[firstpick] = -1;
                dirty[firstpick]=0;
                //flag=1;
				sem_post(&sem_fork[firstpick]);
			}
        }
		//printf("%d",forks[secondpick]);
        // if (flag==0){
        //     sem_post(&sem_fork[firstpick]);
        // }
        forks[secondpick] = this_ID;
		if (forks[firstpick] != this_ID) {
            //sem_post(&sem_fork[secondpick]);
			sem_wait(&sem_fork[firstpick]);
            //sem_wait(&sem_fork[secondpick]);
            forks[firstpick] = this_ID;	
		}
        
        
        //printf("%d", forks[firstpick]);
		state[this_ID] = 3;
        printf("%d is eating, forks chosen are %d %d\n",this_ID,firstpick,secondpick);
        //printf("forks chosen are %d %d\n",firstpick,secondpick);
        //printf("\n");
		usleep(rand() % SLEEP_TIME);
        //printf("%d",state[this_ID]);		
		//printf("%d", forks[firstpick]);
		//release both forks
        
        
        forks[secondpick] = -1;
		request[secondpick]= this_ID;  //acquires token for acquiring fork later
        forks[firstpick] = -1;
		request[firstpick] = this_ID; //acquires token for acquiring fork later
        printf("%d got completed\n",this_ID);
        state[this_ID] = 4;
        sem_post(&sem_fork[secondpick]);
		sem_post(&sem_fork[firstpick]);	//exit critical section
        
        
	
	pthread_exit(NULL);
}



int main(){
	int phil[N];
    for(int i=0; i<N; i++){
        dirty[i]=1;  //all forks are initially dirty
        forks[i]=-1; //forks are not in ownership of anyone
        request[i]=-1; 
		phil[i]=i;
		sem_init(&sem_fork[i],0,1); //initialize such that any philosopher can eat at the beginning
    }

    pthread_t threads[N];
    printf("Start of eating process\n");
    for(int i = 0; i < N; i++){
        pthread_create(&threads[i], NULL, &philosopher, (void*)&phil[i]);
    }

    for(int i = 0; i < N; i++){
        pthread_join(threads[i], NULL);
    }
    printf("End of eating process\n");
}