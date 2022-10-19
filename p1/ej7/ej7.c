#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

void tratarSennal(int sign){
    printf("\nSeñal recibida\n");
    printf("Valor recibido: %d\n\n", sign);
}

int main(){
    pid_t pid, childpid;
    int i, status;

    pid=fork();

    switch(pid){
        case -1:

            perror("fork error");
            printf("errno value = %d\n", errno);
            exit(EXIT_FAILURE);

        case 0:
            
            if(signal(SIGUSR1, tratarSennal)==SIG_ERR){
                perror("signal error");
                printf("errno value = %d\n", errno);
                exit(EXIT_FAILURE);
            }

            while(1){
                pause();
            }
            

        break;

        default:
            printf("Soy el padre con PID %d y mi hijo con PID %d\n", getpid(), pid);

            for(i=0; i<5; i++){
                
                kill(pid, SIGUSR1);
                sleep(1);

            }

            kill(pid, SIGKILL);

            while((childpid=wait(&status))>0){
                if(WIFEXITED(status)){
                    printf("Proceso padre %d, hijo con PID %d finalizado, status = %d\n", getpid(), childpid, WEXITSTATUS(status));
                }else if(WIFSIGNALED(status)){
                    printf("Proceso padre %d, hijo con PID %d finalizado al recibir la señal %d\n", getpid(), childpid, WTERMSIG(status));
                }
            }

            if(childpid==(pid_t)-1 && errno==ECHILD){
                printf("Proceso padre %d, no hay más hijos que esperar. Valor errno= %d, definido: %s\n", getpid(), errno, strerror(errno));
            }else{
                printf("Error en la invocacion de wait. Valor de errno = %d, definido como: %s\n", errno, strerror(errno));
            }

    }

    return 0;
}