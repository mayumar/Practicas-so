#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>

int main(){
    pid_t pid, childpid;
    int status;

    pid=fork();

    switch(pid){
        case -1:

            perror("fork error\n");
            printf("errno value = %d\n", errno);
            exit(EXIT_FAILURE);

        case 0:

            printf("Soy el Hijo, mi PID es %d y el PPID de mi padre es %d\n", getpid(), getppid());
            exit(EXIT_SUCCESS);

        default:

            printf("Proceso padre %d pasa a sleep\n", getpid());
            sleep(5);
            printf("Proceso Padre pasa a wait\n");
            
            while((childpid=wait(&status))>0){
                sleep(5);
                if(WIFEXITED(status)){
                    printf("Proceso Padre %d, hijo con PID %d finalizado, status = %d\n", getpid(), childpid, WEXITSTATUS(status));
                }else if(WIFSIGNALED(status)){
                    printf("Proceso Padre %d, hijo con PID %d finalizado al recibir la señal %d\n", getpid(), childpid, WTERMSIG(status));
                }
            }

            sleep(5);
            
            exit(EXIT_SUCCESS);

    }

    return 0;

}