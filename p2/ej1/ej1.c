#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#define MY_MAX_RAND 100.0


void * random_num(void * arg){

    time_t t;
    srand48(time(&t));
    float * sum = malloc(sizeof(float));
    float num1, num2;

    num1=drand48() * MY_MAX_RAND;
    num2=drand48() * MY_MAX_RAND;

    printf("\nHilo hijo: numeros generados: %.3f, %.3f\n", num1, num2);

    *sum=num1+num2;
    printf("\nHilo hijo: resultado de la suma: %.3f\n", *sum);

    pthread_exit((void *) sum);
}


int main(){
    int i, n=5;
    pthread_t tid[n];
    float * sum[n];
    float * total=malloc(sizeof(float));

    for(i=0; i<n; i++){
        if(pthread_create(tid+i, NULL, (void *) random_num, NULL)){
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }

    printf("\nHilo principal: Hilo creado, esperando su finalización\n");
    
    for(i=0; i<n; i++){
        if(pthread_join(tid[i], (void **) sum+i)){
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);
        }
        *total+=*sum[i];
    }

    printf("Hilo principal: la suma de los valores devueltos es: %.3f\n", *total);


    return 0;
}