#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */
#include <sys/stat.h>  
#define wordslengh 25
using namespace std;
sem_t *sem1,*sem2;

char sem_fn1[] = "my_sem1";
char sem_fn2[]= "my_sem2";

struct protocol {
         int op=0;
	char msg[25];
};


int main(){
    int i=0;
    key_t key = ftok("shmfile",65); 
    int numPalavras = 0;
    char* palavras[50];
    char line[50];
    FILE *arquivo;
    arquivo = fopen("palavras.txt", "r");
    if (arquivo == NULL){
       return EXIT_FAILURE;
       }
    while(fgets(line, sizeof line, arquivo) != NULL)
    {
        //Adiciona cada palavra no vetor
        palavras[i] = strdup(line);
        //removemos o \n final
        palavras[i][strlen(palavras[i])-1]='\0';
        i++;
        //Conta a quantidade de palavras
        numPalavras++;
    }
    
 
//    inicializado um named semaphore
 sem_close(sem1);
    sem_close(sem2);
    sem_unlink(sem_fn1);
    sem_unlink(sem_fn2);
sem1 = sem_open(sem_fn1, O_CREAT, 0666, 0);
 if(sem1 == (void*)-1){
   perror("sem_open failure");
 }
 sem2 = sem_open(sem_fn2, O_CREAT, 0666, 0);
 if(sem2 == (void*)-1){
   perror("sem_open failure");
 }

//  shmget returns an identifier in shmid
    int shmid = shmget(key,1024,0666|IPC_CREAT);

//     shmat to attach to shared memory
    protocol *protocolo = (protocol*)shmat(shmid,(void*)0,0);
    protocolo->op=1;
    cout<<"Para jogar voce precisa iniciar o programa cliente \n";
    i=0;
    while(i<numPalavras){
    strcpy(protocolo->msg,palavras[i]);
    i++;
  if(protocolo->op==2){
        printf("processo cliente finalizado");
        break;
    }
    sem_post(sem1);
    sem_wait(sem2);
    }
    //detach from shared memory
    shmdt(protocolo);
    // destroy the shared memory
    shmctl(shmid,IPC_RMID,NULL);
    //antes de fechar o programa destroi o semaforo
    sem_close(sem1);
    sem_close(sem2);
    sem_unlink(sem_fn1);
    sem_unlink(sem_fn2);
    return 0;
}

