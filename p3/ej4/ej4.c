#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

sem_t sem;

void * escribirA(void * p){
    int i;

    for(i=0; i<5; i++){


        printf("A");
        fflush(NULL);
    }

    pthread_exit(NULL);
}

void * escribirB(void * p){
    int i;

    for(i=0; i<5; i++){
        printf("B");
        fflush(NULL);
    }

    pthread_exit(NULL);
}

int main(){
    pthread_t tid_a, tid_b;

    sem_init(&sem, 0, 1);

    if(pthread_create(&tid_a, NULL, (void *) escribirA, NULL)){
        printf("Error creating thread\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_create(&tid_b, NULL, (void *) escribirB, NULL)){
        printf("Error creating thread\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_join(tid_a, NULL)){
        printf("Error joining thread\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_join(tid_b, NULL)){
        printf("Error joining thread\n");
        exit(EXIT_FAILURE);
    }
    printf("\n");

    return 0;
}