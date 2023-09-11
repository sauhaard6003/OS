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
#include <strings.h>
#include <time.h>
key_t k1=121;
key_t k2=122;
key_t k3=12987;
key_t k4=124;
key_t k5=125;
key_t k6=126;
#define BUFFER 25
int shmid1, shmid2, shmid3, shmid4, shmid5, shmid6;
int* buff;
int *empty;
sem_t *lock;
int *fill;
int *flag;
int *total_items;
#define NUM_PROD 201
#define NUM_CONS 101
void print(int* buff){
    printf("buffer is: ");
    for (int i=0;i<BUFFER;i++){
        printf("%d ",buff[i]);
    }
    printf("\n");
    for (int i=0;i<200000000;i++){
        ;
    }
}
void main(char arg, char* args[]){
    shmid1=shmget(k1, (BUFFER) * sizeof(int), IPC_CREAT|0660);
    shmid2=shmget(k2, sizeof(int), IPC_CREAT|0660);
    shmid3=shmget(k3, sizeof(sem_t), IPC_CREAT|0770);
    shmid4=shmget(k4, sizeof(int), IPC_CREAT|0660);
    shmid5=shmget(k5, sizeof(int), IPC_CREAT|0660);
    shmid6=shmget(k6, sizeof(int), IPC_CREAT|0660);
    buff= (int *)shmat(shmid1, NULL, 0);
    empty=(int *)shmat(shmid2, NULL, 0);
    lock= (sem_t *)shmat(shmid3, NULL, 0);
    fill= (int *)shmat(shmid4, NULL, 0);
    flag= (int *)shmat(shmid5, NULL, 0);
    total_items= (int*)shmat(shmid6, NULL, 0);
    *empty=BUFFER;
    sem_init(lock,1,1);
    *fill=0;
    *total_items=0;
    //*flag=0;
    for (int i=0;i<BUFFER;i++){
        buff[i]=0;
    }
    //print(buff);
    pid_t rc=fork();
    if (rc<0){
        printf("fork failed yaar\n");
        exit(0);
    }
    else if (rc==0){ //child process, i.e. producer
        *flag=0;
        int pindex=0;
        //print(buff);
        for (int i=0;i<NUM_PROD;i++){
            //sleep(2);
            // while (total_items>=BUFFER){
            //     ;
            // }
            
            while ((*empty)==0){
                ;
            }
            sem_wait(lock);
            //sleep(0.5);
            int item=rand()%BUFFER+1;
            while (buff[pindex]!=0){
                pindex++;
                pindex%=BUFFER;
            }
            buff[pindex]=item;
            printf("Produced: %d,\t",buff[pindex]);
            // for (int i=0;i<1000000000;i++){
            //     ;
            // }
            //printf("lol");
            pindex++;
            pindex%=BUFFER;
            //print(buff);
            //printf("total items: %d\n", *total_items);
            (*total_items)++;
            //sem_wait(&empty);
            printf("total items: %d\n", *total_items);
            //sleep(0.5);
            (*fill)++;
            (*empty)--;
            sem_post(lock);
            //print(buff);
            //printf("total items: %d\n", *fill);
            //sleep(1);
        }
        *flag=1;
    }
    else{ //parent process, i.e. consumer
        int cindex=0;
        for (int j=0;j<NUM_CONS;j++){
            if (*flag==1 && (*total_items)==1){
                // sleep(0.1);
                // printf("Consumed: %d, ",buff[cindex]);
                // buff[cindex++]=0;
                // cindex%=BUFFER;
                // (*total_items)--;
                // printf("total items: %d\n", *total_items);
                return;
            }
            while ((*total_items)<2){
                ;
            }
            sem_wait(lock);
            printf("Consumed: %d ",buff[cindex]);
            buff[cindex++]=0;
            cindex%=BUFFER;
            // for (int i=0;i<1000000000;i++){

            //     ;
            // }
            //sleep(0.2);
            printf("and %d,\t",buff[cindex]);
            buff[cindex++]=0;
            //sleep(2);
            cindex%=BUFFER;
            (*total_items)-=2;
            //sleep(2);
            //print(buff);
            //sem_post(&empty);
            //sem_post(&empty);
            printf("total items: %d\n", *total_items);
            //sleep(0.5);
            (*empty)+=2;
            (*fill)-=2;
            sem_post(lock);
            //print(buff);
        }
        //memset(buff,0,sizeof(buff));
        shmdt(buff);
        shmdt(empty);
        shmdt(lock);
        shmdt(fill);
        shmdt(flag);
        shmdt(total_items);
        shmctl(shmid1,IPC_RMID,0);
        shmctl(shmid2,IPC_RMID,0);
        shmctl(shmid3,IPC_RMID,0);
        shmctl(shmid4,IPC_RMID,0);
        shmctl(shmid5,IPC_RMID,0);
        shmctl(shmid6,IPC_RMID,0);
    }
}