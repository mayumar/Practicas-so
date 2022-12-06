#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
int * camisetas;

void rellenaVector(int * v, int n){
    time_t t;
    srand(time(&t));
    int i;

    for(i=0; i<n; i++){
        v[i]=rand()%101;
    }

    printf("Stock de %d tipos de camisetas antes de abrir la tienda:\n", n);
    
    for(i=0; i<n; i++){
        printf("%d ", v[i]);
    }
    printf("\n\n");

}

void * cliente(void * arg){
    int tam=*((int *)arg);
    int modelo=rand()%tam;
    int cantidad=rand()%11;

    if(pthread_mutex_lock(&mtx)!=0){
        printf("Mutex lock error...\n");
        pthread_exit(NULL);
    }

    printf("<-- Cliente %lu, compro %d unidades de la camiseta %d\n", pthread_self(), cantidad, modelo);

    camisetas[modelo]-=cantidad;
    if(camisetas[modelo]<0){
        camisetas[modelo]=0;
    }

    if(pthread_mutex_unlock(&mtx)!=0){
        printf("Mutex unlock error...\n");
        pthread_exit(NULL);
    }
    
    pthread_exit(NULL);
}

void * proveedor(void * arg){
    int tam=*((int *)arg);
    int modelo=rand()%tam;
    int cantidad=rand()%11;

    if(pthread_mutex_lock(&mtx)!=0){
        printf("Mutex lock error...\n");
        pthread_exit(NULL);
    }

    printf("--> Proveedor %lu, suministro %d unidades de la camiseta %d\n", pthread_self(), cantidad, modelo);

    camisetas[modelo]+=cantidad;

    if(pthread_mutex_unlock(&mtx)!=0){
        printf("Mutex unlock error...\n");
        pthread_exit(NULL);
    }
    
    pthread_exit(NULL);
}

int main(int argc, char ** argv){
    if(argc<3){
        printf("Error: indique el numero de clientes y proveedores\n");
        exit(EXIT_FAILURE);
    }

    int clientes=atoi(argv[1]);
    int proveedores=atoi(argv[2]);
    int tam=proveedores;
    int i;
    camisetas=malloc(sizeof(int)*tam);
    rellenaVector(camisetas, tam);
    pthread_t tid_clientes[clientes], tid_proveedores[proveedores];

    for(i=0; i<clientes; i++){
        if(pthread_create(tid_clientes+i, NULL, (void *) cliente, &tam)){
            printf("Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for(i=0; i<proveedores; i++){
        if(pthread_create(tid_proveedores+i, NULL, (void *) proveedor, &tam)){
            printf("Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("MAIN - Esperando a las hebras clientes y proveedores creadas...\n");

    for(i=0; i<clientes; i++){
        if(pthread_join(tid_clientes[i], NULL)){
            printf("Error joining thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for(i=0; i<proveedores; i++){
        if(pthread_join(tid_proveedores[i], NULL)){
            printf("Error joining thread\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("\nStock de %d tipos de camisetas al cerrar la tienda:\n", tam);
    for(i=0; i<tam; i++){
        printf("%d ", camisetas[i]);
    }
    printf("\n");

    
    return 0;
}