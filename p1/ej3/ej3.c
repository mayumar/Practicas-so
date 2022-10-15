#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include "ej3hijos.h"

int main(int argc, char ** argv){
    pid_t pid, childpid;
    int i, status;

    if(argc<2){
        printf("Error, faltan argumentos\n");
        printf("Se debe indicar el nombre de los ficheros\n");
        exit(EXIT_FAILURE);
    }

    for(i=0; i<2; i++){
        pid=fork();

        switch(pid){
            case -1: //ERROR

                perror("fork error");
                printf("erro value = %d\n", errno);
                exit(EXIT_FAILURE);

            case 0: //HIJO
            
                if(i==0){
                    hijo1();
                }else if(i==1){
                    hijo2(argv);
                }
                exit(EXIT_SUCCESS);
        }

    }

    printf("Proceso padre con PID %d\n", getpid());

    while((childpid=wait(&status))>0){
        if(WIFEXITED(status)){
            printf("Proceso padre %d, hijo con PID %d finalizado, status = %d\n", getpid(), childpid, WEXITSTATUS(status));
        }else if(WIFSIGNALED(status)){
            printf("Proceso padre %d, hijo con PID %d finalizado al recibir la señal %d\n", getpid(), childpid, WTERMSIG(status));
        }
    }

    if(errno==ECHILD){
        printf("Proceso padre %d, no hay más hijos que esperar. Valor de errno = %d, definido como: %s\n", getpid(), errno, strerror(errno));
    }else{
        printf("Error en la invocacion de wait. Valor errno = %d, definido como: %s\n", errno, strerror(errno));
    }

}


//calculadora -> gnome-calculator
//ficheros -> gedit fichero1.txt