#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define KEY 5673

typedef struct shared {
    int favouredProcess, completed;
    int P1WantsToEnter, P2WantsToEnter;
} shared;
int shmid;

void Process1();
void Process2();

int main() {
    shmid = shmget(KEY, sizeof(shared), 0644|IPC_CREAT);
    shared* shmp = shmat(shmid, NULL, 0);
    shmp->favouredProcess = 1;
    shmp->P1WantsToEnter = 0;
    shmp->P2WantsToEnter = 0;
    shmp->completed = 0;
    int rc = fork();
    if(rc == -1) {
        fprintf(stderr, "fork failed!\n");
        exit(EXIT_FAILURE);
    } else if(rc == 0) {
        Process1();
    } else {
        Process2();
    }
    if(shmp->completed == 2) {
        shmdt(shmp);
        shmctl(shmid, IPC_RMID, 0);
    }
    return 0;
}

void Process1() {
    shared* shmp = shmat(shmid, NULL, 0);
    do {
        shmp->P1WantsToEnter = 1;
        while(shmp->P2WantsToEnter == 1) {
            if (shmp->favouredProcess == 2) {
                shmp->P1WantsToEnter = 0;
                while(shmp->favouredProcess == 2) {
                    shmp->completed++;
                    printf("process 2 finised!\n");
                }
                shmp->P1WantsToEnter = 1;
            }
        }
        shmp->favouredProcess = 2;
        shmp->P1WantsToEnter = 0;
    } while(shmp->completed < 2);
    shmdt(shmp);
}

void Process2() {
    shared* shmp = shmat(shmid, NULL, 0);
    do {
        shmp->P2WantsToEnter = 1;
        while(shmp->P1WantsToEnter == 1) {
            if (shmp->favouredProcess == 1) {
                shmp->P2WantsToEnter = 0;
                while(shmp->favouredProcess == 1) {
                    shmp->completed++;
                    printf("process 1 finished!\n");
                }
                shmp->P2WantsToEnter = 1;
            }
        }
        shmp->favouredProcess = 1;
        shmp->P2WantsToEnter = 0;
    } while(shmp->completed < 2);
    shmdt(shmp);
}