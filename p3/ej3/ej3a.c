#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define TAM_BUFFER 5

sem_t s;
sem_t cn; //Numero de elementos que leer
sem_t pr; //Numero de espacios libres para escribir
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
    int i;

    for(i=0; i<10; i++){
        if(sem_wait(&pr)!=0){ //Hay un espacio disponible menos
            printf("sem_wait error\n");
            printf("errno value= %d, definido como %s\n", errno, strerror(errno));
            pthread_exit(NULL);
        }

        if(sem_wait(&s)!=0){ //Bloqueamos semáforo de la sc
            printf("sem_wait error\n");
            printf("errno value= %d, definido como %s\n", errno, strerror(errno));
            pthread_exit(NULL);
        }
        
        //printf("Productor: accediendo a buffer, i=%d, buffin=%d\n", i, buffin);
        buffer[buffin]=rand()%11;
        buffin=(buffin+1)%TAM_BUFFER;
        printf("Buffer modificado: \n");
        imprimirVector(buffer, TAM_BUFFER);

        if(sem_post(&s)!=0){ //Desbloqueamos el semáforo de la sc
            printf("post_wait error\n");
            printf("errno value= %d, definido como %s\n", errno, strerror(errno));
            pthread_exit(NULL);
        }

        if(sem_post(&cn)!=0){ //Aumenta el número de elementos a leer
            printf("sem_post error\n");
            printf("errno value= %d, definido como %s\n", errno, strerror(errno));
            pthread_exit(NULL);
        }
    }

    pthread_exit(NULL);
}

void * consumidor(void * arg){
    int i;
    int * num = malloc(sizeof(int) * 10);

    for(i=0; i<10; i++){
        if(sem_wait(&cn)!=0){ //Hay un espacio menos que leer
            printf("sem_wait error\n");
            printf("errno value= %d, definido como %s\n", errno, strerror(errno));
            pthread_exit(NULL);
        }

        if(sem_wait(&s)!=0){ //Bloqueamos semáforo de la sc
            printf("sem_wait error\n");
            printf("errno value= %d, definido como %s\n", errno, strerror(errno));
            pthread_exit(NULL);
        }
        
        //printf("Consumidor: accediendo a buffer, i=%d, buffin=%d\n", i, buffin);
        num[i]=buffer[buffout];
        buffout=(buffout+1)%TAM_BUFFER;

        if(sem_post(&s)!=0){ //Desbloqueamos el semáforo de la sc
            printf("sem_post error\n");
            printf("errno value= %d, definido como %s\n", errno, strerror(errno));
            pthread_exit(NULL);
        }

        if(sem_post(&pr)!=0){ //Aumenta el número de espacios disponibles para escribir
            printf("sem_post error\n");
            printf("errno value= %d, definido como %s\n", errno, strerror(errno));
            pthread_exit(NULL);
        }
    }

    pthread_exit((void *) num);
}

int main(){
    pthread_t tid_p, tid_c;
    int * num = malloc(sizeof(int)*10);
    srand(time(NULL));

    sem_init(&s, 0, 1);
    sem_init(&cn, 0, 0);
    sem_init(&pr, 0, TAM_BUFFER);

    printf("Creando hilo de productor\n");
    if(pthread_create(&tid_p, NULL, (void *) productor, NULL)){
        printf("Error creating thread\n");
        exit(EXIT_FAILURE);
    }

    printf("Creando hilo de consumidor\n");
    if(pthread_create(&tid_c, NULL, (void *) consumidor, NULL)){
        printf("Error creating thread\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_join(tid_p, NULL)){
        printf("Error joining thread\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_join(tid_c, (void **) &num)){
        printf("Error joining thread\n");
        exit(EXIT_FAILURE);
    }

    printf("Lecturas del consumidor: \n");
    imprimirVector(num, 10);


    return 0;
}