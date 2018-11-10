 //Jenifer Wasson
 //Operating Systems
 //Project 5
 
 //Sources used:  rosettacode.org-banker's algorithm: modified for use in my program
 
#include <semaphore.h>
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
  #include <fcntl.h>  




struct Memory {
     int MaxClaims;
     int  seconds;
     int Release;
     long long int  nanoseconds;
     int TerminatedProc[2];
     int processID;
     int RequestID;
     int Requests[4];
     int termNum;
     };

      int noProcesses = 0;
     int ResourceVector[20];
     int AvailableVector[22];
     int claimX = 0; int claimY = 0;
     //int allocX = 0; int allocY = 0;
     int ClaimsMatrix[30][22];
     int AllocMatrix[30][22];

struct Memory  *shmPTR;
bool signal_interrupt = false;


void  ALARMhandler(int sig)
{ if(signal_interrupt == false)
  write (STDOUT_FILENO,"Alarm went off\n",16);
        signal(SIGALRM, SIG_IGN);
   signal_interrupt = true;
  signal(SIGALRM, ALARMhandler); 
  
}

 int main(int argc, char* argv[])
  {

     int alloc[40];
     int newAlloc[30][22]; int newMax[30][22];

     int running[40];
     FILE *fp;
     int exec = 0;
     bool safe = false;
     int printCount = 0;
     sem_t *sem;
     int milliseconds = 0;
     int childCount = 0;
     int value = 0;
     pid_t childID;
     int bankers = 1;
     
     long int getrand = getpid();
     signal(SIGALRM, ALARMhandler);
     int x;
     int count = 30;
     int ShmID;
     int boundmill = 0;
     int i,j;
     int y;

    //initialize arrays
    for(i = 0; i < 20; i++){
        AvailableVector[i] = 0;
        
        ResourceVector[i] = 0;}
    for(i=0; i<30; i++) {
      for(j=0;j<22;j++) {
       AllocMatrix[i][j] = 0;
       }
     }
     key_t ShmKEY;
     alarm(2); //program can only run 2 seconds;
      ShmKEY = ftok(".", 'x');
     ShmID = shmget(ShmKEY, sizeof(struct Memory), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
    
    shmPTR = (struct Memory *) shmat(ShmID, NULL, 0);
    if (shmPTR == NULL) {
      printf("*** shmat error (server) ***\n");
      exit(1);
    } shmPTR->Release = -2;
    shmPTR->seconds = 1;
    for(i=0; i<30; i++) {
             for(j=0;j<21;j++) {
                       ClaimsMatrix[i][j] = 0;
                             }
                    }
                                

    shmPTR->nanoseconds = 0;
    shmPTR->processID = 0;
     sem = sem_open ("sem1113", O_CREAT | O_EXCL, 0644, 1);
      sem_close(sem);
       for(x = 1; x <= 20; x++){
            srand(getrand++); 
             value = (rand() % (8 - 5)) + 5; //set system resources to 5 to 7 randomly
             //fprintf(stderr,"value is %d \n",value);
             ResourceVector[x] = value;
            
             }
       fp= fopen("logfile.txt", "a");
       if (fp == NULL) {
        fprintf(stderr, "Can't open output file %s!\n",
          "logfile.txt");
        exit(1);
       }               
        
      while(1){if(signal_interrupt == true) break;
          
          //if(noProcesses > 18) break;
          if((milliseconds >= boundmill)&&(childCount < 18)){
            childCount++;  noProcesses++; srand(getrand++);
            value = 1 + (rand()%5); shmPTR->MaxClaims = value; 
            srand(getrand++); //fprintf(stderr, "child count is %d\n", shmPTR->processID);
            value = 1 + (rand()%500); //fork every 1 to 500 milliseconds
        boundmill = shmPTR->seconds*1000 + (int)(shmPTR->nanoseconds/1000000) + value;
       
            if ((childID = fork()) == 0) {
                char *args[]={"./user",NULL};
                execvp(args[0],args);
         
        
        perror("child failed to execvp the command");
        return 1;}} 
      

           else{ //add time and run banker's algorithm
             
    
           milliseconds = (1000*shmPTR->seconds) + (int)(shmPTR->nanoseconds/1000000);
          
           //see if children have exited
          for(y = 0; y < shmPTR->termNum; y++){sem = sem_open("sem1113", 0); sem_wait(sem);
               for(i = 0; i < 30; i ++){
                  if (shmPTR->TerminatedProc[y] == AllocMatrix[i][0]){
                     AllocMatrix[i][0] = -2; ClaimsMatrix[i][0] = -2;
                     
                }}fprintf(fp, "Process %d is exiting at time %d"":""%lld\n", shmPTR->TerminatedProc[y],shmPTR->seconds, shmPTR->nanoseconds);
               shmPTR->Release = -2; shmPTR->TerminatedProc[y] = -2;
               shmPTR->termNum = 0;sem_post(sem); sem_close(sem); childCount--;}


          if(shmPTR->Release != -2){  //if a child is in the critical region
               sem = sem_open("sem1113", 0); sem_wait(sem); printCount++;
               if(shmPTR->Release == 0){
                 fprintf(fp,"Process %d requests %d of resource %d at time %d"":""%lld \n", shmPTR->RequestID,shmPTR->Requests[1], shmPTR->Requests[2],shmPTR->seconds,shmPTR->nanoseconds);
             //enter into claims matrix if not already there
                
                 for(i=0; i<30; i++) { if(ClaimsMatrix[i][0] == shmPTR->RequestID) break;
                     if(ClaimsMatrix[i][0] == 0){
                          ClaimsMatrix[i][0] = shmPTR->RequestID;
                          for(j= 1;j< 21;j++) {
                              ClaimsMatrix[i][j] =   shmPTR->MaxClaims;
                                                                                                             //fprintf(stderr,"%d", ClaimsMatrix[i][j]);
              }                     break;
                                                                                                                                                                                                                    }
                                                                                                                                                                                                                    }
             
             

             //add process to allocation matrix if not already there
                for (i = 0; i < 30; i++)
                { 
                   if(shmPTR->RequestID == AllocMatrix[i][0]){ break;}
                   else if(AllocMatrix[i][0] == 0){
                     AllocMatrix[i][0] = shmPTR->RequestID; break;}
                }
 
               for (i = 0; i < 30; i++){
                 if(shmPTR->RequestID == AllocMatrix[i][0]){
                  for(j = 1; j <= 20; j++){
                    if(j == shmPTR->Requests[2])
                       AllocMatrix[i][j] = AllocMatrix[i][j]  + shmPTR->Requests[1]; //add request to alloc matrix
                  }}
           }
             

            //see if process has exceeded its max claim           
              for (i=0; i < 30; i++){
                if(shmPTR->RequestID == AllocMatrix[i][0]){
                  for( j = 1; j <=20; j++){

                     if(shmPTR->Requests[2] == j){

                        
                        if(AllocMatrix[i][j] > ClaimsMatrix[i][j])
                          { bankers = 0; break;}
                     }
                  }}
                  if( bankers == 0) break;
              }
            //run banker's algorithm
              if(bankers == 1){
                
                  count = 0; int active = 0;
               
                for(int i = 0; i < 30; i++){
                   if((AllocMatrix[i][0] == -2) || (ClaimsMatrix[i][0] == 0)){
                         continue;}
                   else{ 
                      newAlloc[count][0] = AllocMatrix[i][0]; newMax[count][0] = ClaimsMatrix[i][0]; 
                      for(int j = 1; j<=20; j++){ 
                        newAlloc[count][j] = AllocMatrix[i][j];
                        newMax[count][j] = ClaimsMatrix[i][j];}count++;active++;}
                 } 
               
    
                 for (i = 0; i < count; i++) {
                   running[i] = 1;
               
                 }   
 
    
               for (i = 0; i < count; i++){
                 for (j = 1; j < 21; j++)
                   alloc[j] = 0;}
             

               for (i = 0; i < count; i++){
                 for (j = 1; j < 21; j++)
                  alloc[j] += newAlloc[i][j];}
 
             
               for (i = 1; i < 21; i++)
                 AvailableVector[i] = ResourceVector[i] - alloc[i];
 
             
               while (count != 0) {
                 safe = false;
                 for (i = 0; i < active; i++) {
                 
                   if (running[i]) { 
                   
                    exec = 1;
                    for (j = 1; j < 21; j++) { 
                       
                    if (newMax[i][j] - newAlloc[i][j] > AvailableVector[j]) {
                        exec = 0;
                        break;
                    }
                    } 
 
                   if (exec) {
                      //fprintf(stderr,"\nProcess %d is executing.\n", newAlloc[i][0]);
                      running[i] = 0;
                      count--;
                      safe = true; bankers = 1;
                      for (j = 1; j < 21; j++)
                        AvailableVector[j] += newAlloc[i][j];
                      break;
                   }  
                  }
                }
 
                if (!safe) {
                  
                  //fprintf(stderr,"%s","\nThe processes are in unsafe state.");
                  break;
                } 
 
                //if (safe){ 
                   
                  //fprintf(stderr,"%s","\nThe process is in safe state.\n");}
 
                
              }  
 
 




            }
            
           
            if(safe == true){
              fprintf(fp,"Process %d is allocated %d of Resource %d\n",shmPTR->RequestID, shmPTR->Requests[1], shmPTR->Requests[2]);
               
               }

               
               
            else{fprintf(fp, "Process %d is blocked from Resource %d\n", shmPTR->RequestID, shmPTR->Requests[2]);
               for(i = 0; i <30; i++){
                 if(shmPTR->RequestID == AllocMatrix[i][0]){
                    for(j = 1; j<=20; j++){
                      if(j == shmPTR->Requests[2])
                         AllocMatrix[i][j] = AllocMatrix[i][j] - shmPTR->Requests[1];}}}
                 }
                                  
            }
            else{ //if it's signaled to release
                for(i = 0; i < 30; i++){
                  if(shmPTR->RequestID == AllocMatrix[i][0]){
                     for(j = 1; j <= 21; j++){
                       if (j == shmPTR->Requests[2]){
                          if((AllocMatrix[i][j] - shmPTR->Requests[1]) >= 0){
                            fprintf(fp,"Process %d is releasing %d of resource %d at time %d"":""%lld \n", shmPTR->RequestID, shmPTR->Requests[1] ,shmPTR->Requests[2],shmPTR->seconds,shmPTR->nanoseconds);
                            AllocMatrix[i][j] = AllocMatrix[i][j] - shmPTR->Requests[1];}}}}}}
           
               if(printCount%20 == 0){
               fprintf(fp,"%s","\nThe Allocated Resource Table:\n");
               for (i = 0; i < 30; i++) {
                if((AllocMatrix[i][0] == -2) || (AllocMatrix[i][0] == 0))
                  continue;
                else fprintf(fp, "Process %d    ", AllocMatrix[i][0]);
                for (j = 1; j < 21; j++)
                   fprintf(fp,"\t%d", AllocMatrix[i][j]);
                fprintf(fp,"%s","\n");
               }
            fprintf(fp,"%s","\n");}

            bankers = 1; safe = false;

            shmPTR->Release = -2;  sem_post(sem); sem_close(sem);}
                       
            long long int nanoseconds = 0;
            while(nanoseconds < 2000000){
             nanoseconds = nanoseconds + 30;

            }
            shmPTR->nanoseconds = shmPTR->nanoseconds + nanoseconds;
            while(shmPTR->nanoseconds >= 1000000000){
              shmPTR->seconds++;
              shmPTR->nanoseconds = shmPTR->nanoseconds - nanoseconds;}}
       }
             do{ if(signal_interrupt == true) break;
       printf("Clock ticking..\n");
       sleep(1);
      }while (true);
     
   
      shmdt((void *) shmPTR);
       sem_close(sem);
      
       sem_unlink("sem1113");
         
       fclose(fp);
       shmctl(ShmID, IPC_RMID, NULL);
       killpg(getpgid(getpid()), SIGTERM);                //                         printf("Server has removed its shared memory...\n");
      //                                               printf("Server exits...\n");
      //                                                    
                                                               
                                                                    exit(EXIT_SUCCESS);
                                                                         
                                                                           }
      //
