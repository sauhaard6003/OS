#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define KEY 3673
#define MAXITER 20
#define LIMIT 5

typedef struct shared {
    int limit, maxIter;
    int Full, Empty;
} shared;
int shmid;

void Process1();
void Process2();
void printval(int, int);

// In this code,  process 1 is producer which increases the value of limit to LIMIT and process 2 is consumer which reads the value 
// of limit from shared memory and decrement it until  it becomes zero again. This switching of processes continues until the total 
// number of iterations of both the process reach MAXITER


int main() {
    shmid = shmget(KEY, sizeof(shared), 0644|IPC_CREAT);
    shared* shmp = shmat(shmid, NULL, 0);
    shmp->limit = 0;
    shmp->maxIter = MAXITER;
    shmp->Empty = 1;
    shmp->Full = 0;
    int rc = fork();
    if(rc == -1) {
        fprintf(stderr, "fork failed!\n");
    } else if(rc == 0) {
        Process1();
    } else {
        Process2();
    }
    if(shmp->maxIter == 0) {
        shmdt(shmp);
        shmctl(shmid, IPC_RMID, 0);   //remove the data from shared memory
    }
    return 0;
}

void Process2() {
    shared* shmp = shmat(shmid, NULL, 0);
    while(shmp->maxIter != 0) {
        while(shmp->Empty == 1 && shmp->maxIter != 0) {
            // wait for process 1 to produce value of limit aand increase it to LIMIT
        }
        while(shmp->limit != 0 && shmp->maxIter != 0) {
            // consume the value of limit and  made limit = 0 
            shmp->limit--;
            printval(shmp->limit, LIMIT - shmp->limit);
            shmp->maxIter--;
        }
        shmp->Empty = 1;
        shmp->Full = 0;
    }
    shmdt(shmp);
}

void Process1() {
    shared* shmp = shmat(shmid, NULL, 0);
    while(shmp->maxIter != 0) {
        while(shmp->Full == 1 && shmp->maxIter != 0) {
            // wait for process 2 to consume value of limit and made limit =0
        }
        while(shmp->limit != LIMIT && shmp->maxIter != 0) {
            // increase limit and made limit = LIMTI
            shmp->limit++;
            printval(shmp->limit, 0);
            shmp->maxIter--;
        }
        shmp->Empty = 0;
        shmp->Full = 1;
    }
    shmdt(shmp);
}
void printval(int stock, int consumed) {
    printf("Stock: %d, Consumed: %d\n", stock, consumed);
}