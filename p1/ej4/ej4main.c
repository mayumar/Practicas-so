#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char ** argv){
    int num1, num2, i, status;
    pid_t pid, childpid;

    if(argc<3){
        printf("Error: faltan argumentos\n");
        printf("Introduzca dos numeros a calcular el factorial\n");
        exit(EXIT_FAILURE);
    }

    for(i=0; i<2; i++){
        pid=fork();

        switch(pid){
            case -1:
                perror("fork error");
                printf("errno value = %d\n", errno);
                exit(EXIT_FAILURE);

            case 0:

                if(i==0){
                    
                    printf("Soy el hijo numero %d, mi PID es %d y el PID de mi padre es %d\n", i+1, getpid(), getppid());
                    execlp("./factorial.exe", "./factorial.exe", argv[1], NULL);

                }else if(i==1){
                    
                    sleep(1);
                    printf("Soy el hijo numero %d, mi PID es %d y el PID de mi padre es %d\n", i+1, getpid(), getppid());
                    execlp("./factorial.exe", "./factorial.exe", argv[2], NULL);

                }

            exit(EXIT_FAILURE);

        }

    }

    while((childpid=wait(&status))>0){
        if(WIFEXITED(status)){
            printf("Proceso padre %d, hijo con PID %d finalizado, status = %d\n", getpid(), childpid, WEXITSTATUS(status));
        }else if(WIFSIGNALED(status)){
            printf("Proceso padre %d, hijo con PID %d finalizado al recibir la señal %d\n", getpid(), childpid, WTERMSIG(status));
        }
    }

    if(errno==ECHILD){
        printf("Proceso padre %d, no hay mas hijos que esperar. Valor errno = %d, definido como: %s\n", getpid(), errno, strerror(errno));
    }else{
        printf("Error en la invocacion de wait. Valor de errno = %d, definido como: %s\n", errno, strerror(errno));
    }


    return 0;
}