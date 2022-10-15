#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){
    int num, i;
    long fact=1;

    if(argc<2){
        printf("Error: faltan argumentos\n");
        printf("Introduzca el numero a calcular el factorial\n");
        exit(EXIT_FAILURE);
    }

    num=atoi(argv[1]);

    for(i=1; i<=num; i++){
        fact*=i;
    }

    printf("El factorial de %d es %ld\n", num, fact);
    
    return 0;
}