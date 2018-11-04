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
     //int ClaimsMatrix[30][22];
     long long int  nanoseconds;
     int TerminatedProc[2];
     int processID;
     int RequestID;
     int Requests[4];
     int termNum;
     };

      int noProcesses = 0;
     int ResourceVector[20];
     int AvailableVector[20];
     int claimX = 0; int claimY = 0;
     //int allocX = 0; int allocY = 0;
     int ClaimsMatrix[30][22];
     int AllocMatrix[30][22];

struct Memory  *shmPTR;
bool signal_interrupt = false;

void helpoptions()
{
  printf("options for running Project 5:  \n");
  printf("type ./parent -h for help options\n");
  printf("type ./parent -n -s with values for n and s.\n");

}
bool bankersAlgorithm(){
    
    int x = 0;
   //fprintf(stderr,"Request ID is %d\n", shmPTR->RequestID);
   //make initial check
   for( x= 1; x <= 3; x++)
   {
     if(shmPTR->Requests[2] == x) //check for resource number
       {
         if(shmPTR->Requests[1] > AvailableVector[x])
           return false;
       }
   }
   //decrease available vector and run algorithm
   for(x=1; x <= 3; x++){
      if(shmPTR->Requests[2] == 2)
      { 
          AvailableVector[x] = AvailableVector[x] - shmPTR->Requests[1];
      }
    }
   

    return true;
}
void  ALARMhandler(int sig)
{ if(signal_interrupt == false)
  write (STDOUT_FILENO,"Alarm went off\n",16);
        signal(SIGALRM, SIG_IGN);
   signal_interrupt = true;
  signal(SIGALRM, ALARMhandler); 
  
}

 int main(int argc, char* argv[])
  {
     
     sem_t *sem;
     int milliseconds = 0;
     int childCount = 0;
     int value = 0;
     pid_t childID;
     bool bankers = true;
     int tempAvail[20];
     long int getrand = getpid();
     signal(SIGALRM, ALARMhandler);
     int x;
     int ShmID;
     int boundmill = 0;
     int i,j;
     int y;
    //initialize arrays
    for(i = 0; i < 20; i++){
        AvailableVector[i] = 0;
        tempAvail[i] = 0; 
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
    } shmPTR->Requests[0] = -2;
    shmPTR->seconds = 1;
    for(i=0; i<30; i++) {
             for(j=0;j<4;j++) {
                       ClaimsMatrix[i][j] = 0;
                             }
                    }
                                

    shmPTR->nanoseconds = 0;
    shmPTR->processID = 0;
     sem = sem_open ("sem1113", O_CREAT | O_EXCL, 0644, 1);
      sem_close(sem);
       for(x = 1; x <= 3; x++){
            srand(getrand++); 
            value = (rand() % (7 - 5)) + 5;//rand number from 5 to 10

             ResourceVector[x] = value;
             AvailableVector[x] = value;}
        for(x = 1; x<= 3; x++){
           fprintf(stderr, "Resource Vector value is %d ", AvailableVector[x]);}        
     //value =  1 + (rand()%5);  //constant max bound is 5 for max claims
       //     shmPTR->MaxClaims = value;
      
      while(1){if(signal_interrupt == true) break;
          //int milliseconds = (1000*shmPTR->seconds) + (int)(shmPTR->nanoseconds/1000000);
          if(noProcesses > 2) break;
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
          for(y = 0; y < shmPTR->termNum; y++){sem = sem_open("sem1113", 0); sem_wait(sem);
               shmPTR->TerminatedProc[y] = -2; //fprintf(stderr, "%s", "hello");
               shmPTR->termNum = 0;sem_post(sem); sem_close(sem); childCount--;}


          if(shmPTR->Requests[0] != -2){
               sem = sem_open("sem1113", 0); sem_wait(sem);
               fprintf(stderr,"Process %d requests %d of resource %d \n", shmPTR->RequestID,shmPTR->Requests[1], shmPTR->Requests[2]);
             //enter into claims matrix if not already there
             
             for(i=0; i<30; i++) { if(ClaimsMatrix[i][0] == shmPTR->RequestID) break;
                     if(ClaimsMatrix[i][0] == 0){
                          ClaimsMatrix[i][0] = shmPTR->RequestID;
                          for(j= 1;j< 4;j++) {
                              ClaimsMatrix[i][j] =   shmPTR->MaxClaims;
                                                                                                             //fprintf(stderr,"%d", ClaimsMatrix[i][j]);
              }                                                                                                                                                                       break;
                                                                                                                                                                                                                                                                                                                                          }
                                                                                                  
                                                                                                                                                                                                                                                                                                                                          }
                                                                                                             //
            
            

             //add process to allocation matrix if not already there
             for (i = 0; i < 30; i++)
            { //fprintf(stderr, "alloc matrix is %d\n", AllocMatrix[i][0]);
               if(shmPTR->RequestID == AllocMatrix[i][0]){ break;}
               else if(AllocMatrix[i][0] == 0){
                   AllocMatrix[i][0] = shmPTR->RequestID; break;}
            }
            

            //for (i = 0; i < 20; i++){
            //
              // for(j = 0; j <= 3; j++){
            //
                //                                                                                                                                                                   fprintf(stderr, "%d ",AllocMatrix[i][j]);
                 //                                                                                                                                                             }                                                                                                                                                                                      fprintf(stderr, "%s", "\n");
                   //                                                                                                                                                       }
             //run banker's algorithm            
             for (i=0; i < 30; i++){
               if(shmPTR->RequestID == AllocMatrix[i][0]){
                  for( j = 1; j <=3; j++){
                     if(shmPTR->Requests[2] == j){
                        if(shmPTR->Requests[1] > AvailableVector[j]){
                           bankers = 0; break;}
                        if(shmPTR->Requests[1] + AllocMatrix[i][j] > ClaimsMatrix[i][j])
                          { bankers = 0; break;}
                     }
                  }}
                  if( bankers == 0) break;
             }
            
            fprintf(stderr, "bankers value is %d\n", bankers);
            if(bankers == 1){
               

               for (x = 1; x <=3; x++){
                 if(shmPTR->Requests[2] == x)
                  AvailableVector[x] = AvailableVector[x] - shmPTR->Requests[1];
               }
               for (i = 0; i < 30; i++){
                if(shmPTR->RequestID == AllocMatrix[i][0]){
                  for(j = 1; j <= 3; j++){
                    if(j == shmPTR->Requests[2])
                       AllocMatrix[i][j] = AllocMatrix[i][j]  + shmPTR->Requests[1]; //add request to alloc matrix
                  }}
               }
            }

            bankers = 1;
            

            shmPTR->Requests[0] = -2;  sem_post(sem); sem_close(sem);}
                       
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
     
   for(i=0; i<30; i++) {
             for(j=0;j<4;j++) {
                       fprintf(stderr,"%d ", ClaimsMatrix[i][j]);
                           }
                    fprintf(stderr,"%s", "\n");
                                }

      shmdt((void *) shmPTR);
               sem_close(sem);
      //               sem_unlink("sem1004");
             sem_unlink("sem1113");
         
    
                                           shmctl(ShmID, IPC_RMID, NULL);
       killpg(getpgid(getpid()), SIGTERM);                //                         printf("Server has removed its shared memory...\n");
      //                                               printf("Server exits...\n");
      //                                                    
                                                               
                                                                    exit(EXIT_SUCCESS);
                                                                         
                                                                           }
      //
