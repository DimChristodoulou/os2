#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h> 
#include <math.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../inc/tree.h"

int main(int argc, char *argv[])
{
    pid_t pid;
    printf("In rootNode with %d pid\n", getpid());
	// fifoPipe = "/tmp/fifo";
	// mkfifo(fifoPipe, 0666);
    int height = atoi(argv[1]);
    int startRead = atoi(argv[2]);
    int endRead = atoi(argv[3]);
    char* fileName = (char*)malloc(100*sizeof(char));
    char* patternName = (char*)malloc(100*sizeof(char));
    strcpy(fileName, argv[4]);
    strcpy(patternName, argv[5]);
    int skew = atoi(argv[6]);
    int numOfRecords = atoi(argv[7]);
    printf("SKEW %d\n", skew);

    if(skew == 1){
        startRead = 1;
        endRead = pow(2, height);
    }

    FILE *fp = fopen("output","w");

    //printf("%d %d %d %s %s\n", height, startRead, endRead,fileName, patternName);

	char **argumentArray = (char**)malloc(15*sizeof(char*));
	
	for(int i = 0; i < 15; i++)
		argumentArray[i] = (char*)malloc(100*sizeof(char));
	
    argumentArray[0] = "exe/splitter-merger";
	sprintf(argumentArray[1], "%d", height);
    //If skew is 0, uniformly distribute ranges
	sprintf(argumentArray[2], "%d", startRead);
	sprintf(argumentArray[3], "%d", endRead);
	argumentArray[4] = fileName;
	argumentArray[5] = patternName;
    sprintf(argumentArray[7], "%d", skew);
    sprintf(argumentArray[8], "%d", numOfRecords);
    sprintf(argumentArray[9], "%d", height);

    char *pidStr = (char*)malloc(20*sizeof(char));
    int myPid = getpid();
    sprintf(pidStr,"%d",myPid);

    char *myRootFifo = (char*)malloc(50*sizeof(char));
    strcat(myRootFifo, "tmp/masterFifo");
    strcat(myRootFifo, pidStr);
    if(mkfifo(myRootFifo, 0666) < 0 ){
        perror("fifo failed!");
    } 
    argumentArray[6] = myRootFifo;

	argumentArray[11] = NULL;

    int childRootFd;

    pid = fork();
    if (pid == 0){
        execvp("exe/splitter-merger", argumentArray);
	}
    else if(pid < 0){
        perror("Error in forking");
    }
    else{        
        char *tempParent = (char*)malloc((5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50)*sizeof(char));
        int retReadRoot = -1;
        childRootFd = open(myRootFifo, O_RDONLY);
        while(retReadRoot != 0){
            retReadRoot = read(childRootFd, tempParent, (5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50));
            printf("\n%s\n",tempParent);
            if( retReadRoot < 0 ){
                perror("Read Fail Root:");
            }
            if(tempParent[0]=='0'){
                printf("STAT: %s", tempParent);
            }
            // else if(tempParent[0]=='S'){
            //     printf("STAT SPLITTER-MERGER: %s", tempParent);
            // }
            else{
                fprintf(fp, "%s\n", tempParent);
            }
        }
    }
    printf("ALL CHILDREN FINISHED\n");

    char **sortArgumentArray = (char**)malloc(15*sizeof(char*));
	for(int i = 0; i < 15; i++)
		sortArgumentArray[i] = (char*)malloc(100*sizeof(char));
    
    sortArgumentArray[0] = "exe/sortNode";
    sortArgumentArray[1] = "output";
    sortArgumentArray[2] = NULL;

    //execvp("exe/sortNode",sortArgumentArray);
    return 0;
}
