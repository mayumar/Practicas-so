#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

void controlador(int sign){
    printf("\n");
    printf("\tRING\n");
    printf("\n");
}

unsigned int f1(int secs){
    alarm(secs);
    pause();
}


int main(){
    int nring=0;

    if(signal(SIGALRM, controlador)==SIG_ERR){
        perror("Signal error");
        printf("errno value= %d\n", errno);
        exit(EXIT_FAILURE);
    }

    printf("Una alarma en 5 segundos...\n");
    f1(5);
    printf("Una alarma en 3 segundos...\n");
    f1(3);

    printf("Una alarma cada segundo...\n");
    while(1){
        f1(1);
        nring++;

        if(nring==4){
            kill(getpid(), SIGKILL);
        }

    }

    return 0;
}