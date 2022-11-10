#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct vector_dividido{
    int * v;
    int inicio;
    int final;
};

int main(int argc, char ** argv){
    
    if(argc<2){
        fprintf(stderr, "Error, faltan argumentos: elija 2 o 5");
        exit(EXIT_FAILURE);
    }

    int nhilos=atoi(argv[1]);
    struct vector_dividido vector_struct[nhilos];
    int v[10], i;

    if(nhilos!=2 && nhilos!=5){
        fprintf(stderr, "Error, valor incorrecto: elija 2 o 5");
    }

    int inicio=0;
    int final=(10/nhilos)-1;

    for(i=0; i<nhilos; i++){
        
        vector_struct[i].v=v;

        vector_struct[i].inicio=inicio;
        inicio+=(10/nhilos);

        vector_struct[i].final=final;
        final+=(10/nhilos);

    }


    return 0;
}