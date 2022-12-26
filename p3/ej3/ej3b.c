#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define TAM_BUFFER 3
#define N_PRODUCTORES 3
#define N_CONSUMIDORES 3

pthread_mutex_t s = PTHREAD_MUTEX_INITIALIZER;
sem_t cn;
sem_t pr;
int buffer[TAM_BUFFER];
int buffin=0, buffout=0;

void imprimirVector(int * v, int n){
    int i;

    for(i=0; i<n; i++){
        printf("%d ", v[i]);
    }
    printf("\n");

}

void * productor(void * arg){
    
    if(sem_wait(&pr)!=0){
        printf("sem_wait error\n");
        printf("errno value= %d, definido como %s\n", errno, strerror(errno));
        pthread_exit(NULL);
    }

    if(pthread_mutex_lock(&s)!=0){
        printf("Mutex lock error...\n");
        pthread_exit(NULL);
    }

    //printf("Productor: accediendo a buffer, buffin=%d\n", buffin);
    buffer[buffin]=rand()%11;
    buffin=(buffin+1)%TAM_BUFFER;
    printf("Buffer modificado: \n");
    imprimirVector(buffer, TAM_BUFFER);

    if(pthread_mutex_unlock(&s)!=0){
        printf("Mutex unlock error...\n");
        pthread_exit(NULL);
    }

    if(sem_post(&cn)!=0){
        printf("sem_post error\n");
        printf("errno value= %d, definido como %s\n", errno, strerror(errno));
        pthread_exit(NULL);
    }

    pthread_exit(NULL);
}

void * consumidor(void * arg){
    int * num = malloc(sizeof(int));

    if(sem_wait(&cn)!=0){
        printf("sem_wait error\n");
        printf("errno value= %d, definido como %s\n", errno, strerror(errno));
        pthread_exit(NULL);
    }

    if(pthread_mutex_lock(&s)!=0){
        printf("Mutex lock error...\n");
        pthread_exit(NULL);
    }

    *num=buffer[buffout];
    //printf("Consumidor: accediendo a buffer, num= %d, buffout= %d\n", *num, buffout);
    buffout=(buffout+1)%TAM_BUFFER;

    if(pthread_mutex_unlock(&s)!=0){
        printf("Mutex unlock error...\n");
        pthread_exit(NULL);
    }

    if(sem_post(&pr)!=0){
        printf("sem_post error\n");
        printf("errno value= %d, definido como %s\n", errno, strerror(errno));
        pthread_exit(NULL);
    }

    pthread_exit((void *) num);
}

int main(){
    pthread_t tid_p[N_PRODUCTORES], tid_c[N_CONSUMIDORES];
    int ** num = malloc(sizeof(int*)*N_CONSUMIDORES);
    int i;
    srand(time(NULL));

    if(sem_init(&cn, 0, 0)!=0){
        printf("sem_init error\n");
        printf("errno value= %d, definido como %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(sem_init(&pr, 0, TAM_BUFFER)!=0){
        printf("sem_init error\n");
        printf("errno value= %d, definido como %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("Creando hilos de productores\n");
    for(i=0; i<N_PRODUCTORES; i++){
        if(pthread_create(tid_p+i, NULL, (void *) productor, NULL)){
            printf("Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Creando hilos de consumidores\n");
    for(i=0; i<N_CONSUMIDORES; i++){
        if(pthread_create(tid_c+i, NULL, (void *) consumidor, NULL)){
            printf("Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for(i=0; i<N_PRODUCTORES; i++){
        if(pthread_join(tid_p[i], NULL)){
            printf("Error joining thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for(i=0; i<N_CONSUMIDORES; i++){
        if(pthread_join(tid_c[i], (void**) num+i)){
            printf("Error joining thread\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Lecturas del consumidor: \n");
    
    for(i=0; i<N_CONSUMIDORES; i++){
        printf("%d ", **(num+i));
    }
    printf("\n");

    return 0;
}