#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char ** argv){

    if(argc<2){
        printf("\nError faltan argumentos: introduzca el PID del proceso\n");
        exit(EXIT_FAILURE);
    }

    pid_t PID=atoi(argv[1]);

    kill(PID, SIGUSR1);

    sleep(1);
    
    kill(PID, SIGKILL);

    return 0;
}