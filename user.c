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
     //int ClaimsMatrix[30][22];
     long long int  nanoseconds;
     int TerminatedProc[2];
     int processID;
     int RequestID;
     int Requests[4];
     int termNum;
};

struct Memory  *shmPTR;
int main() {
  key_t ShmKEY;
  int value = 0;
   int MaxClaims = 0;
   long int getrand = getpid();
   int boundmil = 0;
   int milliseconds = 0;
   int i, j;
   int ShmID;
          sem_t *sem;
   //          long long int x = 0;
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
     shmPTR->processID++;
     srand(getrand++);
     MaxClaims = shmPTR->MaxClaims;
     value = 1 + (rand()%100); 
      boundmil =  1000*shmPTR->seconds + (int)(shmPTR->nanoseconds/1000000) + value;
     processID = shmPTR->processID;
      
     //fprintf(stderr, "process %d claims is %d\n",processID, MaxClaims); 
     while(true){  
            milliseconds = 1000*shmPTR->seconds + (int)(shmPTR->nanoseconds/1000000);
     if((shmPTR->Requests[0] == -2)&&(milliseconds >= boundmil)) {
                sem = sem_open("sem1113", 0); sem_wait(sem);
               shmPTR->Requests[0] = 0; srand(getrand++); value = 1 + (rand()%MaxClaims);//make request number
               shmPTR->Requests[1] = value; 
               srand(getrand++); value = 1 + (rand()%5); //make claim of particular resource
               shmPTR->Requests[2] = value; 
               //enter its max claims if it hasn't been already
                 // for(i=0; i<30; i++) { if(shmPTR->ClaimsMatrix[i][0] == processID) break;
                    // if(shmPTR->ClaimsMatrix[i][0] == 0){
                         // shmPTR->ClaimsMatrix[i][0] = processID;
                         // for(j= 1;j< 4;j++) {
                          //   shmPTR->ClaimsMatrix[i][j] = MaxClaims;
                                                                                                             //fprintf(stderr,"%d", ClaimsMatrix[i][j]);
                        //  }
                                                                                                                                               //break;
                                                                                                                                                 //            }//
                                                                                                                                                                 //}
               shmPTR->MaxClaims = MaxClaims;                                                                                                                                                             
               boundmil =  1000*shmPTR->seconds + (int)(shmPTR->nanoseconds/1000000) + value;
               shmPTR->RequestID = processID; sem_post(sem); sem_close(sem);
       srand(getrand++);
               value = 1 + (rand()%100);
       if(value <= 10){ shmPTR->Requests[0] = -2; break;}
         }
     }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
      sem = sem_open("sem1113", 0); sem_wait(sem);
      //remove from claims matrix
      //for(i = 0; i < 30; i++){
        // if(processID == shmPTR->ClaimsMatrix[i][0]){
          //  shmPTR->ClaimsMatrix[i][0] = -2;
           // for(j=1; j< 4; j++){
             //  shmPTR->ClaimsMatrix[i][j] = 0;}
           //break;}
     // } 
        //fprintf(stderr, "Process %d is exiting\n", processID); 
      //shmPTR->RequestID = -2;
      shmPTR->TerminatedProc[shmPTR->termNum] = processID;
      shmPTR->termNum++; sem_post(sem); sem_close(sem);
       shmdt((void *) shmPTR);
    exit(0);
}
