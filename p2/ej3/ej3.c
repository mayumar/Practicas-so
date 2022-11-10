#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct vector_dividido{
    int * v;
    int inicio;
    int final;
};

void rellenar_vector(int * v, int n){
    time_t t;
    srand(time(&t));
    int i;

    for(i=0; i<n; i++){
        v[i]=rand()%9+1;
    }

    printf("El vector es: \n");
    for(i=0; i<n; i++){
        printf("%d ", v[i]);
    }

    printf("\n");

}

void * sum_vector(void * arg){
    struct vector_dividido * datos_vector=(struct vector_dividido *) arg;
    int * sum = malloc(sizeof(int));
    int i;
    *sum=0;

    printf("Hilo hijo: soy el hijo que empieza en %d y acaba en %d\n", datos_vector->inicio, datos_vector->final);

    for(i=datos_vector->inicio; i<=datos_vector->final; i++){
        *sum+=datos_vector->v[i];
    }

    printf("Hilo hijo (i=%d, f=%d): resultado de la suma: %d\n",datos_vector->inicio, datos_vector->final, *sum);

    pthread_exit((void *) sum);

}



int main(int argc, char ** argv){
    
    if(argc<2){
        fprintf(stderr, "Error, faltan argumentos: elija 2 o 5");
        exit(EXIT_FAILURE);
    }

    int nhilos=atoi(argv[1]), i;
    struct vector_dividido vector_struct[nhilos];
    int * v=malloc(sizeof(int)*10);
    pthread_t tid[nhilos];

    if(nhilos!=2 && nhilos!=5){
        fprintf(stderr, "Error, valor incorrecto: elija 2 o 5");
        exit(EXIT_FAILURE);
    }

    rellenar_vector(v, 10);

    int inicio=0;
    int final=(10/nhilos)-1;

    for(i=0; i<nhilos; i++){
        
        vector_struct[i].v=v;

        vector_struct[i].inicio=inicio;
        inicio+=(10/nhilos);

        vector_struct[i].final=final;
        final+=(10/nhilos);

    }

    int * sum[nhilos];
    int total=0;

    for(i=0; i<nhilos; i++){
        if(pthread_create(tid+i, NULL, (void *) sum_vector, vector_struct+i)){
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Hilo principal: hilos creados, esperando su finalización\n");

    for(i=0; i<nhilos; i++){
        if(pthread_join(tid[i], (void **) sum+i)){
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);
        }
        total+=*sum[i];
    }

    printf("Hilo principal: la suma de los valores devueltos es: %d\n", total);


    return 0;
}