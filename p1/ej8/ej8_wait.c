#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

void mostrarPID(int signal){
    printf("Mi PID es %d\n", getpid());
}

int main(){
    
    printf("Esperando la señal...\n");

    if(signal(SIGUSR1, mostrarPID)==SIG_ERR){
        perror("signal error");
        printf("errno value = %d\n", errno);
        exit(EXIT_FAILURE);
    }

    

    while(1){
        pause();
    }

    return 0;
}