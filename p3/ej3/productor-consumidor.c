#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
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
    srand(time(NULL));

    for(i=0; i<10; i++){
        sem_wait(&pr); //Hay un espacio disponible menos
        sem_wait(&s); //Bloqueamos semáforo de la sc
        
        //printf("Productor: accediendo a buffer, i=%d, buffin=%d\n", i, buffin);
        buffer[buffin]=rand()%11;
        buffin=(buffin+1)%TAM_BUFFER;
        printf("Buffer modificado: \n");
        imprimirVector(buffer, TAM_BUFFER);

        sem_post(&s); //Desbloqueamos el semáforo de la sc
        sem_post(&cn); //Aumenta el número de elementos a leer
    }

    pthread_exit(NULL);
}

void * consumidor(void * arg){
    int i;
    int * num = malloc(sizeof(int) * 10);

    for(i=0; i<10; i++){
        sem_wait(&cn); //Hay un espacio menos que leer
        sem_wait(&s); //Bloqueamos semáforo de la sc
        
        //printf("Consumidor: accediendo a buffer, i=%d, buffin=%d\n", i, buffin);
        num[i]=buffer[buffout];
        buffout=(buffout+1)%TAM_BUFFER;

        sem_post(&s); //Desbloqueamos el semáforo de la sc
        sem_post(&pr); //Aumenta el número de espacios disponibles para escribir
    }

    pthread_exit((void *) num);
}

int main(){
    pthread_t tid_p, tid_c;
    int * num = malloc(sizeof(int)*10);

    sem_init(&s, 0, 1);
    sem_init(&cn, 0, 0);
    sem_init(&pr, 0, TAM_BUFFER);

    printf("Creando hilo de productor\n");
    pthread_create(&tid_p, NULL, (void *) productor, NULL);
    printf("Creando hilo de consumidor\n");
    pthread_create(&tid_c, NULL, (void *) consumidor, NULL);

    pthread_join(tid_p, NULL);
    pthread_join(tid_c, (void **) &num);

    printf("Lecturas del consumidor: \n");
    imprimirVector(num, 10);


    return 0;
}