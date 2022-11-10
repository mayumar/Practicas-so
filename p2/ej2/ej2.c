#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void * contar_lineas(void * arg){
    FILE * nombfich;
    int * n_lineas = malloc(sizeof(int));
    char linea[100];

    if((nombfich=fopen(arg, "r"))==NULL){
        fprintf(stderr, "Error opening file %s\n", arg);
        exit(EXIT_FAILURE);
    }


    while(fgets(linea, 100, nombfich)!=NULL){
        (*n_lineas)++;
    }

    printf("\nHilo hijo: el fichero %s tiene %d lineas\n", arg, *n_lineas);

    fclose(nombfich);

    pthread_exit((void *) n_lineas);

}

int main(int argc, char ** argv){
    
    if(argc<2){
        printf("Error, faltan argumentos: escriba los ficheros a abrir\n");
        exit(EXIT_FAILURE);
    }
    
    int n_ficheros=argc-1, i, total=0;
    pthread_t tid[n_ficheros];
    int * n_lineas[n_ficheros];

    for(i=0; i<n_ficheros; i++){
        if(pthread_create(tid+i, NULL, (void *) contar_lineas, (void *) argv[i+1])){
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("\nHilo principal: Hilos creados, esperando su finalización\n");

    for(i=0; i<n_ficheros; i++){
        if(pthread_join(tid[i], (void **) n_lineas+i)){ //(void **) &(n_lineas[i])
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);
        }
        total+=*n_lineas[i];
    }

    printf("\nHilo principal: la suma de las lineas de los ficheros es: %d\n", total);
    
    return 0;
}