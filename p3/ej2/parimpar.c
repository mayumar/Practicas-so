#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mtxpar = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtximpar = PTHREAD_MUTEX_INITIALIZER;
int par=0, impar=0;

void * suma(void * arg){
    srand(time(NULL));
    int n=*(int *)arg;
    int * sum = malloc(sizeof(int));
    int i, num;
    *sum=0;

    if(n%2==0){

        if(pthread_mutex_lock(&mtxpar)!=0){
            printf("Mutex lock error...\n");
            pthread_exit(NULL);
        }

        for(i=0; i<5; i++){
            num=rand()%11;
            par+=num;
            *sum+=num;
        }

        if(pthread_mutex_unlock(&mtxpar)!=0){
            printf("Mutex unlock error...\n");
            pthread_exit(NULL);
        }

    }else if(n%2!=0){

        if(pthread_mutex_lock(&mtximpar)!=0){
            printf("Mutex lock error...\n");
            pthread_exit(NULL);
        }

        for(i=0; i<5; i++){
            num=rand()%11;
            impar+=num;
            *sum+=num;
        }

        if(pthread_mutex_unlock(&mtximpar)!=0){
            printf("Mutex unlock error...\n");
            pthread_exit(NULL);
        }
    }
    
    pthread_exit((void *) sum);
}

int main(int argc, char ** argv){
    if(argc != 2){
        printf("Error: faltan argumentos, introduzca el numero de hilos a crear\n");
        exit(EXIT_FAILURE);
    }

    int i;
    int n=atoi(argv[1]);
    pthread_t tid[n];
    int ** sum = malloc(sizeof(int*)*n);
    int * orden = malloc(sizeof(int)*n);
    int totalpar=0, totalimpar=0;

    for(i=0; i<n; i++){
        orden[i]=i+1;
        if(pthread_create(tid+i, NULL, (void *) suma, (void *) (orden+i))){
            printf("Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for(i=0; i<n; i++){
        if(pthread_join(tid[i], (void **) sum+i)){
            printf("Error joining thread\n");
            exit(EXIT_FAILURE);
        }
        
        if((i+1)%2==0){
            totalpar+=*sum[i];
        }else if((i+1)%2!=0){
            totalimpar+=*sum[i];
        }

        printf("Main()... La hebra de orden de creacion %d devolvio el valor de la suma total: %d\n", i+1, *sum[i]);

    }

    printf("Main()... Valor de la variable compartida impar: %d\n", impar);
    printf("Main()... Suma de los valores devueltos por las hebras de creación impartes: %d\n", totalimpar);
    printf("\n");
    printf("Main()... Valor de la variable compartida par: %d\n", par);
    printf("Main()... Suma de los valores devueltos por las hebras de creación pares: %d\n", totalpar);


    return 0;
}