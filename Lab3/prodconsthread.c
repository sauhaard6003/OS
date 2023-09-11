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

#define BUFFER 25
sem_t empty,lock,fill;
int total_items=0;
int pindex=0;
int cindex=0;
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
int flag=0;
void producer(int *buff){
    flag=0;
    for (int i=0;i<NUM_PROD;i++){
        //sleep(2);
        // while (total_items>=BUFFER){
        //     ;
        // }
        while (total_items>=BUFFER){
            ;
        }
        //sem_wait(&empty);
        sem_wait(&lock);
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
        //sleep(0.1);
        pindex++;
        pindex%=BUFFER;
        //print(buff);
        total_items++;
        //sem_wait(&empty);
        printf("total items: %d\n", total_items);
        // sem_post(&fill);
        sem_post(&lock);
        //sleep(1);
    }
    flag=1;
}

void consumer(int *buff){

    for (int j=0;j<NUM_CONS;j++){
        if (flag==1 && total_items==1){
            // sleep(0.1);
            // printf("Consumed: %d, ",buff[cindex]);
            // buff[cindex++]=0;
            // cindex%=BUFFER;
            // total_items-=1;
            // printf("total items: %d\n", total_items);
            return;
        }
        while (total_items<2){
            ;
        }
        sem_wait(&lock);
        // sem_wait(&fill);
        // sem_wait(&fill);
        //sem_wait(&empty);
        //sem_wait(&empty);
        //cindex%=10;
        
        // for (int i=0;i<1000000000;i++){
        //     ;
        // }
        //sleep(0.1);
        printf("Consumed: %d ",buff[cindex]);
        buff[cindex++]=0;
        cindex%=BUFFER;
        // for (int i=0;i<1000000000;i++){
        //     ;
        // }
        //sleep(0.1);
        printf("and %d,\t",buff[cindex]);
        buff[cindex++]=0;
        //sleep(2);
        cindex%=BUFFER;
        total_items-=2;
        //sleep(2);
        //print(buff);
        //sem_post(&empty);
        //sem_post(&empty);
        printf("total items: %d\n", total_items);
        // sem_post(&empty);
        // sem_post(&empty);
        sem_post(&lock);
        //sleep(1);
    }
}
int main(char arg, char* args[]){
    int final[BUFFER];
    memset(final,0,sizeof(final));
    sem_init(&empty,0,BUFFER);
    sem_init(&lock,0,1);
    sem_init(&fill,0,0);
    pthread_t prod,cons;
    pthread_create(&prod, NULL, (void *)producer, final);
    pthread_create(&cons, NULL, (void *)consumer, final);
    pthread_join(prod,NULL);
    pthread_join(cons, NULL);
    return 0;
}