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

#define THREADCOUNT 10

int running[THREADCOUNT];
int tokens[THREADCOUNT];
int count = 0;

int getMax(int tokens[]){
    int n = sizeof(*tokens) / sizeof(int);
    int MAX = tokens[0];
    for(int i = 0; i < n ;++i){
        if(tokens[i]>MAX){
            MAX = tokens[i];
        }
    }

    return MAX;
}


void Lock(int ProcessID){
    ProcessID -= 1;
    running[ProcessID] = 1;
    tokens[ProcessID] = 1 + getMax(tokens);
    running[ProcessID] = 0;
}

void Unlock(int ProcessID){
    ProcessID -= 1;
    tokens[ProcessID]= 0;
}

void *initThread(void* threadNum){
    int thread = *((int*)threadNum);
    Lock(thread);
    count += 1;
    Unlock(thread);
    return NULL;
}

int main() {

    pthread_t threads[THREADCOUNT];

    for(int i = 0; i < THREADCOUNT; i++){
        pthread_create(&threads[i], NULL, &initThread, &i);
    }

    for(int i = 0; i < THREADCOUNT; i++){
        pthread_join(threads[i], NULL);
    }

    printf("Count: %d \n",count);
}