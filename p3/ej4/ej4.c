#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

sem_t a;
sem_t b;

void * escribirA(void * p){
    int i;

    for(i=0; i<5; i++){
        //printf("A: bloqueo a\n");
        if(sem_wait(&a)!=0){
            printf("sem_wait error\n");
            printf("errno value= %d, definido como %s\n", errno, strerror(errno));
            pthread_exit(NULL);
        }

        printf("A");
        fflush(NULL);

        if(sem_post(&b)!=0){
            printf("sem_post error\n");
            printf("errno value= %d, definido como %s\n", errno, strerror(errno));
            pthread_exit(NULL);
        }
        //printf("A: he desbloqueado b\n");
    }

    pthread_exit(NULL);
}

void * escribirB(void * p){
    int i;

    for(i=0; i<5; i++){
        //printf("B: bloqueo b\n");
        if(sem_wait(&b)!=0){
            printf("sem_wait error\n");
            printf("errno value= %d, definido como %s\n", errno, strerror(errno));
            pthread_exit(NULL);
        }

        printf("B");
        fflush(NULL);

        if(sem_post(&a)!=0){
            printf("sem_post error\n");
            printf("errno value= %d, definido como %s\n", errno, strerror(errno));
            pthread_exit(NULL);
        }
        //printf("B: he desbloqueado a\n");
    }

    pthread_exit(NULL);
}

int main(){
    pthread_t tid_a, tid_b;

    if(sem_init(&a, 0, 0)!=0){
        printf("sem_init error\n");
        printf("errno value= %d, definido como %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(sem_init(&b, 0, 1)!=0){
        printf("sem_init error\n");
        printf("errno value= %d, definido como %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

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