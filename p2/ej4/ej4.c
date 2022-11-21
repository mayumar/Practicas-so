#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int global=0;

void * incrementa(){
    int i;
    
    printf("Hilo hijo con TID %ld: Incrementando global\n", pthread_self());

    for(i=0; i<1000; i++){
        global++;
    }

    printf("Hilo hijo con TID %ld: global = %d\n", pthread_self(), global);

    pthread_exit(NULL);

}


int main(){
    pthread_t tid1, tid2;

    if(pthread_create(&tid1, NULL, (void * ) incrementa, NULL)){
        fprintf(stderr, "Error creating thread\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_create(&tid2, NULL, (void * ) incrementa, NULL)){
        fprintf(stderr, "Error creating thread\n");
        exit(EXIT_FAILURE);
    }

    printf("Hilo principal: esperando finalización\n");

    if(pthread_join(tid1, NULL)){
        fprintf(stderr, "Error joining thread\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_join(tid2, NULL)){
        fprintf(stderr, "Error joining thread\n");
        exit(EXIT_FAILURE);
    }

    printf("Hilo principal: la variable global es %d\n", global);

    return 0;
}