#include "ej3hijos.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


void hijo1(){
    printf("Proceso hijo con PID %d, mi padre con PPID %d\n", getpid(), getppid());
    execlp("gnome-calculator", "gnome-calculator", NULL);
}

void hijo2(char ** argv){
    printf("Proceso hijo con PID %d, mi padre con PPID %d\n", getpid(), getppid());
    execvp(argv[1], &argv[1]);
}