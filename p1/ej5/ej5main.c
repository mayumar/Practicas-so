#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char ** argv){
    pid_t pid=1, childpid;
    int status, i, var=0;

    if(argc<2){
        printf("Error: intoduzca el numero de procesos hijos a crear\n");
        exit(EXIT_FAILURE);
    }

    int nprocess=atoi(argv[1]);

    for(i=0; i<nprocess && pid!=0; i++){

        pid=fork();

        switch(pid){
            case -1: //error

                perror("fork error\n");
                printf("errno value= %d", errno);
                exit(EXIT_FAILURE);


            case 0: //hijo

                printf("Soy el Hijo, mi PID es %d y el PPID de mi padre es %d\n", getpid(), getppid());
                var++;
                exit(EXIT_SUCCESS);

        }

    }


    printf("Soy el padre var=%d\n", var);
    
    while((childpid=wait(&status))>0){
        if(WIFEXITED(status)){
            printf("Proceso padre %d, hijo con PID %d finalizado, status = %d\n", getpid(), childpid, WEXITSTATUS(status));
        }else if(WIFSIGNALED(status)){
            printf("Proceso padre %d, hijo con PID %d finalizado al recibir la señal %d\n", getpid(), childpid, WTERMSIG(status));
        }
    }

    if(errno==ECHILD){
        printf("Proceso padre %d, no hay más hijos que esperar. Valor errno= %d, definido: %s\n", getpid(), errno, strerror(errno));
    }else{
        printf("Error en la invocacion de wait. Valor de errno = %d, definido como: %s\n", errno, strerror(errno));
    }

    return 0;
}