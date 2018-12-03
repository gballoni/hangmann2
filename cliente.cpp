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
#include <list>
#define wordslengh 25
sem_t *mutex, *item;
sem_t *sem1, *sem2;
char sem_fn1[] = "my_sem1";
char sem_fn2[] = "my_sem2";
char chute;
int flag;
using namespace std;
struct wordfish{
    char underscore[25];
    char word[25];
    int letras;
    int visiveis;
};
wordfish palavra;
//protocolo de troca de mensagens em memoria compartilhada
struct protocol {
         int op;
	char msg[25];
};
protocol *protocolo;
//struct que guarda a palavra e underscores
void carrega_palavra(){
	int i;
	printf("Data read from memory: %s\n",protocolo->msg);
	palavra.letras=0;
    strcpy(palavra.word,protocolo->msg);
    for(i=0;palavra.word[i]!='\0';i++){
    	palavra.underscore[i]='*';
    	palavra.letras++;
    }
    palavra.underscore[i]='\0';

}
int main(){
    int i,contador;
    int op =1;
    // ftok to generate unique key
    key_t key = ftok("shmfile",65);
    
    // shmget returns an identifier in shmid
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    //conecta ao semaforo
 /* Open the Semaphore */

 sem1 = sem_open(sem_fn1, 0, 0666, 0);

 if(sem1 == (void*) -1){
   perror("sem_open failure");

 }
sem2 = sem_open(sem_fn2, 0, 0666, 0);

 if(sem2 == (void*) -1){
   perror("sem_open failure");

 }


    // shmat to attach to shared memory
 	protocolo = (protocol*) shmat(shmid,(void*)0,0);
    protocolo->op=1;  
printf("conectado\n");
    while(1){
   	contador=0;
    sem_wait(sem1);
    printf("Iniciar novo jogo?\n");
    printf("1) para sim\n");
    printf("2) para nao\n");
    scanf("%d",&op);
//
    if(op==2){
        protocolo->op=2;
        sem_post(sem2);
        exit(0);
   
    }
    //carrega apalvra da memoria compartilhada para a memoria local
    carrega_palavra();
  //  scanf("%d",&op);
    sem_post(sem2);
    while(contador<=6){
  		system("clear");
                   if(palavra.letras==0){
	        	      printf("WIN \n");
	        	      break;
	        }
	    switch(contador)
	                {
                  case 0: 
	                  printf("___________\n");
	                  printf("     |     \n");
	                  printf("           \n");
	                  printf("           \n");
	                  printf("           \n");
	                  break;
                  case 1: 
                  	 printf("___________\n");
	                  printf("     |     \n");
	                  printf("     O     \n");
	                  printf("           \n");
	                  printf("           \n");
	                  break;
                  case 2: 
                  	 printf("___________\n");
	                  printf("     |     \n");
	                  printf("     O     \n");
	                  printf("     |     \n");
	                  printf("           \n");
	                  break;
                  case 3: 
                  	 printf("___________\n");
	                  printf("     |     \n");
	                  printf("     O     \n");
	                  printf("    /|     \n");
	                  printf("           \n");
	                  break;
                 case 4: 
                  	 printf("___________\n");
	                  printf("     |     \n");
	                  printf("     O     \n");
	                  printf("    /|\\    \n");
	                  printf("           \n");
	                  break;
                  case 5: 
                  	 printf("___________\n");
	                  printf("     |     \n");
	                  printf("     O     \n");
	                  printf("    /|\\    \n");
	                  printf("      \\    \n");
	                  break;
	              case 6: 
                  	 printf("___________\n");
	                  printf("     |     \n");
	                  printf("     O     \n");
	                  printf("    /|\\    \n");
	                  printf("    / \\    \n");
	                  break;
	                 }
	       	printf("%s\n",palavra.underscore);
	       	printf("erros:%d \n",contador);
	        printf("chute uma letra \n");
	        scanf("\n%c",&chute);
	        flag=0;
	        for (int i = 0; palavra.word[i]!='\0'; i++){
	        	if(palavra.word[i]==chute){
	        		flag=1;
	        		palavra.underscore[i]=chute;
	        		palavra.letras--;
	        	}
	        }
	        //incrementa contador
	        if(flag==0){
	        	contador++;
	        }
	      
	        if(contador==6){
	        	printf("GAME OVER\n");
	        }
    }

   // detach from shared memory
    }
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
