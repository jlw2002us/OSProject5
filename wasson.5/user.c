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
     long long int  milliseconds;
     int TerminatedProc[2];
     int processID;
     int childCount;
     long int Requests;
     int termNum;
};

struct Memory  *shmPTR;
int main() {
key_t ShmKEY;
   int quantum = 2000;
   int terminated = 0;
   int value = 0;
   int NextProcess = -2;
    int ShmID;
   sem_t *sem;
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
     fprintf(stderr, "%d\n", shmPTR->MaxClaims);
     fprintf(stderr,"My process id is %d\n", shmPTR->processID);
     while(true){
        if(shmPTR->Requests == 0) {
                    
            shmPTR->Requests = getpid();
            sem = sem_open("sem1004", 1), sem_post(sem), sem_close(sem);
            break;
         }
    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               processID = shmPTR->processID;
      shmPTR->TerminatedProc[shmPTR->termNum] = processID;
      shmPTR->termNum++;
      shmPTR->childCount--;
      shmdt((void *) shmPTR);
    exit(0);
}
