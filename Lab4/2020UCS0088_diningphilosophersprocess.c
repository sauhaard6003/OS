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
static const int SLEEP_TIME=10000;

int flag=0;
int *request;  
int *forks;    
  
int *dirty;   
int *state; // 1 denotes philosopher is thinking, 2 denotes hungry , 3 denotes eating, 4 denotes done  
sem_t *sem_fork;


void *philosopher(int this_ID) {	

	int firstpick = this_ID;
	int secondpick = this_ID+1;
	
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

	key_t k1=123;
    key_t k2=2345;
    key_t k3=34567;
    key_t k4=456789;
    key_t s1=567899;

    int shmid1=shmget(k1, N*sizeof(int), IPC_CREAT|0660);
	int shmid2=shmget(k2, N*sizeof(int), IPC_CREAT|0660);
	int shmid3=shmget(k3, N*sizeof(int), IPC_CREAT|0660);
	int shmid4=shmget(k4, N*sizeof(int), IPC_CREAT|0660);
    int shm =  shmget(s1, N*sizeof(sem_t), IPC_CREAT | 0777 );
    sem_fork=(sem_t*)shmat(shm,0,0);

	forks=(int *)shmat(shmid1,NULL,0);
	request=(int *)shmat(shmid2,NULL,0);
	dirty=(int *)shmat(shmid3,NULL,0);
	state=(int *)shmat(shmid4,NULL,0);


	int phil[N];
    for(int i=0; i<N; i++){
        dirty[i]=1; //all forks are initially dirty
        forks[i]=-1; //forks are not in ownership of anyone
        request[i]=-1;
		phil[i]=i;
		sem_init(&sem_fork[i],1,1); //initialize such that any philosopher can eat at the beginning
    }

    printf("Start of eating process\n");
    for(int i = 0; i < N; i++){
        pid_t rc=fork();
        if(rc==0){
			philosopher(i);
			exit(0);
		}
        if (rc<0){
            printf("Fork failed\n");
            exit(0);
        }
    }
	shmdt(dirty);
    shmdt(state);
    shmdt(request);
    shmdt(forks);
    shmdt(sem_fork);
    shmctl(shmid1,IPC_RMID,0);
    shmctl(shmid2,IPC_RMID,0);
    shmctl(shmid3,IPC_RMID,0);
    shmctl(shmid4,IPC_RMID,0);
    shmctl(shm,IPC_RMID,0);
    sleep(1);
    printf("End of eating process\n");
    exit(0);
}