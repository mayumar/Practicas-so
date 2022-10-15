#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char ** argv){
    pid_t pid=1, childpid;
    int status, i;

    for(i=0; i<2; i++){   
        pid=fork();

        switch(pid){
            case -1: //error

                perror("fork error\n");
                printf("errno value= %d", errno);
                exit(EXIT_FAILURE);


            case 0: //hijo

                printf("Soy el Hijo, mi PID es %d y el PPID de mi padre es %d\n", getpid(), getppid());

            break;

            default: //padre

                while((childpid=wait(&status))>0){
                    if(WIFEXITED(status)){
                        printf("Proceso padre %d, hijo con PID %d finalizado, status = %d\n", getpid(), childpid, WEXITSTATUS(status));
                    }else if(WIFSIGNALED(status)){
                        printf("Proceso padre %d, hijo con PID %d finalizado al recibir la señal %d\n", getpid(), childpid, WTERMSIG(status));
                    }
                }

                if(childpid==-1 && errno==ECHILD){
                    printf("Proceso padre %d, no hay más hijos que esperar. Valor errno= %d, definido: %s\n", getpid(), errno, strerror(errno));
                }else{
                    printf("Error en la invocacion de wait. Valor de errno = %d, definido como: %s\n", errno, strerror(errno));
                }

        }
    }

    

    return 0;
}