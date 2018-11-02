#include <string.h>
  #include <ctype.h>
  #include  <sys/shm.h>
  #include <sys/types.h>
  #include <sys/wait.h>
  #include <unistd.h>
  #include <time.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <signal.h>
  #include <stdbool.h>
  #include <time.h>
  #include <semaphore.h>

struct Memory {
     int MaxClaims;
     int  seconds;
     long long int  nanoseconds;
     int TerminatedProc[2];
     int processID;
     int RequestID;
     int Requests[4];
     //int childCount;
     int termNum;
};

struct Memory  *shmPTR;
int main() {
key_t ShmKEY;
   //int quantum = 2000;
   //int terminated = 0;
   int value = 0;
   long int getrand = getpid();
   int boundmil = 0;
   int milliseconds = 0;
   //int NextProcess = -2;
    int ShmID;
   sem_t *sem;
   long long int x = 0;
   //long long int x = 0;
   //   int flag = 0;
   //
        int processID = 0;
               
                              ShmKEY = ftok(".",'x');
                                                   ShmID = shmget(ShmKEY, sizeof(struct Memory), 0666);
                                                                           if (ShmID < 0){
                                                                                                        printf("*** shmget error (client) ***\n");
                                                                                                                                         exit(1);
                                                                                                                                                                              }
                                                                                                                                                                                                                       shmPTR = (struct Memory *) shmat(ShmID, NULL, 0);
                                                                                                                                                                                                                                                                   if(ShmID == -1){
                                                                                                                                                                                                                                                                                                                    printf("*** shmat error(client) ***\n");
                                                                                                                                                                                                                                                                                                                                                                          exit(1);
                                                                                                                                                                                                                                                                                                                                                                                                                                      }
     shmPTR->processID = (int)getpid(); 
     srand(getrand++);
     value = 1 + (rand()%100); 
      boundmil =  1000*shmPTR->seconds + (int)(shmPTR->nanoseconds/1000000) + value;
     //fprintf(stderr, "max claim is %d\n", shmPTR->MaxClaims);
     processID = shmPTR->processID; 
     fprintf(stderr,"My process id is %d\n", shmPTR->processID);
     while(true){  
            milliseconds = 1000*shmPTR->seconds + (int)(shmPTR->nanoseconds/1000000);
     //       fprintf(stderr,"milliseconds is %d", milliseconds);
            if((shmPTR->Requests[0] == -2)&&(milliseconds >= boundmil)) {
                //fprintf(stderr,"milliseconds is %d\n", milliseconds);
            shmPTR->Requests[0] = 0;  shmPTR->RequestID = processID;
             sem = sem_open("sem1004", 1);sem_post(sem), sem_close(sem);
             srand(getrand++);
            value = 1 + (rand()%100);
            
            if(value <= 45) break;
         }
     }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               processID = shmPTR->processID;
      shmPTR->TerminatedProc[shmPTR->termNum] = processID;
      shmPTR->termNum++;
      //fprintf(stderr, "child count is %d\n", shmPTR->childCount);
     // shmPTR->childCount = shmPTR->childCount - 1;
      shmdt((void *) shmPTR);
    exit(0);
}
