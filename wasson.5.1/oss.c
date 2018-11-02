//Jennifer Wasson
//Operating Systems
//Project 5

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
     long long int  nanoseconds;
     int TerminatedProc[2];
     int processID;
     int RequestID;
     int Requests[4];
     int termNum;
     //int childCount;
};

struct Memory  *shmPTR;
bool signal_interrupt = false;

void helpoptions()
{
  printf("options for running Project 5:  \n");
  printf("type ./parent -h for help options\n");
  printf("type ./parent -n -s with values for n and s.\n");

}


//catch alarm
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
     int childCount = 0;
     int value = 0;
     int MaxClaims[20];
     bool forkValue = false;
     long int getrand = getpid();
     signal(SIGALRM, ALARMhandler);
     //int claimsMatrix[4][20];
     
     int x;
     int ShmID;
     int boundmill = 0;
     
     int y;
     long int var[5];
     alarm(2); //program can only run 2 seconds;
     
     int i;//, z;
     key_t ShmKEY;
     //int c,n,s;
     //int hflag;
     //int index;
     //char *nvalue = NULL;
     //char *svalue = NULL;
      opterr = 0;
    //if (argc > 5)
    //{
      // fprintf(stderr, "Too many arguments");
       //return 1;
   // }

    //get address for execvp call
    //for (z=0; z < argc; z++)
   // {
     //  if((argv[z][0] == '-') && (argv[z][1] == 'n'))
       //  break;
   // }
     //while ((c = getopt (argc, argv, "hn:s:")) != -1){
     // switch (c)
       //{
         //case 'h':
         //hflag = 1;
         //break;

         //case 'n':
         // nvalue = optarg;
          //break;
         //case 's':
         // svalue = optarg;
          //break;
         //case '?':
          //if ((optopt == 'n') ||  (optopt == 's'))
           //fprintf (stderr, "Option -%c requires an argument.\n", optopt);
          //else if (isprint (optopt))
          // fprintf (stderr, "Unknown option `-%c'.\n", optopt);
          //else
          // fprintf (stderr,
            //       "Unknown option character `\\x%x'.\n",
              //     optopt);
          //return 1;
          //default:
            //abort ();
      //}
   // }
    
    //for (index = optind; index < argc; index++)
      //printf ("Non-option argument %s\n", argv[index]);
    
     //if(hflag == 1)
    // {
      // helpoptions();
      // return 1;
    // }
     
     
      //change the number user entered into int
      //if(nvalue != NULL){

        // n = atoi(nvalue);
         //if (n == 0){
           //printf("Error:  non-numerical data entered\n");
           //return 1;}
       //}
       //else{
        //printf("%s","Error: no n value  entered\n");
        //return 1;}
       //if(svalue!= NULL){

         //  s = atoi(svalue);
          //if (s == 0){
           //printf("Error:  non-numerical data entered\n");
           //return 1;}
         // }
        // else{
          //printf("%s","Error: no s value entered\n");
         //return 1;
        // }
     //make shared memory
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
    }
    //shmPTR->childCount = 0;
    shmPTR->Requests[0] = -2;
    shmPTR->seconds = 1;
    shmPTR->nanoseconds = 0;
      sem = sem_open ("sem1004", O_CREAT | O_EXCL, 0644, 0);
      sem_close(sem);
    
    //make the resource vector
   for(x = 0; x < 20; x++){
            srand(getrand++); 
            value =  1 + (rand()%10);
             MaxClaims[x] = value;}        
     value =  1 + (rand()%5);  //constant max bound is 5
            shmPTR->MaxClaims = value;
      
      while(1){if(signal_interrupt == true) break;
          
             //if(childCount > 18) break;
          int milliseconds = (1000*shmPTR->seconds) + (int)(shmPTR->nanoseconds/1000000);
        
          if((milliseconds >= boundmill)&&(childCount < 18)){
            childCount++; 
          
            srand(getrand++);
            value = 1 + (rand()%500); //fork every 1 to 500 milliseconds
            //fprintf(stderr, "Value is %d\n", value);
            boundmill = shmPTR->seconds*1000 + (int)(shmPTR->nanoseconds/1000000) + value;
            //shmPTR->processID = MaxProcesses;

            forkValue = true;
            if (fork() == 0) {
            //    shmPTR->processID = (int)getpid();
             
               char *args[]={"./user",NULL};
                execvp(args[0],args);
         
        
        perror("child failed to execvp the command");
        return 1;}} 
      

            

          if(shmPTR->Requests[0] != -2){
            sem = sem_open("sem1004", 0); sem_wait(sem);
            fprintf(stderr,"Process %d requests %d of resource %d \n", shmPTR->RequestID,shmPTR->Requests[1], shmPTR->Requests[2]);
            shmPTR->Requests[0] = -2; }
          //fprintf(stderr, "term 0 is %d\n", shmPTR->TerminatedProc[0]);
          for(y = 0; y < shmPTR->termNum; y++){
               shmPTR->TerminatedProc[y] = -2; //fprintf(stderr, "%s", "hello");
               shmPTR->termNum = 0;childCount--;}
          if(forkValue == true){ 
            value =  1 + (rand()%5);
            shmPTR->MaxClaims = value;
             forkValue = false; }
            long long int nanoseconds = 0;
            while(nanoseconds < 200000000){
             nanoseconds = nanoseconds + 85;

            }
            shmPTR->nanoseconds = shmPTR->nanoseconds + nanoseconds;
            while(shmPTR->nanoseconds >= 1000000000){
              shmPTR->seconds++;
              shmPTR->nanoseconds = shmPTR->nanoseconds - nanoseconds;}
       }
                     
           
           
     
     
    //if (signal_interrupt == true) break;
    //fprintf(stderr, "%ld, %ld, %ld", var[0], var[1], var[2]);
     do{ if(signal_interrupt == true) break;
       printf("Clock ticking..\n");
       sleep(1);
      }while (true);
     
      
     shmdt((void *) shmPTR);
     sem_close(sem);
     sem_unlink("Sem1004");
      killpg(getpgid(getpid()), SIGTERM);
     printf("Server has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("Server has removed its shared memory...\n");
     printf("Server exits...\n");
     
     
     exit(EXIT_SUCCESS);
     
  }




